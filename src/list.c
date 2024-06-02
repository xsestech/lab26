//
// Created by Alex on 03.05.2024.
//
#include "list.h"

#include <math.h>

struct list_element_t {
  double value;
  int prev;
  int next;
};
/**
 * In this realiztion we assume, that top_element_empty always exits
 */
struct list_t {
  int first;
  int last;
  size_t capacity;
  size_t size;
  int top_element_empty;
  list_element_t *container;
};

/**
 * Logariphmic list growth
 * @param old_capacity capacity, that needs to be increased
 * @return new capacity
 * @internal
 */
size_t list_new_capacity(size_t old_capacity) {
  if (!old_capacity) {
    return 2;
  }
  return old_capacity << 1;
}
/**
 * Compute new size for realloc
 * @param list poitner to list
 * @param new_capacity new capacity of list
 * @return amount of memory to realloc
 * @internal
 */
size_t list_compute_realloc_size_for_capacity(list_handle_t list, size_t new_capacity) {
  return new_capacity * sizeof(*(list->container));
}

/**
 * Grows list capacity to specified capacity
 * Won't do grow if new_capacity if less or equal to current capacuty
 * @param list list pointer
 * @param new_capacity capacity to grow
 * @internal
 */
void list_grow_capacity(list_handle_t list, size_t new_capacity) {
  size_t old_capacity = list->capacity;
  if (new_capacity <= old_capacity) return;
  size_t new_data_size = list_compute_realloc_size_for_capacity(list, new_capacity);
  if (list->container) {
    list->container = realloc(list->container, new_data_size);
  } else {
    list->container = malloc(new_data_size);
  }
  assert(list->container);
  for (int i = old_capacity; i < new_capacity - 1; i++) {
    list->container[i].next = i + 1;
  }
  list->container[new_capacity - 1].next = -1;
  if (old_capacity != 0) {
    list->container[list->top_element_empty].next = (int) old_capacity;
  }
  list->capacity = new_capacity;
}

/**
 * Increases list capacity according to list_new_capacity function
 * @param list pointer to list
 * @internal
 */
void list_grow(list_handle_t list) {
  size_t old_capacity = list->capacity;
  size_t new_capacity = list_new_capacity(old_capacity);
  list_grow_capacity(list, new_capacity);
}
/**
 * Decreases list capacity, when it is half empty
 * @param list list_ptr
 * @internal
 */
void list_shrink(list_handle_t list) {
  if (list->size * 2 <= list->capacity && list->size != 0) {
    size_t new_data_size = list_compute_realloc_size_for_capacity(list, list->size);
    list_element_t *new_container = malloc(new_data_size);
    assert(new_container);
    int i = 0;
    for (list_iter_t iter = list_begin(list); !list_iter_equal(iter, list_iter_null(list)); iter =
         list_iter_next(iter)) {
      list_element_t old_element = list->container[iter.list_element_idx];
      if (old_element.next != -1) {
        old_element.next = i + 1;
      }
      if (old_element.prev != -1) {
        old_element.prev = i - 1;
      }
      new_container[i] = old_element;
      i++;
    }
    free(list->container);
    list->container = new_container;
    list->capacity = list->size;
    list->first = 0;
    list->last = --i;
  }
}

list_handle_t list_malloc_empty_handle() {
  list_handle_t list = malloc(sizeof(list_t));
  list->capacity = 0;
  list->size = 0;
  list->first = -1;
  list->last = -1;
  list->top_element_empty = 0;
  list->container = NULL;
  return list;
}

list_handle_t list_init() {
  list_handle_t list = list_malloc_empty_handle();
  list_grow(list);
  return list;
}
list_handle_t list_init_size(size_t size) {
  list_handle_t list = list_malloc_empty_handle();
  list_grow_capacity(list, size);
  return list;
}

void list_destroy(list_handle_t list) {
  free(list->container);
  free(list);
}

list_iter_t list_begin(list_handle_t list) {
  return (list_iter_t){list, list->first};
}

list_iter_t list_end(list_handle_t list) {
  return (list_iter_t){list, list->last};
}

double list_iter_val(list_iter_t iter) {
  return iter.list->container[iter.list_element_idx].value;
}

list_iter_t list_iter_next(list_iter_t iter) {
  return (list_iter_t){iter.list, iter.list->container[iter.list_element_idx].next};
}
list_iter_t list_iter_prev(list_iter_t iter) {
  return (list_iter_t){iter.list, iter.list->container[iter.list_element_idx].prev};
}

list_iter_t list_iter_null(list_handle_t list) {
  return (list_iter_t){list, -1};
}

bool list_iter_equal(list_iter_t iter1, list_iter_t iter2) {
  if (iter1.list != iter2.list) {
    return false;
  }
  if (iter1.list_element_idx != iter2.list_element_idx) {
    return false;
  }
  return true;
}
void list_insert_before(list_iter_t iter, double val) {
  list_handle_t list = iter.list;
  if (list->container[list->top_element_empty].next == -1) {
    list_grow(iter.list);
  }
  if (list_iter_equal(iter, list_iter_null(list)) &&
    list->top_element_empty == 0) {
    list->top_element_empty = list->container[0].next;
    list->container[0] = (list_element_t){val, -1, -1};
    list->first = 0;
    list->last = 0;
    list->size++;
    return;
  }
  if (list->container[iter.list_element_idx].prev == -1) {
    list->first = list->top_element_empty;
  }
  int new_top_elem_empty = list->container[list->top_element_empty].next;
  list->container[list->top_element_empty] = (list_element_t){
    val, list->container[iter.list_element_idx].prev, iter.list_element_idx
  };
  list->container[iter.list_element_idx].prev = list->top_element_empty;
  list->top_element_empty = new_top_elem_empty;
  list->size++;
}

void list_insert_after(list_iter_t iter, double val) {
  list_handle_t list = iter.list;
  if (list->container[list->top_element_empty].next == -1) {
    list_grow(iter.list);
  }
  if (list_iter_equal(iter, list_iter_null(list)) &&
    list->top_element_empty == 0) {
    list->top_element_empty = list->container[0].next;
    list->container[0] = (list_element_t){val, -1, -1};
    list->first = 0;
    list->last = 0;
    list->size++;
    return;
  }
  if (list->container[iter.list_element_idx].next == -1) {
    list->last = list->top_element_empty;
  }
  int new_top_elem_empty = list->container[list->top_element_empty].next;
  list->container[list->top_element_empty] = (list_element_t){
    val, iter.list_element_idx, list->container[iter.list_element_idx].next
  };
  list->container[iter.list_element_idx].next = list->top_element_empty;
  list->top_element_empty = new_top_elem_empty;
  list->size++;
}

double list_remove(list_iter_t iter) {
  list_handle_t list = iter.list;

  if (list_iter_equal(iter, list_iter_null(list))) {
    return NAN;
  }
  list_element_t elem = list->container[iter.list_element_idx];
  if (elem.next == -1) {
    list->last = elem.prev;
  }
  if (elem.prev != -1) {
    list->container[elem.prev].next = elem.next;
  }
  list->container[iter.list_element_idx].next = list->top_element_empty;
  list->top_element_empty = iter.list_element_idx;
  list->size--;
  list_shrink(list);
  return elem.value;
}
void list_push_back(list_handle_t list, double val) {
  list_insert_after(list_end(list), val);
}
void list_push_front(list_handle_t list, double val) {
  list_insert_before(list_end(list), val);
}

bool list_is_empty(list_handle_t list) { return list->size == 0; }
void list_reverse(list_handle_t list) {
  int current_idx = list->first;
  while (current_idx != -1) {
    int tmp_idx = list->container[current_idx].next;
    list->container[current_idx].next = list->container[current_idx].prev;
    list->container[current_idx].prev = tmp_idx;
    current_idx = tmp_idx;
  }
  int tmp_first = list->first;
  list->first = list->last;
  list->last = tmp_first;
}
list_handle_t list_copy(list_handle_t list) {
  list_handle_t new_list = list_init_size(list->capacity);
  memcpy(new_list->container,
         new_list->container,
         list_compute_realloc_size_for_capacity(new_list, new_list->capacity));
  return new_list;
}

list_handle_t list_merge(list_handle_t list1, list_handle_t list2) {
  if (list1->size == 0) {
    return list_copy(list2);
  }
  if (list2->size == 0) {
    return list_copy(list1);
  }
  list_handle_t merged_list = list_init_size(list1->size + list2->size + 1);
  list_iter_t list1_iter = list_begin(list1);
  list_iter_t list2_iter = list_begin(list2);
  while (!list_iter_equal(list1_iter, list_end(list1)) || !list_iter_equal(list2_iter, list_end(list2))) {
    if (list_iter_val(list1_iter) > list_iter_val(list2_iter)) {
      list_push_back(merged_list, list_iter_val(list2_iter));
      list2_iter = list_iter_next(list2_iter);
    } else {
      list_push_back(merged_list, list_iter_val(list1_iter));
      list1_iter = list_iter_next(list1_iter);
    }
  }
  while (!list_iter_equal(list1_iter, list_end(list1))) {
    list_push_back(merged_list, list_iter_val(list1_iter));
    list1_iter = list_iter_next(list1_iter);
  }
  while (!list_iter_equal(list2_iter, list_end(list2))) {
    list_push_back(merged_list, list_iter_val(list2_iter));
    list2_iter = list_iter_next(list2_iter);
  }
  return merged_list;
}

void list_split(list_handle_t list, list_handle_t *list1, list_handle_t *list2) {

  *list1 = malloc(sizeof(list_t));
  *list2 = malloc(sizeof(list_t));
  **list1 = *list;
  **list2 = *list;
  if (list->size == 1) {
    (*list2)->first = -1;
    (*list2)->last = -1;
    (*list2)->size = 0;
    return;
  }
  int first_idx = list->first;
  int second_idx = list->first;
  int size = 0;
  while ((second_idx != -1) && (list->container[second_idx].next != -1)) {
    size++;
    first_idx = list->container[first_idx].next;
    second_idx = list->container[list->container[second_idx].next].next;
  }
  first_idx = list->container[first_idx].prev;
  (*list1)->last = first_idx;
  (*list1)->size = size;
  (*list2)->first = list->container[first_idx].next;
  (*list2)->size -= size;
  list->container[first_idx].next = -1;
  list->container[(*list2)->first].prev = -1;
  free(list);
}

list_handle_t list_merge_one_container(list_handle_t list1, list_handle_t list2) {
  if (list1->size == 0) {
    free(list1);
    return list2;
  }
  if (list2->size == 0) {
    free(list2);
    return list1;
  }
  if (list_iter_val(list_begin(list1)) > list_iter_val(list_begin(list2))) {
    list_handle_t tmp = list1;
    list1 = list2;
    list2 = tmp;
  }
  list_iter_t iter1 = list_begin(list1);
  list_iter_t iter2 = list_begin(list2);
  while (!list_iter_equal(iter1, list_iter_null(list1)) && !list_iter_equal(iter2, list_iter_null(list2))) {
    if (list_iter_val(iter1) > list_iter_val(iter2)) {
      list_iter_t tmp = list_iter_next(iter2);
      if (list1->container[iter1.list_element_idx].prev == -1) {
        list1->first = iter2.list_element_idx;
      }
      list1->container[list1->container[iter1.list_element_idx].prev].next = iter2.list_element_idx;
      list2->container[iter2.list_element_idx].prev = list1->container[iter1.list_element_idx].prev;
      list2->container[iter2.list_element_idx].next = iter1.list_element_idx;
      list1->container[iter1.list_element_idx].prev = iter2.list_element_idx;
      iter2 = tmp;
    } else {
      iter1 = list_iter_next(iter1);
    }
  }
  if (!list_iter_equal(iter2, list_iter_null(list2))) {
    list_iter_t end_iter = iter2;
    int i = 0;
    while (!list_iter_equal(end_iter, list_end(list2))) {
      end_iter = list_iter_next(end_iter);
      i++;
    }
    list1->container[list1->last].next = iter2.list_element_idx;
    list1->container[iter2.list_element_idx].prev = list1->last;

    list1->last = end_iter.list_element_idx;
  }
  list1->size += list2->size;
  free(list2);
  return list1;
}

list_handle_t list_merge_sort(list_handle_t list) {
  if (list->size <= 1) return list;
  list_handle_t list1 = NULL;
  list_handle_t list2 = NULL;
  list_split(list, &list1, &list2);
  return list_merge_one_container(list_merge_sort(list1), list_merge_sort(list2));
}
