//
// Created by yueg on 9/17/15.
//

#ifndef SORTNEWS_ARTICLE_H
#define SORTNEWS_ARTICLE_H
#include <string>
#include <map>
#include "term_spool.h"

class Article {
 public:
  Article(int article_time, const std::string &title, const std::string &content, const std::string &url, const TermSpool *t);
  void GetTermMap(std::map<std::string, int> &ret) const;
  std::string GetArticleTitle() const;
  std::string GetArticleContent() const;
  float GetArticleHeat() const;
  void SetArticleHeat(float heat);
  int GetArticleTime();

 private:
  int article_time_;
  float article_heat_;
  std::string title_;
  std::string content_;
  std::string url_;
};


#endif //SORTNEWS_ARTICLE_H
