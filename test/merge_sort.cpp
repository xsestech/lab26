//
// Created by Alex on 02.06.2024.
//
#include "gtest/gtest.h"

extern "C" {
#include "list.h"
}
int compare (const void *a, const void *b)
{
  const int *ia = (const int *)a; // casting pointer types
  const int *ib = (const int *)b;
  return *ia  - *ib;
}

TEST(merge, testMergeBig) {
  list_handle_t list = list_init();
  int data[] = {2, 5, 7, 1, 3, 5};
  for (int i = 0; i < sizeof(data)/sizeof(int); i++) {
    list_push_back(list, data[i]);
  }
  list_handle_t list1 = NULL;
  list_handle_t list2 = NULL;
  list_split(list, &list1, &list2);
  list = list_merge_one_container(list1, list2);
  int i = 0;
  qsort(data, sizeof(data)/sizeof(int), sizeof(int), compare);
  for (list_iter_t iter = list_begin(list); !list_iter_equal(iter, list_iter_null(list)); iter = list_iter_next(iter)) {
    printf("%d ", (int)list_iter_val(iter));
    ASSERT_EQ(list_iter_val(iter), data[i]);
    i++;
  }
  list_destroy(list);
}
TEST(merge, testMergeSmall) {
  list_handle_t list = list_init();
  int data[] = {2, 1};
  for (int i = 0; i < sizeof(data)/sizeof(int); i++) {
    list_push_back(list, data[i]);
  }
  list_handle_t list1 = NULL;
  list_handle_t list2 = NULL;
  list_split(list, &list1, &list2);
  list = list_merge_one_container(list1, list2);
  int i = 0;
  qsort(data, sizeof(data)/sizeof(int), sizeof(int), compare);
  for (list_iter_t iter = list_begin(list); !list_iter_equal(iter, list_iter_null(list)); iter = list_iter_next(iter)) {
    printf("%d ", (int)list_iter_val(iter));
    ASSERT_EQ(list_iter_val(iter), data[i]);
    i++;
  }
  list_destroy(list);
}
TEST(merge, testMergeMedium) {
  list_handle_t list = list_init();
  int data[] = {2, 1, 3};
  for (int i = 0; i < sizeof(data)/sizeof(int); i++) {
    list_push_back(list, data[i]);
  }
  list_handle_t list1 = NULL;
  list_handle_t list2 = NULL;
  list_split(list, &list1, &list2);
  list = list_merge_one_container(list1, list2);
  int i = 0;
  qsort(data, sizeof(data)/sizeof(int), sizeof(int), compare);
  for (list_iter_t iter = list_begin(list); !list_iter_equal(iter, list_iter_null(list)); iter = list_iter_next(iter)) {
    printf("%d ", (int)list_iter_val(iter));
    ASSERT_EQ(list_iter_val(iter), data[i]);
    i++;
  }
  list_destroy(list);
}
TEST(merge, testMergeOne) {
  list_handle_t list = list_init();
  int data[] = {5};
  for (int i = 0; i < sizeof(data)/sizeof(int); i++) {
    list_push_back(list, data[i]);
  }
  list_handle_t list1 = NULL;
  list_handle_t list2 = NULL;
  list_split(list, &list1, &list2);
  list = list_merge_one_container(list1, list2);
  int i = 0;
  qsort(data, sizeof(data)/sizeof(int), sizeof(int), compare);
  for (list_iter_t iter = list_begin(list); !list_iter_equal(iter, list_iter_null(list)); iter = list_iter_next(iter)) {
    printf("%d ", (int)list_iter_val(iter));
    ASSERT_EQ(list_iter_val(iter), data[i]);
    i++;
  }
  list_destroy(list);
}
TEST(merge, testMergeSort) {
  list_handle_t list = list_init();
  int data[] = {2, 1, 3, 5, 7, 15, 10};
  for (int i = 0; i < sizeof(data)/sizeof(int); i++) {
    list_push_back(list, data[i]);
  }
  list = list_merge_sort(list);
  int i = 0;
  qsort(data, sizeof(data)/sizeof(int), sizeof(int), compare);
  for (list_iter_t iter = list_begin(list); !list_iter_equal(iter, list_iter_null(list)); iter = list_iter_next(iter)) {
    printf("%d ", (int)list_iter_val(iter));
    ASSERT_EQ(list_iter_val(iter), data[i]);
    i++;
  }
  list_destroy(list);
}