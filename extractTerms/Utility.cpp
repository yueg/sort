/**
* @file Utility.cpp
* @author Ryan, Beck, Sdctw
* @date Tue Jul 16 14:00:00 CST 2012
* @version 1.3.3
* @brief 
*  
**/

#include "Utility.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
* @brief transform half width character to full width character.
* @param p source string
* @param q target string
* @param map position and length map from target string to source string
* @param qlen the length of target string
* @return number of transformed letters
* @note  nothing
*/

int dbc2sbc(const char *p, char *q, int *map, int qlen)
{
    const char* pOrignalBuffer = p;
    char* pTargetBuffer = q;
    const int dwOrignalLen = (const int)strlen(p);
    int dwTargetLen = qlen;

    /***************************************
    *			字符转换                   *
    ****************************************/
    int   dwOrignalCount = 0;
    int   dwTargetCount  = 0;
    gchar_t uwChineseCode = 0;
    //char* pChieseCode = (char*)&uwChineseCode;
    while(dwOrignalCount < dwOrignalLen){
        if (dwTargetCount + 4 > dwTargetLen) {
            //fprintf(stderr, "DBC2SBC error: target space is not enough.\n");
            printf("utility:%s\n",p);
            break;
        }
        unsigned char b1 = (unsigned char)pOrignalBuffer[dwOrignalCount];

        if (b1 < 0x80){//single byte
            uwChineseCode = b1 + 0x80;
            uwChineseCode<<= 8;
            uwChineseCode|= 0xA3;
            *(gchar_t*)(pTargetBuffer+dwTargetCount) = uwChineseCode;
            map[dwTargetCount] = dwOrignalCount;
            map[dwTargetCount+1] = 1;
            dwOrignalCount++;
            dwTargetCount+= 2;
        }
        else{
            if (dwOrignalCount > dwOrignalLen - 2) break;					//导致最后一个字符可能丢失
            unsigned char  b2 = (unsigned char)pOrignalBuffer[dwOrignalCount+1];
            bool is4Byte = false;
            unsigned char b3,b4;
            if (dwOrignalCount <= (dwOrignalLen - 4)) is4Byte = true;
            if(is4Byte){
                b3 = (unsigned char)pOrignalBuffer[dwOrignalCount+2];
                b4 = (unsigned char)pOrignalBuffer[dwOrignalCount+3];
            }
            if ( is4Byte && ((b1>=0x81&&b1<=0x82 )||(b1>=0x95&&b1<=0x98)) && (b2 >= 0x30 && b2 < 0x40) && (b3>=0x81&&b3<=0xFE) && (b4>=0x30&&b4<=0x39))// four bytes
            {
                pTargetBuffer[dwTargetCount] = (unsigned char)0x81;
                pTargetBuffer[dwTargetCount+1] = (unsigned char)0x30;
                map[dwTargetCount] = dwOrignalCount;
                map[dwTargetCount+1] = 4;
                dwOrignalCount+= 4;
                dwTargetCount+= 2;
            }

                //two bytes
            else {
                gchar_t *g = (gchar_t*)(pOrignalBuffer+dwOrignalCount);
                *(gchar_t*)(pTargetBuffer+dwTargetCount) = *g;
                map[dwTargetCount] = dwOrignalCount;
                map[dwTargetCount+1] = 2;
                dwOrignalCount+= 2;
                dwTargetCount+= 2;
            }
        }
    }
    *(gchar_t*)(pTargetBuffer+dwTargetCount) = 0;

    return dwTargetCount/2;
}

int sbc2dbc(const char *p, char *q, int qLen) {
    const char *pOrignalBuffer = p;
    char *pTargetBuffer = q;
    const int dwOrignalLen = (const int)strlen(p);
    int dwTargetLen = qLen;


    int dwOrignalCount = 0;
    int dwTargetCount  = 0;
    unsigned char uwChineseCode;

    while(dwOrignalCount < dwOrignalLen) {
        if(dwTargetCount + 2 > dwTargetLen) {
            printf("SBC2DBC error: target space is not enough. \n");
            break;
        }
        unsigned char b1 = (unsigned char)pOrignalBuffer[dwOrignalCount++];
        unsigned char b2 = (unsigned char)pOrignalBuffer[dwOrignalCount++];
        if(b1 == 0xA3 && b2 >= 0xA1 && b2 <= 0xFE) {
            uwChineseCode = b2 - 0x80;
            pTargetBuffer[dwTargetCount++] = uwChineseCode;
        }
        else if(b1 == 0xA1 && b2 == 0xA1) {
            uwChineseCode = 0x20;
            pTargetBuffer[dwTargetCount++] = uwChineseCode;
        }
        else {
            pTargetBuffer[dwTargetCount++] = b1;
            pTargetBuffer[dwTargetCount++] = b2;
        }
    }
    pTargetBuffer[dwTargetCount] = '\0';
    return dwTargetCount;
}


/**
* @brief transform half width character to full width character.
* @param p source string
* @param q target string
* @param map position and length map from target string to source string
* @param qlen the length of target string
* @return number of transformed letters
* @note  nothing
*/

int dbc2sbc1(const char *p, char *q, int *map, int& b, const int end)
{
    const char* pOrignalBuffer = p;
    char* pTargetBuffer = q;
    const int dwOrignalLen = (const int)strlen(p);
    bool go_on = true;

    /***************************************
    *			字符转换                   *
    ****************************************/
    int   dwOrignalCount = b;
    int   dwTargetCount  = 0;
    gchar_t uwChineseCode = 0;
    //char* pChieseCode = (char*)&uwChineseCode;
    while(dwOrignalCount < dwOrignalLen && go_on && dwTargetCount < end * 2){
        //if (dwTargetCount + 4 > dwTargetLen) {
        //	fprintf(stderr, "DBC2SBC error: target space is not enough.\n");
        //	break;
        //}
        unsigned char b1 = (unsigned char)pOrignalBuffer[dwOrignalCount];

        if (b1 < 0x80){//single byte
            uwChineseCode = b1 + 0x80;
            uwChineseCode<<= 8;
            uwChineseCode|= 0xA3;
            *(gchar_t*)(pTargetBuffer+dwTargetCount) = uwChineseCode;
            map[dwTargetCount] = dwOrignalCount;
            map[dwTargetCount+1] = 1;
            dwOrignalCount++;
            //dwTargetCount+= 2;
        }
        else{
            if (dwOrignalCount > dwOrignalLen - 2) break;					//导致最后一个字符可能丢失
            unsigned char  b2 = (unsigned char)pOrignalBuffer[dwOrignalCount+1];
            bool is4Byte = false;
            unsigned char b3,b4;
            if (dwOrignalCount <= (dwOrignalLen - 4)) is4Byte = true;
            if(is4Byte){
                b3 = (unsigned char)pOrignalBuffer[dwOrignalCount+2];
                b4 = (unsigned char)pOrignalBuffer[dwOrignalCount+3];
            }
            if ( is4Byte && ((b1>=0x81&&b1<=0x82 )||(b1>=0x95&&b1<=0x98)) && (b2 >= 0x30 && b2 < 0x40) && (b3>=0x81&&b3<=0xFE) && (b4>=0x30&&b4<=0x39))// four bytes
            {
                pTargetBuffer[dwTargetCount] = (unsigned char)0x81;
                pTargetBuffer[dwTargetCount+1] = (unsigned char)0x30;
                map[dwTargetCount] = dwOrignalCount;
                map[dwTargetCount+1] = 4;
                dwOrignalCount+= 4;
                //dwTargetCount+= 2;
            }

                //two bytes
            else {
                gchar_t *g = (gchar_t*)(pOrignalBuffer+dwOrignalCount);
                *(gchar_t*)(pTargetBuffer+dwTargetCount) = *g;
                map[dwTargetCount] = dwOrignalCount;
                map[dwTargetCount+1] = 2;
                dwOrignalCount+= 2;
                //dwTargetCount+= 2;
            }
        }
        //mark
        if ( dwTargetCount>SENSIZE*3/4 &&(COMMA_CONVERSION_CHAR == *(gchar_t*)(pTargetBuffer+dwTargetCount) ||
                                          PERIOD_CONVERSION_CHAR == *(gchar_t*)(pTargetBuffer+dwTargetCount) ||
                                          Q_MARK_CONVERSION_CHAR == *(gchar_t*)(pTargetBuffer+dwTargetCount) ||
                                          EXCLAMATION_CONVERSION_CHAR == *(gchar_t*)(pTargetBuffer+dwTargetCount)))
        {
            go_on = false;
        }
        dwTargetCount+= 2;
    }
    *(gchar_t*)(pTargetBuffer+dwTargetCount) = 0;
    if (dwOrignalCount < dwOrignalLen)
    {
        b = dwOrignalCount;
    }
    else b = -1;

    return dwTargetCount/2;
}
