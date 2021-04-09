#include <stdio.h>
#include "jsonPacker.h"

#define NDEBUG  1


int main(int argc, char **argv) {
    FILE *fp, *out;

    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s filename.json outputFile\n", argv[1]);
        return -1;
    }

    if(!(fp = fopen(argv[1],"r")))
    {
        fprintf(stderr, "Can not open file %s\n", argv[1]);
        return -1;
    }

    if(!(out = fopen(argv[2], "wb")))
    {
            fprintf(stderr, "Can not open file %s for output\n", argv[2]);
            return -1;
    }

    jsonPacker(fp,out);
    fclose(out);
    fclose(fp);

    testDeSerilization(argv[2]);

    return 0;;
}
