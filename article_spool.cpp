//
// Created by yueg on 9/17/15.
//
#include "article_spool.h"
#include "./extractTerms/searchTerm.h"
#include "./extractTerms/Utility.h"

using namespace std;

#define MAXSIZE 1000
// #define TERMSPATH "./conf/innotree/business.seg"
#define TERMSPATH "/home/yueg/innotree/sort/data/business.seg"

ArticleSpool::ArticleSpool() {
  createTime_ = (int)time(0);
  updateTime_ = createTime_;
  spoolMax_ = MAXSIZE;
  spoolSize_ = 0;
  heapUtil_ = new HeapUtil(MAXSIZE);
  termTable_ = new TermSpool(TERMSPATH);
  word_dic_.push_back(LoadDicFromFile(TERMSPATH));
  word_weight_.push_back(1);
}

ArticleSpool::~ArticleSpool() {
  delete termTable_;
  delete heapUtil_;
  for (size_t i = 0; i < word_dic_.size(); i++) {
    UnloadDic(word_dic_[i]);
  }
}

void ArticleSpool::Push(const string &url, int pubtime, const string &title, const string &content) {
  updateTime_ = (int)time(0);
  Article *article = new Article(pubtime, title, content, url, termTable_);
  map<string, int> word_map;
  GetWordMapFromArticle(article, &word_map);
  termTable_->UpdateTermCount(word_map, article->GetArticleTime());
  UpdateArticleHeat(article, word_map);
  heapUtil_->InsertIntoHeap(article, true);
}

void ArticleSpool::GetArticleOfMaxHeat(int size, vector<Article> *out) const {
  HeapUtil *outHeap = new HeapUtil(size);
  vector<Article *> outArticleAddr;
  vector<Article *> articleHeap;
  heapUtil_->GetHeap(articleHeap);
  if (static_cast<int>(articleHeap.size()) < size) {
    outArticleAddr = articleHeap;
  }
  else {
    outHeap->BuildHeap(articleHeap, size, true);
    outHeap->GetHeap(outArticleAddr);
  }
  out->clear();
  for (size_t i = 0; i < outArticleAddr.size(); i++) {
    out->push_back(*(outArticleAddr[i]));
  }
  delete outHeap;
}


void ArticleSpool::UpdateAllArticle() {
  termTable_->DeleteTimeOutTermInSpool();
  vector<Article *> article_vec;
  heapUtil_->GetHeap(article_vec);
  for (size_t i = 0; i < article_vec.size(); i++) {
    map<string, int> word_map;
    GetWordMapFromArticle(article_vec[i], &word_map);
    UpdateArticleHeat(article_vec[i], word_map);
  }
  heapUtil_->BuildHeap(article_vec, MAXSIZE, true);
}

void ArticleSpool::GetWordMapFromArticle(const Article *article, map<string, int> *word_map) {
  vector<string> word_vec;
  string input_str = article->GetArticleTitle() + article->GetArticleTitle() + article->GetArticleTitle() + article->GetArticleContent();
  for (size_t i = 0; i < word_dic_.size(); i++) {
    vector<string> temp;
    searchLongTerms(word_dic_[i], input_str.c_str(), temp, 100);
    word_vec.insert(word_vec.end(), temp.begin(), temp.end());
  }
  char temp[1024];
  for (size_t i = 0; i < word_vec.size(); i++) {
    sbc2dbc(word_vec[i].c_str(), temp, 1024);
    string word(temp);
    map<string, int>::iterator iter = word_map->find(word);
    if (iter == word_map->end()) {
      word_map->insert(make_pair(word, 1));
    } else {
      (*word_map)[word]++;
    }
  }
}

void ArticleSpool::UpdateArticleHeat(Article *article, const map<string, int> &word_map) {
  float article_heat = 0;
  for (map<string, int>::const_iterator iter = word_map.begin(); iter != word_map.end(); iter++) {
    Term *term = termTable_->GetTermByWord(iter->first);
    article_heat += iter->second * term->GetTermHeat();
  }
  article->SetArticleHeat(article_heat);
}

