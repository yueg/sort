/**
* @file simpleDic.h
* @author  Beck,
*
**/
#include <math.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <map>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include "simpleDic.h"
#include "Utility.h"
using namespace std;
using namespace analysis;

simpleDic* simpleDic::dinstance=NULL;

simpleDic::simpleDic() {
	textpointer = 0;
	texts = NULL;
	count = 0;
	terms = 0;
	for (int i=0;i<MAX_ENTITY_WORD_LEN;i++){
		sizes[i] = 0;
	}
	memset(nodemap,0,sizeof(nodemap));
}

simpleDic::~simpleDic() {
	if (terms) delete [] terms;
	if (texts) delete texts;
	for (int i=0;i<MAX_ENTITY_WORD_LEN;i++)
		if (nodemap[i]) {
			delete [] nodemap[i];
		}
}
#define readInt(n) {\
	fread(&n,sizeof(int),1,fin);\
};

#define readStr(str) {\
	int l;\
	readInt(l);\
	fread(str,1,l,fin);\
	str[l]=0;\
};

#define readBuf(buf,count,type) {\
	readInt(count);\
	if (buf==NULL) buf=new type[count+100];fread(buf,sizeof(type),count,fin);\
};

bool simpleDic::loadDic(const char* fname)
{
	FILE* fin = fopen(fname,"rb");
	if (!fin) {
		fprintf(stderr,"load simpleDic err, can't open %s \n",fname);
		return false;
	}

	char buf[200];
	readStr(buf);
	fprintf(stderr,"simpleDic last modifyed time is:%s\n",buf);

	readBuf(texts,textpointer,UINT16);
	readBuf(terms,count,TermInfo);
	for (int i=0;i<MAX_ENTITY_WORD_LEN;i++) {
		readBuf(nodemap[i],sizes[i],TermTreeNode);
	}

	fclose(fin);
	return true;
}

#define writeInt(intn) {\
	n = intn;\
	fwrite(&n,sizeof(int),1,fout);\
};

#define writeStr(str) {\
	int l=strlen(str);\
	writeInt(l);\
	fwrite(str,1,l,fout);\
};

#define writeBuf(buf,count,len) {\
	writeInt(count)\
	fwrite(buf,len,count,fout);\
};
bool simpleDic::saveDic(const char* fname)
{
	FILE* fout = fopen(fname,"wb");
	if (!fout)
		return false;

	//time
	size_t n=0;

	time_t t= time(NULL);
	char *ct = ctime(&t);
	writeStr(ct);
	fprintf(stderr,"simpleDic modifyed timeflag is:%s\n",ct);

	writeBuf(texts,textpointer,sizeof(UINT16));	//9861kb
	writeBuf(terms,count,sizeof(TermInfo));		//65579kb

	//13692kb
	//sizes[0]=65536;
 	for (int i=0;i<MAX_ENTITY_WORD_LEN;i++) {
 		writeBuf(nodemap[i],sizes[i],sizeof(TermTreeNode));
 	}
	fclose(fout);
	return true;
}

int uint_len(const UINT16 *data)
{
	int x = 0;
	while(data[x]!=0)
		x++;
	return x;
}
typedef pair<string, string> pPAIR;
int cmpstr(const pPAIR& x, const pPAIR& y)
{
	return strcmp(x.first.c_str(),y.first.c_str())<0;
}
bool simpleDic::makeDicTree(vector<string> v_Terms)
{
	if(v_Terms.size() == 0) return false;
	//1.sort
	vector<pPAIR> vTerms;
	char cdata[MAX_ENTITY_WORD_LEN*2+4];
	int cmap[MAX_ENTITY_WORD_LEN*2+4];
	char data[12];
	int new_size[MAX_ENTITY_WORD_LEN] = {0};
	for(int i=0;i<v_Terms.size();i++){
		string str;
		int nlen = dbc2sbc(v_Terms[i].c_str(),cdata,cmap,MAX_ENTITY_WORD_LEN*2+4);
		if(nlen<1|| nlen > MAX_ENTITY_WORD_LEN)
			continue;
		cdata[2*nlen] = '\0';
		new_size[nlen-1] +=1 ;
		if(1==nlen){
			//printf("a=%s_\n",cdata);
			continue;
		}
		UINT16 *temp=(UINT16*)cdata;
		for(int j=0;j<nlen;j++){
			sprintf(data,"%d",temp[j]);
			str += data;
		}
		vTerms.push_back(make_pair(str, cdata));
	}
	sort(vTerms.begin(), vTerms.end(),cmpstr);//ÅÅÐò
	for(int i= MAX_ENTITY_WORD_LEN-1; i>0; --i){
		new_size[i-1] += new_size[i];
	}
	new_size[0] = 65536;
	//
	texts = new UINT16[vTerms.size()*100];
	terms = new TermInfo[vTerms.size()+100];
	texts[0] = 0;
	textpointer = 1;
	count = vTerms.size()+1;
	sizes[0] = 65536;
	nodemap[0] = new TermTreeNode[65536];//65536
	for(int i=1;i<MAX_ENTITY_WORD_LEN;i++){
		sizes[i] = 0;
		if(i>0){
			if(new_size[i]!=0)
				nodemap[i] = new TermTreeNode[new_size[i]+200];
			else
				nodemap[i] = NULL;
		}
	}
	for(int i=0;i<65536;i++){
		nodemap[0][i].text = i;
		nodemap[0][i].count = 0;
		nodemap[0][i].ptr = -1;
	}
	//
	//FILE *fp =fopen("sort.txt","w");
	int addsize = 0;
	int addtriesize = 0;
	int onesize = 0;
	for(int i=0;i<vTerms.size();i++){
		pPAIR m_pari = vTerms.at(i);
		//fprintf(fp,"%s\t%d\n",m_pari.first.c_str(),m_pari.second.pageCount);
		string tempInfo = m_pari.second;
		int nlen = dbc2sbc(tempInfo.c_str(),cdata,cmap,MAX_ENTITY_WORD_LEN*2+4);
		if(nlen<1|| nlen > MAX_ENTITY_WORD_LEN) continue;
		cdata[2*nlen] = '\0';
		if(1==nlen){
			//printf("b=%s_\n",cdata);
			continue;
		}
		UINT16* itemp = (UINT16 *)cdata;
		int len = nlen;
		bool isAddTrie = false;
		//trie
		TermTreeNode* tempNode = getEntry(itemp[0]);
		if(len==1){
			tempNode->count = 1;
			tempNode->ptr = -(i+1);
			onesize++;
			isAddTrie = true;
		}
		for(int j=1;j<len;j++){
			TermTreeNode* node = findNode(tempNode,itemp[j],j);
			if(NULL==node){
				tempNode->count++;
				if(tempNode->ptr<0){
					if(j==1&&tempNode->count==1)
						tempNode->ptr = sizes[1];
					else{	//Ìí¼ÓÐéÄâ½Úµã
						nodemap[j][sizes[j]].count = 1;
						nodemap[j][sizes[j]].text = 0;
						nodemap[j][sizes[j]].ptr = tempNode->ptr;
						tempNode->ptr = sizes[j];
						sizes[j]++;
					}
					while(j!=len-1){
						nodemap[j][sizes[j]].count = 1;
						nodemap[j][sizes[j]].text = itemp[j];
						nodemap[j][sizes[j]].ptr = sizes[j+1];
						sizes[j]++;
						j++;
					}
					nodemap[j][sizes[j]].count = 1;
					nodemap[j][sizes[j]].text = itemp[j];
					nodemap[j][sizes[j]].ptr = -(i+1);
					sizes[j]++;
				}
				else{
					while(j!=len-1){
						nodemap[j][sizes[j]].count = 1;
						nodemap[j][sizes[j]].text = itemp[j];
						nodemap[j][sizes[j]].ptr = sizes[j+1];
						sizes[j]++;
						j++;
					}
					nodemap[j][sizes[j]].count = 1;
					nodemap[j][sizes[j]].text = itemp[j];
					nodemap[j][sizes[j]].ptr = -(i+1);
					sizes[j]++;
				}
				isAddTrie = true;
				break;
			}
			else {
				tempNode = node;
			}
		}
		addsize++;
		if(isAddTrie==false) {
			//printf("trie:%s\n",cdata);
			continue;
		}
		addtriesize++;
		//terms
		terms[i+1].termOffset = textpointer;
		terms[i+1].len = len;
		//texts
		for (int k=0;k<len;k++)	{
			texts[textpointer++] = itemp[k];
		}
		texts[textpointer++]=0;
	}
//	for(int i=0;i<MAX_ENTITY_WORD_LEN;++i){
//		printf("newsize=%d_%d sizes=%d\n",i,new_size[i], sizes[i]);
//	}
	//fprintf(stderr,"sortsize=%d,adddicsize=%d,addtriesize=%d,onesize=%d\n",vTerms.size(),addsize,addtriesize,onesize);
	//fclose(fp);

	return true;
}

bool simpleDic::makeDicFromFile(const char * filename)
{
	FILE *fp = fopen(filename,"r");
	if(NULL==fp) {
		fprintf(stderr,"open file error:%s\n",filename);
		return false;
	}
	char data[2048];
	int size = 0;
	map<string,int> m_file_terms;
	vector<string> vTerms;
	while(fgets(data,2048,fp))
	{
		int len = strlen(data);
		if(len<1||len>120) continue;
		data[len] = '\0';
		if(len>2&&data[len-2]=='\r'&&data[len-1]=='\n'){
			data[len-2] = '\0';
			len -=2;
		}
		if(data[len-1]=='\n' || data[len-1]=='\t'){
			data[len-1] = '\0';
			--len;
		}
		while(data[len-1]==' '&&len>0){
			data[len-1]='\0';
			--len;
		}
		if(0==len) continue;
		map<string,int>::iterator iter = m_file_terms.find(data);
		if(iter!= m_file_terms.end()){
			continue;
		}
		else{
			m_file_terms.insert(make_pair(data,1));
			vTerms.push_back(data);
			++size;
		}
	}
	fclose(fp);
	//fprintf(stderr, "file:%s\n\thave termsize=%d\n",filename, size);
	bool bMake = makeDicTree(vTerms);
	/*if(bMake)
		printf("Dic size = %d.\nmake Dic sucess.\n",count-1);
	else
		printf("make Dic error.\n");*/
	return bMake;
}
