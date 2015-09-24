/*
 * searchTerm.h
 *
 *  Created on: 2012-11-9
 *      Author: beck
 */

#ifndef SEARCHTERM_H_
#define SEARCHTERM_H_

#include <string>
#include <vector>
#include <map>
/*
 * 函数功能：初始化词典
 * filename，词典的词表路径
 * void*，返回的词典对象的句柄
 */
void* LoadDicFromFile(const char *filename);

/*
 * 函数功能：卸载词典对象
 */
bool UnloadDic(void* dicHandle);

/*
 * 函数功能：查找一个字符串中所有的包含词典的term
 * dicHandle：词典对象
 * inputString：待查找的字符串
 * termsResult：查找到的term集合
 * maxSize：最多输出的term个数，maxSize=-1表示查找所有的可能term
 */
bool searchTerms(void* dicHandle, const char * inputString, std::vector<std::string> &termsResult, int maxSize = -1);
bool searchLongTerms(void* dicHandle, const char * inputString, std::vector<std::string> &termsResult, /*std::map<std::string,int> &m_books,*/ int maxSize = -1);

/*
 * 函数功能：挖掘网页中的影视实体，并输出
 * dicHandle：词典对象
 * title：网页标题
 * content：网页内容
 */
bool webEntityMine(void* dicHandle, const char * title,int titleLength, const char *content, int contentLength, char *output, int &outlength);


#endif /* SEARCHTERM_H_ */
