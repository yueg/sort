//
// Created by yueg on 9/17/15.
//

#ifndef SORTNEWS_TERMSPOOL_CLASS_H
#define SORTNEWS_TERMSPOOL_CLASS_H
#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <set>
#include <time.h>
#include "term.h"

class TermSpool {
 public:
  TermSpool(const std::string &termFilePath);
  void UpdateTermCount(const std::map<std::string, int> &aticleTerm, int flushtime);
  Term *GetTermByWord(const std::string &word);
  void DeleteTimeOutTermInSpool();

 private:
  std::vector<Term *> termSpool_;
  std::map<std::string, Term *> termMap_;
  int createTime_;
  int updateTime_;
};


#endif //SORTNEWS_TERMS_CLASS_H
