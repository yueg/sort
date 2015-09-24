//
// Created by yueg on 9/22/15.
//

#include <math.h>
#include "term.h"
#include <algorithm>

Term::Term(std::string word, float weight) {
  word_ = word;
  heat_ = 0;
  createTime_ = (int)time(0);
  count_.clear();
  timeStamp_.clear();
  weight_ = weight;
}

void Term::UpdateTerm(int time, int count) {
  count_.push_back(count);
  timeStamp_.push_back(time);
  UpdateHeat(time);
}

void Term::DeleteTimeOutTerm() {
  int now = (int)time(0);
  int deleteTime = now / 60 - 360;
  for (int i = 0; timeStamp_[i] <= deleteTime && !timeStamp_.empty(); i++) {
    timeStamp_.pop_front();
    count_.pop_front();
  }
}

float Term::GetTermHeat() {
  return heat_;
}

void Term::UpdateHeat(int time){
  float heat = 0;
  for (int i = (int)count_.size() - 1; i >= 0; i--) {
    int min = (time - timeStamp_[i]) / 60;
    if (min > 360) {
      break;
    }
    heat += count_[i] / sqrt(min + 1);
  }
  heat_ = heat;
}
