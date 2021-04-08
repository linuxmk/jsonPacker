#include "tlv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int32_t tlv_chain_add_bool( tlv_chain_t *a, int8_t x)
{
    return tlv_chain_add_raw(a, 1, 1, (unsigned char *)&x);
}

int32_t tlv_chain_add_int32( tlv_chain_t *a, int32_t x)
{
    return tlv_chain_add_raw(a, 2, sizeof (x), (unsigned char *)&x);
}

int32_t tlv_chain_add_double(tlv_chain_t *a, double x)
{
    return tlv_chain_add_raw(a, 3, sizeof (x), (unsigned char *)&x);
}
// add tlv object which contains null terminated string
int32_t tlv_chain_add_str( tlv_chain_t *a,  const char *str)
{
    return tlv_chain_add_raw(a, 4, strlen(str) + 1, (unsigned char *)str);
}

int32_t tlv_chain_add_raw(  tlv_chain_t *a, unsigned char type, int16_t size, const unsigned char *bytes)
{
    if(a == NULL || bytes == NULL)
        return -1;

    // all elements used in chain?
    if(a->used == 50)
        return -1;

    int index = a->used;
    a->object[index].type = type;
    a->object[index].size = size;
    a->object[index].data = malloc(size);
    memcpy(a->object[index].data, bytes, size);

    // increase number of tlv objects used in this chain
    a->used++;

    // success
    return 0;
}

int32_t tlv_chain_free( tlv_chain_t *a)
{
    if(a == NULL)
        return -1;

    for(int i =0; i < a->used; i++)
    {
        free(a->object[i].data);

        a->object[i].data = NULL;
    }

    return 0;
}

// serialize the tlv chain into byte array
int32_t tlv_chain_serialize( tlv_chain_t *a, unsigned char *dest, /* out */ int32_t* count)
{
    if(a == NULL || dest == NULL)
        return -1;

    // Number of bytes serialized
    int32_t counter = 0;

    for(int i = 0; i < a->used; i++)
    {
        dest[counter] = a->object[i].type;
        counter++;

        memcpy(&dest[counter], &a->object[i].size, 2);
        counter += 2;

        memcpy(&dest[counter], a->object[i].data, a->object[i].size);
        counter += a->object[i].size;
    }

    // Return number of bytes serialized
    *count = counter;

    // success
    return 0;
}

int32_t tlv_chain_deserialize( const unsigned char *src,  tlv_chain_t *dest, int32_t length)
{
    if(dest == NULL || src == NULL)
        return -1;

    // we want an empty chain
    if(dest->used != 0)
        return -1;

    int32_t counter = 0;
    while(counter < length)
    {
        if(dest->used == 50)
            return -1;

        // deserialize type
        dest->object[dest->used].type = src[counter];
        counter++;

        // deserialize size
        memcpy(&dest->object[dest->used].size, &src[counter], 2);
        counter+=2;

        // deserialize data itself, only if data is not NULL
        if(dest->object[dest->used].size > 0)
        {
            dest->object[dest->used].data = malloc(dest->object[dest->used].size);
            memcpy(dest->object[dest->used].data, &src[counter], dest->object[dest->used].size);
            counter += dest->object[dest->used].size;
        }else
        {
            dest->object[dest->used].data = NULL;
        }

        // increase number of tlv objects reconstructed
        dest->used++;
    }

    // success
    return 0;

}



int32_t tlv_chain_print( tlv_chain_t *a, FILE *fp)
{
    if(a == NULL)
        return -1;

    // go through each used tlv object in the chain
    for(int i =0; i < a->used; i++)
    {

        switch (a->object[i].type)
        {
            case 1:
            {
            uint8_t x;
            memcpy(&x, a->object[i].data, sizeof(uint8_t));
            fprintf(fp,"%d \n",x);
            }
            break;
        case 2:
            {
                uint32_t x;
                memcpy(&x, a->object[i].data, sizeof(uint32_t));
                fprintf(fp,"%d \n",x);
            }
            break;
        case 3:
        {
            double x;
            memcpy(&x, a->object[i].data, sizeof(double));
            fprintf(fp,"%f \n",x);
        }
            break;

        case 4:

              fprintf(fp,"%s \n",a->object[i].data);
            break;

           }
    }


    return 0;
}
