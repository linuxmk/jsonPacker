#ifndef jsonpacker_h
#define jsonpacker_h

#include<stdio.h>
#include<json-c/json.h>
#include "tlv.h"

#include <apr-1.0/apr_general.h>
#include <apr-1.0/apr_hash.h>
#include <apr-1.0/apr_strings.h>

int jsonPacker(FILE *fp, FILE *out);
void testDeSerilization(const char *filename);
int printHash(void *rec, const void *key, apr_ssize_t klen, const void *value);

#endif
