/**
* @file simpleDic.h
* @author Beck,
*
**/
#ifndef ENTITYDIC_H_
#define ENTITYDIC_H_

#ifndef NULL
#define NULL 0
#endif
#include <string>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#define MAX_ENTITY_WORD_LEN		50
typedef unsigned short UINT16;
typedef unsigned int UINT32;

namespace analysis {
#pragma pack(1)
	struct TermInfo
	{
		UINT32 	termOffset;
		char 	len;
	};
	/*
	词典树的中间节点。
	 */
	struct TermTreeNode {
		UINT16 	text;
		short 	count;
		int 	ptr;
	};
#pragma pack()

	class simpleDic {
	public:
		static simpleDic * dinstance;
		simpleDic();
		virtual ~simpleDic();
		//
		int 	textpointer;					//texts的长度
		UINT16*	texts;							//词典中所有的term内容
		int 	count;							//词表数量(TermInfo数量)
		TermInfo *terms;						//词条属性数组
		TermTreeNode* nodemap[MAX_ENTITY_WORD_LEN];	//词表的trie树结构，最大深度为15
		int sizes[MAX_ENTITY_WORD_LEN];			//词表的trie树结构

	public:
		//返回单体实例
		static simpleDic* instance(){
			if (dinstance==NULL) dinstance = new simpleDic();
			return  dinstance;
		}
		//销毁单体实例
		static void releaseInstance() {
			if (dinstance!=NULL) {
				delete dinstance;
				dinstance = NULL;
			}
		}
		//载入词库/字信息库的内存镜像
		bool	loadDic(const char* fname);
		bool	saveDic(const char* fname);
		bool 	makeDicFromFile(const char * filename);
		bool 	makeDicTree(vector<string> v_Terms);
		//
		inline TermTreeNode* getEntry(UINT16 text) {
			return &nodemap[0][text];
		}
		inline TermTreeNode * getNode(size_t len, size_t index){
			return &nodemap[len][index];
		}

		inline TermInfo * getTermNode(int offset) {
			return &terms[offset];
		}

		// 词条数量
		inline int size() {
			return count-1;
		}

		// binary search方法查找前缀数的下一个节点。
		inline TermTreeNode * findNode(TermTreeNode *cnode, UINT16 text, UINT16 len) {
			if(cnode->ptr<0)  return NULL;
			int i=cnode->ptr;
			int j=cnode->ptr+cnode->count;
			while (i+1<=j) {
				if (i+1==j) {
					if (nodemap[len][i].text==text) return &nodemap[len][i];
					return NULL;
				}
				int  k = (i+j)>>1;
				int v = (int)nodemap[len][k].text - (int) text;
				if (v==0) return &nodemap[len][k];
				else if (v<0) i = k; else j=k;
			}
			return NULL;
		}

		// 寻找某个字符串的TermNode
		TermInfo * searchText(UINT16* text, UINT16 len) {
			if(len==0 || len> MAX_ENTITY_WORD_LEN) return NULL;
			TermTreeNode *node = getEntry(text[0]);
			if(len==1){
				if(node->count==0) return NULL;
				else if (node->ptr<0) return &this->terms[-node->ptr];
				else if (nodemap[len][node->ptr].text==0) {
					return &this->terms[-nodemap[len][node->ptr].ptr];
				}
				return NULL;
			}
			int i=1;
			while (node->count>0 && i<len) {
				node = findNode(node,text[i],i);
				i++;
				if (NULL == node)	break;
				if (i==len) {
					if (node->ptr<0) return &this->terms[-node->ptr];
					else if (nodemap[len][node->ptr].text==0) {
						return &this->terms[-nodemap[len][node->ptr].ptr];
					}
				}
			}
			return NULL;
		}
		//大小写转换
		int ToLowerCase(const char* src,char* dest,int maxDestLen){
			if(NULL == src) return 0;
			int len = strlen(src);
			int n_len = 0;
			for(int i=0; i<len&&i<maxDestLen;++i){
				//				if(s[i]>='a' && s[i]<='z')
				//					s[i]=s[i]-32;
				if(src[i]>='A' && src[i]<='Z')
					dest[i]=src[i]+32;
				else
					dest[i] = src[i];
				++n_len;
			}
			return n_len;
		}

	};
}
#endif /* ENTITYDIC_H_ */
