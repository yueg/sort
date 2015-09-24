//
// Created by yueg on 8/24/15.
//

#include <stdio.h>
#include <malloc.h>

#include "fileOperation.h"
#include <iostream>
#include <string.h>
#include "tl_codetbl.h"
using namespace std;
using namespace encoding;

bool writeRes2File(const char *filename, string str) {
    FILE *fp = fopen(filename, "w");
    if(NULL == fp) {
        fprintf(stderr, "open result file error:%s\n", filename);
        return false;
    }
    int stringLen = str.size();
    int count = fwrite(str.c_str(), stringLen, sizeof(char), fp);
    fclose(fp);
}

char * readInputFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if(NULL == fp) {
        fprintf(stderr, "open input file error:%s\n", filename);
        return NULL;
    }
    fseek(fp,0L,SEEK_END);
    int flen = ftell(fp);
    cout << flen << endl;
    char *str = (char *)malloc(flen+1);
    fseek(fp,0L,SEEK_SET);
    fread(str,flen,1,fp);
    str[flen]=0;
    return str;
}
