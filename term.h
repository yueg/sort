//
// Created by yueg on 9/22/15.
//

#ifndef SORTNEWS_TERM_H
#define SORTNEWS_TERM_H
#include <string>
#include <deque>

class Term {
 public:
  Term(std::string word, float weight);
  ~Term();
  void UpdateTerm(int time, int count);
  void DeleteTimeOutTerm();
  float GetTermHeat();
  void UpdateHeat(int time);

 private:
  std::string word_;
  std::deque<int> count_;
  std::deque<int> timeStamp_;
  float heat_;
  int createTime_;
  float weight_;
};


#endif //SORTNEWS_TERM_H
