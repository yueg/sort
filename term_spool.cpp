//
// Created by yueg on 9/17/15.
//

#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <string.h>
#include "term_spool.h"


using namespace std;

TermSpool::TermSpool(const string &termFilePath) {
  FILE *fp = fopen(termFilePath.c_str(), "r");
  if(NULL==fp) {
    fprintf(stderr,"open file error:%s\n", termFilePath.c_str());
  } else {
    char s[2048];
    while(fgets(s, 2048, fp)) {
      int len = (int)strlen(s);
      for(int i = 0; i < len; i++) {
        if(s[i] == '\n') {
          s[i] = '\0';
        }
      }
      Term *term = new Term(s, 1);
      termSpool_.push_back(term);
      termMap_.insert(make_pair(s, term));
    }
    fclose(fp);
  }
  createTime_ = (int)time(0);
  updateTime_ = createTime_;
}


void TermSpool::UpdateTermCount(const map<string, int> &aticleTerm, int flushtime) {
  for (map<string, int>::const_iterator iter = aticleTerm.begin(); iter != aticleTerm.end(); iter++) {
    Term *term = termMap_[iter->first];
    term->UpdateTerm(flushtime, iter->second);
  }
  updateTime_ = (int)time(0);
}

Term* TermSpool::GetTermByWord(const std::string &word) {
  return termMap_[word];
}


void TermSpool::DeleteTimeOutTermInSpool() {
  for (size_t i = 0; i < termSpool_.size(); i++) {
    termSpool_[i]->DeleteTimeOutTerm();
  }
}
