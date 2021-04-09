#include<stdio.h>
#include<json-c/json.h>
#include "tlv.h"

#include <apr-1.0/apr_general.h>
#include <apr-1.0/apr_hash.h>
#include <apr-1.0/apr_strings.h>

int printHash(void *rec, const void *key, apr_ssize_t klen, const void *value);

int jsonPacker(FILE *fp, FILE *out)
{
    char buffer[1024];
    struct json_object *parsed_json;
    unsigned char chainbuff[4096] = {0};
    int serOutSize;
    int ret = 0;

    apr_pool_t *mp;
    apr_hash_t *ht;

    if( (ret = apr_initialize()) < 0)
        return ret;

    if( (ret = apr_pool_create(&mp, NULL)) < 0)
        return  ret;


    ht = apr_hash_make(mp);
    tlv_chain_t chain;

    memset(&chain, 0, sizeof(tlv_chain_t));

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
                 case json_type_null:
                    printf("json_type_nulln\n");
                 break;
                 case json_type_boolean:
                        LOGI("json_type_booleann  key = %s val = %d\n", key, json_object_get_boolean(val));

                        if(!apr_hash_get(ht, key, APR_HASH_KEY_STRING))
                         {
                            apr_hash_set(ht, key, APR_HASH_KEY_STRING, (void *)apr_itoa(mp,counter));
                            tlv_chain_add_bool(&chain, json_object_get_boolean(val));
                        }
                 break;
                 case json_type_double:
                        LOGI("json_type_doublen  key = %s val = %f\n", key, json_object_get_double(val));

                        if(!apr_hash_get(ht, key, APR_HASH_KEY_STRING))
                        {
                            apr_hash_set(ht, key, APR_HASH_KEY_STRING, apr_itoa(mp,counter));
                            tlv_chain_add_double(&chain, json_object_get_double(val));
                        }
                 break;
                 case json_type_int:
                        LOGI("json_type_intn  key = %s val = %d\n", key, json_object_get_int(val));

                        if(!apr_hash_get(ht, key, APR_HASH_KEY_STRING))
                        {
                            apr_hash_set(ht, key, APR_HASH_KEY_STRING, apr_itoa(mp,counter));
                            tlv_chain_add_int32(&chain, json_object_get_int(val));
                        }
                 break;
                 case json_type_string:
                        LOGI("json_type_stringn key = %s val = %s\n", key, json_object_get_string(val));

                        if(!apr_hash_get(ht, key, APR_HASH_KEY_STRING))
                        {
                            apr_hash_set(ht, key, APR_HASH_KEY_STRING,(void *) apr_itoa(mp,counter));
                            tlv_chain_add_str(&chain, json_object_get_string(val));
                        }
                 break;
             }
        }
        free(parsed_json);
    }

    tlv_chain_serialize(&chain, chainbuff, &serOutSize);
    fwrite(chainbuff, 1, serOutSize, out);
    apr_hash_do(printHash,out, ht);

    tlv_chain_free(&chain);
    return ret;
}

int printHash(void *rec, const void *key, apr_ssize_t klen, const void *value)
{
        FILE *out = (FILE *) rec;

        int ret = fwrite(key, 1, strlen((char*)key), out);
        return ret;
}

void testDeSerilization(const char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);

    fseek(fp, 0, SEEK_SET);

    unsigned char chainbuff[2048] = {0};
    tlv_chain_t  chain2;

    memset(&chain2, 0, sizeof(tlv_chain_t));

    fread(chainbuff,1, len,fp);

    tlv_chain_deserialize(chainbuff, &chain2, len);

    tlv_chain_print(&chain2);
}

