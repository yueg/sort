/*
 * searchTerm.cpp
 *
 *  Created on: 2012-11-9
 *      Author: beck
 */
#include <iostream>
#include "searchTerm.h"
#include "simpleDic.h"
#include "Utility.h"
#include "tl_codetbl.h"
#include <map>

using namespace analysis;
using namespace encoding;
using namespace std;

void* LoadDicFromFile(const char *filename)
{
	if (NULL == filename) {
		fprintf(stderr,"file path is empty\n");
		return NULL;
	}

	try{
		simpleDic* myDic = simpleDic::instance();
		bool b = myDic->makeDicFromFile(filename);
		//cout << (int)b << endl;
		if(b)
			return myDic;
	}
	catch(char *err){
		fprintf(stderr,"Load Dic error: %s\n", err);
		return NULL;
	}
	catch(...){
		fprintf(stderr,"Load Dic error: unknown reason\n");
		return NULL;
	}
	return NULL;
}

bool UnloadDic(void* dicHandle)
{
	if (NULL != dicHandle){
		simpleDic* mydics = (simpleDic *)dicHandle;
		mydics->releaseInstance();
		mydics = NULL;
	}

	return true;
}

bool searchTerms(void* dicHandle, const char * inputString, std::vector<std::string> &termsResult, int maxSize)
{
	if(NULL == dicHandle){
		fprintf(stderr,"dicHandle is NULL.\n");
		return false;
	}
	if(NULL == inputString){
		fprintf(stderr,"inputString is NULL.\n");
		return false;
	}
	termsResult.clear();
	if(0 == maxSize){
		return true;
	}

	simpleDic* myDic = (simpleDic*)dicHandle;
//	for(int i=1;i<myDic->count; ++i){
//		TermInfo *cur = &myDic->terms[i];
//		printf("%s\t%d\n",(char*)(myDic->texts+cur->termOffset),cur->len);
//	}
	int find_size =0;
	int len = strlen(inputString);
	char cdata[len*2+4];
	int cmap[len*2+4];
	int nlen = dbc2sbc(inputString,cdata,cmap,len*2+4);
	cdata[2*nlen] = '\0';
	UINT16* gdata = (UINT16*)cdata;
	map<string,int> m_have_Terms;

	for(int pos = 0; pos<nlen; ++pos)
	{
		TermTreeNode *cnode = myDic->getEntry(gdata[pos]), *node;
		bool found = cnode->count>0, finishnode;
		bool bEnough = false;
		int curlen = 0;
		while (pos+curlen<nlen && found)
		{
			node = cnode;
			finishnode = false;
			if (curlen>=0) {
				if (cnode->ptr>=0) {
					node = myDic->getNode(curlen+1,cnode->ptr);
					finishnode = node->text==0;
				} else {
					node = cnode;
					finishnode = node->text!=0;
				}
			}
			//找到了一个叶节点，匹配到一个词。
			if (finishnode) {
				TermInfo *cur = myDic->getTermNode(-node->ptr);
				char * str_term = (char*)(myDic->texts+cur->termOffset);
				map<string,int>::iterator iter = m_have_Terms.find(str_term);
				if(iter != m_have_Terms.end()){

				}
				else{
					m_have_Terms.insert(make_pair(str_term,1));
					++find_size;
					termsResult.push_back(str_term);
					if(maxSize>0 &&find_size>=maxSize){
						bEnough = true;
						break;
					}
				}
			}
			if(cnode->ptr<0)
				break;
			curlen++;
			cnode = myDic->findNode(cnode, gdata[pos+curlen], curlen);
			found = cnode!=NULL;
		}//end while
		if(bEnough) break;
	}//for


	return true;
}

bool searchLongTerms(void* dicHandle, const char * inputString, std::vector<std::string> &termsResult, /*std::map<std::string,int> &m_books,*/ int maxSize)
{
	if(NULL == dicHandle){
		fprintf(stderr,"dicHandle is NULL.\n");
		return false;
	}
	if(NULL == inputString){
		fprintf(stderr,"inputString is NULL.\n");
		return false;
	}
	termsResult.clear();
	if(0 == maxSize){
		return true;
	}

	simpleDic* myDic = (simpleDic*)dicHandle;
//	for(int i=1;i<myDic->count; ++i){
//		TermInfo *cur = &myDic->terms[i];
//		printf("%s\t%d\n",(char*)(myDic->texts+cur->termOffset),cur->len);
//	}
	int find_size =0;
	int len = strlen(inputString);
	char cdata[len*2+4];
	int cmap[len*2+4];
	int nlen = dbc2sbc(inputString,cdata,cmap,len*2+4);
	cdata[2*nlen] = '\0';
	UINT16* gdata = (UINT16*)cdata;
	map<string,int> m_have_Terms;

//	const char * bookMark="《》";
//	UINT16* gbook = (UINT16*)bookMark;

	for(int pos = 0; pos<nlen; )
	{
		bool bEnough = false;

//		//find 《》
//		if(gdata[pos] == gbook[0]){
//			int endpos = pos+1;
//			bool isFind = false;
//			while(endpos-pos <17){
//				if( gdata[endpos] == gbook[1]){
//					isFind = true;
//					break;
//				}
//				endpos++;
//			}
//			if(isFind){
//				string findTerm;
//				findTerm.assign(cdata+2*(pos+1), 2*(endpos-pos-1) );
//				m_books.insert(make_pair(findTerm,1));
//				map<string, int>::iterator iter = m_have_Terms.find(findTerm);
//				if (iter != m_have_Terms.end()) {
//
//				} else {
//					m_have_Terms.insert(make_pair(findTerm, 1));
//					++find_size;
//					termsResult.push_back(findTerm);
//					if (maxSize > 0 && find_size >= maxSize) {
//						bEnough = true;
//					}
//				}
//
//				pos = endpos; //1
//				if(bEnough) break;
//				continue;
//			}
//		}

		//entity
		//printf("pos=%d\n", pos);
		TermTreeNode *cnode = myDic->getEntry(gdata[pos]), *node;
		bool found = cnode->count>0, finishnode;
		int curlen = 0;

		string findTerm = "";
		int addpos = 1;
		while (pos+curlen<nlen && found)
		{
			node = cnode;
			finishnode = false;
			if (curlen>=0) {
				if (cnode->ptr>=0) {
					node = myDic->getNode(curlen+1,cnode->ptr);
					finishnode = node->text==0;
				} else {
					node = cnode;
					finishnode = node->text!=0;
				}
			}
			//找到了一个叶节点，匹配到一个词。
			if (finishnode) {
				TermInfo *cur = myDic->getTermNode(-node->ptr);
				char * str_term = (char*)(myDic->texts+cur->termOffset);
				//printf("term:%s,curlen=%d,len=%d\n",str_term,curlen,cur->len);
				addpos = cur->len;
				findTerm = str_term;
			}
			if(cnode->ptr<0)
				break;
			curlen++;
			cnode = myDic->findNode(cnode, gdata[pos+curlen], curlen);
			found = cnode!=NULL;
		}//end while
		//a long term
		if(findTerm.length()>0){
			//printf("findTerm:%s\n",findTerm.c_str());
			/*map<string, int>::iterator iter = m_have_Terms.find(findTerm);
			if (iter != m_have_Terms.end()) {

			} else {
				m_have_Terms.insert(make_pair(findTerm, 1));

			}*/
			++find_size;
			termsResult.push_back(findTerm);
			if (maxSize > 0 && find_size >= maxSize) {
				bEnough = true;
				break;
			}
		}
		//go to len
		//if (curlen!=0) printf("curlen=%d",curlen);
		pos += addpos; //1

		if(bEnough) break;
	}//for

	return true;
}
string ToDBS(string str) {
    string result = "";
    unsigned char tmp; unsigned char tmp1;
    for (unsigned int i = 0; i < str.length(); i++) {
        tmp = str[i];
        tmp1 = str[i + 1];
        //cout << "uchar:" << (int) tmp << endl;
        if (tmp == 163) {///第一个字节是163，标志着是全角字符
            result += (unsigned char) str[i + 1] - 128;
            i++;
            continue;
        } else if (tmp > 163) {//汉字
            result += str.substr(i, 2);
            i++;
            continue;
        } else if (tmp == 161 && tmp1 == 161) {///处理全角空格
            result += "";
            i++;
        } else {
            result += str.substr(i, 1);
        }
    }
    return result;
}

void split(std::string src, std::string token, vector<std::string>& vect)
{
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = src.find_first_of(token, nbegin);
        if(nend == -1)
            vect.push_back(src.substr(nbegin, src.length()-nbegin));
        else
            vect.push_back(src.substr(nbegin, nend-nbegin));
        nbegin = nend + 1;
    }
}


bool webEntityMine(void* dicHandle, const char * title,int titleLength, const char *content, int contentLength, char *output, int &outlength)
{
	std::map<string,int> m_terms;
	m_terms.insert(make_pair("电视剧",1));
	m_terms.insert(make_pair("电影",1));
	m_terms.insert(make_pair("色情片",1));
	m_terms.insert(make_pair("电视连续剧",1));
	m_terms.insert(make_pair("大片",1));
	m_terms.insert(make_pair("动漫",1));
	m_terms.insert(make_pair("动画片",1));
	m_terms.insert(make_pair("恐怖片",1));
	m_terms.insert(make_pair("影片",1));
	m_terms.insert(make_pair("情色片",1));
	m_terms.insert(make_pair("科幻片",1));
	m_terms.insert(make_pair("武打片",1));
	m_terms.insert(make_pair("佳片",1));
	m_terms.insert(make_pair("贺岁片",1));
	m_terms.insert(make_pair("动作片",1));
	m_terms.insert(make_pair("喜剧片",1));
	m_terms.insert(make_pair("爱情片",1));
	m_terms.insert(make_pair("悬疑片",1));
	m_terms.insert(make_pair("战争片",1));
	m_terms.insert(make_pair("警匪片",1));
	m_terms.insert(make_pair("古装片",1));
	m_terms.insert(make_pair("韩剧",1));
	m_terms.insert(make_pair("港剧",1));
	m_terms.insert(make_pair("美剧",1));
	m_terms.insert(make_pair("台剧",1));
	m_terms.insert(make_pair("日剧",1));
	m_terms.insert(make_pair("泰剧",1));
	m_terms.insert(make_pair("禁片",1));
	m_terms.insert(make_pair("惊悚片",1));
	m_terms.insert(make_pair("鬼片",1));
	m_terms.insert(make_pair("猛片",1));


	std::map<string,int> m_suffixs;
	m_suffixs.insert(make_pair("好看",1));
	m_suffixs.insert(make_pair("经典",1));
	m_suffixs.insert(make_pair("推荐",1));
	m_suffixs.insert(make_pair("介绍",1));
	m_suffixs.insert(make_pair("十大",1));
	m_suffixs.insert(make_pair("大全",1));
	m_suffixs.insert(make_pair("排行",1));
	m_suffixs.insert(make_pair("必看",1));
	m_suffixs.insert(make_pair("0部",1));
	m_suffixs.insert(make_pair("十部",1));
	m_suffixs.insert(make_pair("什么",1));
	m_suffixs.insert(make_pair("演过的",1));
	m_suffixs.insert(make_pair("演的",1));
	m_suffixs.insert(make_pair("主演",1));
	m_suffixs.insert(make_pair("目录",1));
	m_suffixs.insert(make_pair("最新",1));
	m_suffixs.insert(make_pair("时间表",1));
	m_suffixs.insert(make_pair("全集",1));
	m_suffixs.insert(make_pair("不错的",1));
	m_suffixs.insert(make_pair("热播",1));
	m_suffixs.insert(make_pair("好笑",1));
	m_suffixs.insert(make_pair("著名的",1));


	std::map<string,int> m_delSuffixs;
	m_delSuffixs.insert(make_pair("剧情介绍",1));
	m_delSuffixs.insert(make_pair("分集介绍",1));
	m_delSuffixs.insert(make_pair("全集介绍",1));
	m_delSuffixs.insert(make_pair("演员表介绍",1));

	unsigned int outputMaxLength = outlength;
	outlength = 0;
	output[0]='\0';

	if(NULL == dicHandle){
		return false;
	}
	if(NULL == title || NULL == content){
		return false;
	}
	bool isDebug = false;

	//title
	if (NULL != title && titleLength > 0) {
		char buf[1024];
		int len = encoding::conv_utf8_2_gbk(title, titleLength, buf,1024);
		if (len <= 0) {
			return false;
		}
		buf[len] = '\0';
		string str_buf(buf);
		bool isFindTerm = false;
		for(std::map<string,int>::iterator iter = m_terms.begin(); iter != m_terms.end(); iter++){
			unsigned int pos = str_buf.find(iter->first);
			if( pos >=0 && pos< str_buf.length() && pos <50 ){
				isFindTerm = true;
				break;
			}
		}
		bool isFindSuffix = false;
		for (std::map<string, int>::iterator iter = m_suffixs.begin();iter != m_suffixs.end(); iter++) {
			unsigned int pos = str_buf.find(iter->first);
			if ( pos >=0 && pos< str_buf.length() && pos <50 ) {
				isFindSuffix = true;
				break;
			}
		}
		bool isDelSuffix = false;
		for (std::map<string, int>::iterator iter = m_delSuffixs.begin(); iter != m_delSuffixs.end(); iter++) {
			unsigned int pos = str_buf.find(iter->first);
			if (pos >= 0 && pos < str_buf.length()) {
				isDelSuffix = true;
				break;
			}
		}

		if(false == isFindSuffix || false == isFindTerm){
			return false;
		}
		if(isDelSuffix){
			return false;
		}
	}else{
		return false;
	}

	//content
	if (NULL != content && contentLength > 0) {
		if(isDebug)  printf("\n[content len:%d]:%s\n",contentLength,content);
		string str_buf(content);
		std::vector<string> v_contents;
		split(str_buf,"[br]",v_contents);


		std::map<std::string,int> m_shortContents;
		std::vector<std::vector<string> > v_entitys;
		std::map<string,int> m_result;
		std::map<std::string,int> m_allbooks; //gbk
		std::vector<std::string> v_shortResults;
		std::map<std::string,int> m_shortResults;
		for(unsigned int j=0; j<v_contents.size(); j++){
			if ( v_contents.at(j).length() > 2 && v_contents.at(j).length() < 90) {  //utf-8
				m_shortContents.insert(make_pair(v_contents.at(j), 1));
			}

			char buf[65536];
			int len = encoding::conv_utf8_2_gbk(v_contents.at(j).c_str(), v_contents.at(j).length(), buf, 65536);
			if (len <= 0) {
				continue;
			}
			buf[len] = '\0';
			str_buf = buf;


			std::vector<std::string> termsResult;
			searchLongTerms(dicHandle, str_buf.c_str(), termsResult/*, m_allbooks*/);

			if(termsResult.size()>0){
				std::vector<string> entity_tmp;
				if(isDebug)  printf("\n term[%d]:", (int)termsResult.size());

				for(unsigned int i=0; i< termsResult.size(); i++){
					string str_buf = ToDBS(termsResult.at(i));
					if(strcmp(str_buf.c_str(), "对不起我爱你")==0){
						str_buf = "对不起,我爱你";
					}
					char tbuf[1024];
					len = encoding::conv_gbk_2_utf8(str_buf.c_str(), str_buf.length(), tbuf, 1024);
					if (len <= 0) {
						continue;
					}
					tbuf[len] = '\0';
					if(isDebug)  printf("\t%s",tbuf);

					// add short
					if( m_shortContents.size()>0 ){
						bool isShort =false;
						for(std::map<std::string, int>::iterator iter = m_shortContents.begin(); iter != m_shortContents.end(); iter++){
							unsigned int pos = iter->first.find(tbuf);
							if(pos>=0 && pos < iter->first.length()-1){
								isShort = true;
								break;
							}
						}
						if(isShort){
							std::map<string,int>::iterator iter_m = m_shortResults.find(tbuf);
							if (iter_m == m_shortResults.end()) {
								bool isChild = false;
								for(std::map<string,int>::iterator iter_tmp = m_shortResults.begin();iter_tmp != m_shortResults.end(); ){
									if( NULL != strstr(iter_tmp->first.c_str(), tbuf) ) {
										isChild = true;
										break;
									}else if( NULL != strstr(tbuf, iter_tmp->first.c_str()) ) {
										m_shortResults.erase(iter_tmp++);
									}else{
										iter_tmp++;
									}
								}
								if (false == isChild) {
									v_shortResults.push_back(tbuf);
									m_shortResults.insert(make_pair(tbuf, 1));
									if (isDebug) printf("[short]");
								}
							}
						}
					}

					//只要《》
					if( m_allbooks.find(termsResult.at(i)) == m_allbooks.end() ){
						continue;
					}

					std::map<string,int>::iterator iter_m = m_result.find(tbuf);
					if (iter_m == m_result.end()) {
						bool isChild = false;
						for (std::map<string, int>::iterator iter_tmp = m_result.begin();iter_tmp != m_result.end();) {
							if ( NULL != strstr(iter_tmp->first.c_str(), tbuf)) {
								isChild = true;
								break;
							} else if ( NULL != strstr(tbuf, iter_tmp->first.c_str())) {
								m_result.erase(iter_tmp++);
							} else {
								iter_tmp++;
							}
						}

						if (false == isChild) {
							entity_tmp.push_back(tbuf);
							m_result.insert(make_pair(tbuf,1));
							if(isDebug)  printf("[other]");
						}
					}
				}
				if(isDebug)  printf("\n");

				if(entity_tmp.size()>0){
					v_entitys.push_back(entity_tmp);
				}
			}
		}

		//result1 short content
		if( m_shortResults.size()>3 || ( m_result.size()<m_shortResults.size() && m_shortResults.size() >1 ) ){
			int cnt = 0;
			if (isDebug) printf("[short]\t");
			for (unsigned int j = 0; j < v_shortResults.size(); j++) {
				std::map<string,int>::iterator iter_m = m_shortResults.find(v_shortResults.at(j));
				if (iter_m == m_shortResults.end()) {
					continue;
				}
				if (j == 0) {			//add 2015-4-13
					if (strcmp(title, v_shortResults.at(j).c_str()) == 0) {
						break;
					}
				}
				if (isDebug)	printf("%s\t", v_shortResults.at(j).c_str());

				if ((v_shortResults.at(j).length() + 1 + outlength)	>= outputMaxLength) {
					break;
				}
				sprintf(output + outlength, "%s\t", v_shortResults.at(j).c_str());
				outlength += (v_shortResults.at(j).length() + 1);
				cnt++;
				if (cnt > 15)
					break;
			}

			output[outlength] = '\0';

			if (isDebug && v_entitys.size() > 0) {
				if (isDebug) printf("\n");
			}

			if (outlength > 0) {
				return true;
			}
		}
		//result2
		unsigned int max_cnt = 15;
		if(v_entitys.size()>8) {
			max_cnt = 1;
		}
		int cnt = 0;
		for(unsigned int i = 0; m_result.size()>1 && i< v_entitys.size(); i++){
			std::vector<std::string> entity_tmp = v_entitys.at(i);
			bool isBreak =false;
			for(unsigned int j=0; j<entity_tmp.size() && j<max_cnt; j++){
				std::map<string,int>::iterator iter_m = m_result.find(entity_tmp.at(j));
				if (iter_m == m_result.end()) {
					if(1==max_cnt && entity_tmp.size()>1){
						j ++;
					}else{
						continue;
					}
				}
				if (i ==0 && j == 0) {			//add 2015-4-13
					if (strcmp(title, entity_tmp.at(j).c_str() ) == 0) {
						isBreak = true;
						break;
					}
				}
				if(isDebug) printf("%s\t", entity_tmp.at(j).c_str() );

				if( (entity_tmp.at(j).length()+1 + outlength) >= outputMaxLength ){
					break;
				}
				sprintf(output+outlength, "%s\t", entity_tmp.at(j).c_str());
				outlength += (entity_tmp.at(j).length()+1);
				cnt ++;
				if(cnt>15) break;
			}
			if(cnt>15 || isBreak) break;
		}
		output[outlength]='\0';

		if( isDebug && v_entitys.size()>0 ){
			if(isDebug) printf("\n");
		}

		if( outlength >0 ) {
			return true;
		}

	}else{
		return false;
	}


	return false;
}
