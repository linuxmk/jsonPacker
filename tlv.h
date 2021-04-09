
#ifndef tlv_h
#define tlv_h
#include <stdint.h>
#include <stdio.h>

#define MAX_TLV_OBJECTS 100


#ifdef Debug
#define LOGI(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define LOGI(fmt, ...) do {} while (0)
#endif

// TLV data structure
typedef struct tlv
{
    int8_t type;    // type
    uint8_t * data; // pointer to data
    int16_t size;   // size of data
}tlv_t;

// TLV chain data structure. Contains array of MAX_TLV_OBJECTS tlv objects.
typedef struct tlv_chain
{
    tlv_t object[MAX_TLV_OBJECTS];
    uint8_t used; // keep track of tlv elements used
}tlv_chain_t;

int32_t tlv_chain_add_bool( tlv_chain_t *a, int8_t x);
int32_t tlv_chain_add_int32( tlv_chain_t *a, int32_t x);
int32_t tlv_chain_add_double(tlv_chain_t *a, double x);
int32_t tlv_chain_add_str(tlv_chain_t *a, const char *str);
int32_t tlv_chain_add_raw( tlv_chain_t *a, unsigned char type, int16_t size, const unsigned char *bytes);
int32_t tlv_chain_serialize( tlv_chain_t *a, unsigned char *dest, int32_t *count);
int32_t tlv_chain_deserialize(const unsigned char *src, tlv_chain_t *dest, int32_t length);
int32_t tlv_chain_print(tlv_chain_t *a);
int32_t tlv_chain_free( tlv_chain_t *a);

#endif
