//
// Created by yueg on 9/17/15.
//

#ifndef SORTNEWS_ARTICLESPOOL_H
#define SORTNEWS_ARTICLESPOOL_H
#include <vector>
#include <map>
#include "article.h"
#include "term_spool.h"
#include "heap_util.h"


class ArticleSpool
{
 public:
  ArticleSpool();
  ~ArticleSpool();
  void Push(const std::string &url, int pubtime, const std::string &title, const std::string &content);
  void GetArticleOfMaxHeat(int size, std::vector<Article> *out) const;
  void UpdateAllArticle();
  void GetWordMapFromArticle(const Article *article, std::map<std::string, int> *word_map); 
  void UpdateArticleHeat(Article *article, const std::map<std::string, int> &word_map);

 private:
  int createTime_;
  int updateTime_;
  int spoolMax_;
  int spoolSize_;
  HeapUtil *heapUtil_;
  TermSpool *termTable_;
  std::vector<void *> word_dic_;
  std::vector<float> word_weight_;
};


#endif //SORTNEWS_ARTICLESPOOL_H
