
#include<stdio.h>
#include<json-c/json.h>
#include "tlv.h"

#include <apr-1.0/apr_general.h>
#include <apr-1.0/apr_hash.h>
#include <apr-1.0/apr_strings.h>

void extractJson(FILE *fp, FILE *out)
{
    char buffer[1024];
    struct json_object *parsed_json;


    apr_pool_t *mp;
    apr_hash_t *ht;

    apr_initialize();
    apr_pool_create(&mp, NULL);

    ht = apr_hash_make(mp);
    tlv_chain_t chain, chain2;


    memset(&chain, 0, sizeof(chain));
    memset(&chain2, 0, sizeof(chain2));

    while(fgets(buffer,1024, fp))
    {
        parsed_json = json_tokener_parse(buffer);
        int type;
        int counter = 0;

        json_object_object_foreach(parsed_json,key, val)
        {
            ++counter;
            type = json_object_get_type(val);
             switch (type)
             {
                 case json_type_null: printf("json_type_nulln\n");
                 break;
                 case json_type_boolean:
                        printf("json_type_booleann  key = %s val = %d\n", key, json_object_get_boolean(val));
                        apr_hash_set(ht, key, APR_HASH_KEY_STRING, (void *)apr_itoa(mp,counter));

                        tlv_chain_add_bool(&chain, json_object_get_boolean(val));
                 break;
                 case json_type_double:
                        printf("json_type_doublen  key = %s val = %f\n", key, json_object_get_double(val));
                        apr_hash_set(ht, key, APR_HASH_KEY_STRING, apr_itoa(mp,counter));

                        tlv_chain_add_double(&chain, json_object_get_double(val));

                 break;
                 case json_type_int:
                        printf("json_type_intn  key = %s val = %d\n", key, json_object_get_int(val));
                        apr_hash_set(ht, key, APR_HASH_KEY_STRING, apr_itoa(mp,counter));

                        tlv_chain_add_int32(&chain, json_object_get_int(val));

                 break;
                 case json_type_string:
                        printf("json_type_stringn key = %s val = %s\n", key, json_object_get_string(val));

                        apr_hash_set(ht, key, APR_HASH_KEY_STRING,(void *) apr_itoa(mp,counter));

                        tlv_chain_add_str(&chain, json_object_get_string(val));

                 break;
             }


        }

//        {
//               const char *val = apr_hash_get(ht, "key1", APR_HASH_KEY_STRING);
//               printf("val for \"foo\" is %d\n", atoi(val));
//           }



}

    unsigned char chainbuff[2048] = {0};
    int l;


    tlv_chain_serialize(&chain, chainbuff, &l);
    size_t ret = fwrite(chainbuff, 1, l, out);


}

int main(int argc, char **argv) {
	FILE *fp;

    if(argc !=2)
    {
        fprintf(stderr, "Usage: %s filename.json\n", argv[1]);
        return -1;
    }

    if(!(fp = fopen(argv[1],"r")))
    {
        fprintf(stderr, "Can not open file %s\n", argv[1]);
        return -1;
    }
    FILE *out = fopen("output.bin", "wb");

    extractJson(fp,out);
    fclose(out);


    fclose(fp);

    out = fopen("output.bin", "rb");
    fseek(out, 0, SEEK_END);
    size_t len = ftell(out);

    fseek(out, 0, SEEK_SET);

    unsigned char chainbuff[2048] = {0};
    tlv_chain_t  chain2;
    fread(chainbuff,1, len,out);

    tlv_chain_deserialize(chainbuff, &chain2, len);

    tlv_chain_print(&chain2,stderr);

    return 0;;
}
