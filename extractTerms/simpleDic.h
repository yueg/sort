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
	�ʵ������м�ڵ㡣
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
		int 	textpointer;					//texts�ĳ���
		UINT16*	texts;							//�ʵ������е�term����
		int 	count;							//�ʱ�����(TermInfo����)
		TermInfo *terms;						//������������
		TermTreeNode* nodemap[MAX_ENTITY_WORD_LEN];	//�ʱ��trie���ṹ��������Ϊ15
		int sizes[MAX_ENTITY_WORD_LEN];			//�ʱ��trie���ṹ

	public:
		//���ص���ʵ��
		static simpleDic* instance(){
			if (dinstance==NULL) dinstance = new simpleDic();
			return  dinstance;
		}
		//���ٵ���ʵ��
		static void releaseInstance() {
			if (dinstance!=NULL) {
				delete dinstance;
				dinstance = NULL;
			}
		}
		//����ʿ�/����Ϣ����ڴ澵��
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

		// ��������
		inline int size() {
			return count-1;
		}

		// binary search��������ǰ׺������һ���ڵ㡣
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

		// Ѱ��ĳ���ַ�����TermNode
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
		//��Сдת��
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
