//
// Created by yueg on 9/17/15.
//

#include "article.h"
using namespace std;

Article::Article(int article_time, const string &title, const string &content, const string &url, const TermSpool *termSpool) {
  article_time_ = article_time;
  title_ = title;
  content_ = content;
  url_ = url;
  article_heat_ = 0;
}

float Article::GetArticleHeat() const {return article_heat_; }

string Article::GetArticleTitle() const {return title_; }

string Article::GetArticleContent() const {return content_; }

void Article::SetArticleHeat(float heat) {article_heat_ = heat; }

int Article::GetArticleTime() {return article_time_; }
