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
 * �������ܣ���ʼ���ʵ�
 * filename���ʵ�Ĵʱ�·��
 * void*�����صĴʵ����ľ��
 */
void* LoadDicFromFile(const char *filename);

/*
 * �������ܣ�ж�شʵ����
 */
bool UnloadDic(void* dicHandle);

/*
 * �������ܣ�����һ���ַ��������еİ����ʵ��term
 * dicHandle���ʵ����
 * inputString�������ҵ��ַ���
 * termsResult�����ҵ���term����
 * maxSize����������term������maxSize=-1��ʾ�������еĿ���term
 */
bool searchTerms(void* dicHandle, const char * inputString, std::vector<std::string> &termsResult, int maxSize = -1);
bool searchLongTerms(void* dicHandle, const char * inputString, std::vector<std::string> &termsResult, /*std::map<std::string,int> &m_books,*/ int maxSize = -1);

/*
 * �������ܣ��ھ���ҳ�е�Ӱ��ʵ�壬�����
 * dicHandle���ʵ����
 * title����ҳ����
 * content����ҳ����
 */
bool webEntityMine(void* dicHandle, const char * title,int titleLength, const char *content, int contentLength, char *output, int &outlength);


#endif /* SEARCHTERM_H_ */
