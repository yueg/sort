//
// Created by yueg on 9/18/15.
//

/**
 * 使用堆的思想，获得热度最大或者最小的n个文章的id
 */

#ifndef SORTNEWS_HEAPUTIL_H
#define SORTNEWS_HEAPUTIL_H
#include <map>
#include "article.h"


class HeapUtil {
 public:
  HeapUtil(int size);

  // 插入元素到堆
  bool InsertIntoHeap(const Article *article, bool type);

  // 根据this->dataMap建堆
  void BuildHeap(const std::vector<Article *> articleVec,int n, bool type);

  // 类似堆排序中调整堆的算法
  void AdjustHeap(int i, int size, bool type);

  // 获取堆指针
  void GetHeap(std::vector<Article *> &article) const;

  // 获取堆大小
  int GetHeapSize() const;

  // 设置heap大小
  void SetHeapMaxSize(int size);

 private:
  std::vector<Article *> heap_;                        // 堆
  int heap_size_;                                       // 堆大小
  int heap_max_;
};


#endif //SORTNEWS_HEAPUTIL_H
