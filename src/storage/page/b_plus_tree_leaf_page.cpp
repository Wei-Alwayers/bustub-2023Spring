//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/page/b_plus_tree_leaf_page.cpp
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <sstream>

#include "common/exception.h"
#include "common/rid.h"
#include "storage/page/b_plus_tree_leaf_page.h"

namespace bustub {

/*****************************************************************************
 * HELPER METHODS AND UTILITIES
 *****************************************************************************/

/**
 * Init method after creating a new leaf page
 * Including set page type, set current size to zero, set next page id and set max size
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::Init(int max_size) {
    SetPageType(IndexPageType::LEAF_PAGE);
    SetSize(0);
    SetMaxSize(max_size);
}

/**
 * Helper methods to set/get next page id
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::GetNextPageId() const -> page_id_t { return next_page_id_; }

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::SetNextPageId(page_id_t next_page_id) {
  next_page_id_ = next_page_id;
}

/*
 * Helper method to find and return the key associated with input "index"(a.k.a
 * array offset)
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::KeyAt(int index) const -> KeyType {
  return array_[index].first;
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::Add(const KeyType &key, const ValueType &value, const KeyComparator &comparator){
  int size = GetSize();
  array_[size].first = key;
  array_[size].second = value;
  size++;
  SetSize(size);
  // 使用Lambda表达式指定比较方式，比较数组的 first 元素
  std::sort(array_, array_ + size, [&comparator](const auto &a, const auto &b) {
    return comparator(a.first, b.first);
  });

  //检查是否已满
  if(size == GetMaxSize()){
    // TODO: split
  }
}

INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::LeafFind(const KeyType &key, const KeyComparator &comparator,ValueType *value) const -> bool {
  // 二分查找
  int low = 0;
  int high = GetSize() - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;
    if (comparator(array_[mid].first, key)) {
      low = mid + 1;
    } else if (comparator(key, array_[mid].first)) {
      high = mid - 1;
    } else {
      *value = array_[mid].second; // 如果找到key，返回对应的second元素
      return true;
    }
  }
  return false;  // 如果未找到key，返回指定的字符串
}

template class BPlusTreeLeafPage<GenericKey<4>, RID, GenericComparator<4>>;
template class BPlusTreeLeafPage<GenericKey<8>, RID, GenericComparator<8>>;
template class BPlusTreeLeafPage<GenericKey<16>, RID, GenericComparator<16>>;
template class BPlusTreeLeafPage<GenericKey<32>, RID, GenericComparator<32>>;
template class BPlusTreeLeafPage<GenericKey<64>, RID, GenericComparator<64>>;
}  // namespace bustub
