#include<stdio.h>
#include<json-c/json.h>

#include <apr-1.0/apr_general.h>
#include <apr-1.0/apr_hash.h>
#include <apr-1.0/apr_strings.h>

void extractJson(FILE *fp)
{
    char buffer[1024];
    struct json_object *parsed_json;
    struct json_object *name;
    struct json_object *age;
    struct json_object *friends;
    struct json_object *friend;
    size_t n_friends;


    apr_pool_t *mp;
        apr_hash_t *ht;

        apr_initialize();
        apr_pool_create(&mp, NULL);

        ht = apr_hash_make(mp);


    while(fgets(buffer,1024, fp))
    {
        parsed_json = json_tokener_parse(buffer);
        enum json_type type;

        json_object_object_foreach(parsed_json,key, val)
        {
            type = json_object_get_type(val);
             switch (type) {
             case json_type_null: printf("json_type_nulln\n");
             break;
             case json_type_boolean: printf("json_type_booleann  key = %s val = %d\n", key, json_object_get_boolean(val));
             break;
             case json_type_double: printf("json_type_doublen  key = %s val = %f\n", key, json_object_get_double(val));
             break;
             case json_type_int: printf("json_type_intn  key = %s val = %d\n", key, json_object_get_int(val));
             break;
             case json_type_object: printf("json_type_objectn\n");
             break;
             case json_type_array: printf("json_type_arrayn\n");
             break;
             case json_type_string:
                 printf("json_type_stringn key = %s val = %s\n", key, json_object_get_string(val));
             break;
             }
        }

        json_object_object_get_ex(parsed_json, "key1", &name);
        json_object_object_get_ex(parsed_json, "key2", &age);
        json_object_object_get_ex(parsed_json, "key3", &friends);

        printf("Name: %s\n", json_object_get_string(name));
        printf("Age: %d\n", json_object_get_int(age));

        n_friends = json_object_get_boolean(friends);
        printf("Found %lu friends\n",n_friends);

//        for(i=0;i<n_friends;i++) {
//            friend = json_object_array_get_idx(friends, i);
//            printf("%lu. %s\n",i+1,json_object_get_string(friend));
//        }
    }
}

int main(int argc, char **argv) {
	FILE *fp;


	size_t i;	

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

    extractJson(fp);


    fclose(fp);
    return 0;;
}
