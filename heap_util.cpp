//
// Created by yueg on 9/18/15.
//

#include "heap_util.h"
using namespace std;



HeapUtil::HeapUtil(int size) {
  heap_size_ = 0;
  heap_.reserve(size);
  heap_max_ = size;
}

bool HeapUtil::InsertIntoHeap(const Article *article, bool type) {
  bool ret = false;
  float articleHeat = article->GetArticleHeat();
  if (heap_size_ == 0) {
    ret = true;
    heap_.push_back((Article *)article);
    heap_size_ = (int)heap_.size();
  }
  else if (heap_size_ == heap_max_ && articleHeat > heap_[0]->GetArticleHeat()) {
    ret = true;
    delete heap_[0];
    heap_[0] = (Article *)article;
    this->AdjustHeap(0, heap_size_, type);
  }
  else if (heap_size_ < heap_max_) {
    ret = true;
    heap_size_ += 1;
    heap_.push_back((Article *)article);
    for (int i = heap_size_ / 2; i >= 1; i = (i - 1) / 2) {
      this->AdjustHeap(i, heap_size_, type);
    }
  }
  return ret;
}

void HeapUtil::BuildHeap(const vector<Article *> articleVec,int n, bool type) {
    heap_.clear();
    heap_.reserve(n);
    int i;
    vector<Article *>::const_iterator iter;
    for(i = 0, iter = articleVec.begin(); i < n && iter != articleVec.end(); i++, iter++) {
        heap_.push_back(*iter);
    }
    heap_size_ = i;
    for(int j = heap_size_ / 2; j >= 1; j--) {
        this->AdjustHeap(j, heap_size_, type);
    }
    for(; iter != articleVec.end(); iter++) {
        if(type && heap_size_ > 0 && (*iter)->GetArticleHeat() > heap_[0]->GetArticleHeat()) {
            heap_[0] = *iter;
            this->AdjustHeap(0, heap_size_, type);
        }
        else if(!type && heap_size_ > 0 && (*iter)->GetArticleHeat() < heap_[0]->GetArticleHeat()) {
            heap_[0] = *iter;
            this->AdjustHeap(0, heap_size_, type);
        }
    }
}

void HeapUtil::AdjustHeap(int i, int size, bool type) {
    int lchild = 2 * i;
    int rchild = 2 * i + 1;
    int max = i;
    if(i <= size / 2 && type) {
        if(lchild < size && heap_[lchild]->GetArticleHeat() < heap_[max]->GetArticleHeat()) {
            max = lchild;
        }
        if(rchild < size && heap_[rchild]->GetArticleHeat() < heap_[max]->GetArticleHeat()) {
            max = rchild;
        }
        if(max != i) {
            swap(heap_[i], heap_[max]);
            AdjustHeap(max, size, type);
        }
    }
    else if(i <= size / 2 && !type) {
        if(lchild < size && heap_[lchild]->GetArticleHeat() > heap_[max]->GetArticleHeat()) {
            max = lchild;
        }
        if(rchild < size && heap_[rchild]->GetArticleHeat() > heap_[max]->GetArticleHeat()) {
            max = rchild;
        }
        if(max != i) {
            swap(heap_[i], heap_[max]);
            AdjustHeap(max, size, type);
        }
    }
}

void HeapUtil::GetHeap(vector<Article *> &article) const { article = heap_;}

int HeapUtil::GetHeapSize() const {return heap_size_; }

void HeapUtil::SetHeapMaxSize(int max) {
  heap_size_ = 0;
  heap_.reserve((unsigned long)max);
  heap_max_ = max;
}
