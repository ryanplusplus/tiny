/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "tiny_list.h"
#include "tiny_utils.h"

typedef struct {
  tiny_list_node_t* previous;
  tiny_list_node_t* current;
  tiny_list_node_t* target;
  uint16_t index;
} info_t;

static bool for_each_info(tiny_list_node_t* node, uint16_t index, void* context) {
  reinterpret(find, context, info_t*);
  find->index = index;
  find->previous = find->current;
  find->current = node;
  return !(node == find->target);
}

static void find_node_info(tiny_list_t* self, tiny_list_node_t* node, info_t* info) {
  info->previous = NULL;
  info->current = NULL;
  info->target = node;
  info->index = 0;
  tiny_list_for_each(self, for_each_info, info);
}

void tiny_list_init(tiny_list_t* self) {
  self->head = NULL;
}

uint16_t tiny_list_count(tiny_list_t* self) {
  if(self->head == NULL) {
    return 0;
  }
  else {
    info_t info;
    find_node_info(self, NULL, &info);
    return info.index + 1;
  }
}

void tiny_list_push_front(tiny_list_t* self, tiny_list_node_t* node) {
  node->next = self->head;
  self->head = node;
}

void tiny_list_push_back(tiny_list_t* self, tiny_list_node_t* node) {
  info_t info;
  find_node_info(self, NULL, &info);

  if(info.current == NULL) {
    self->head = node;
  }
  else {
    info.current->next = node;
  }

  node->next = NULL;
}

tiny_list_node_t* tiny_list_pop_front(tiny_list_t* self) {
  tiny_list_node_t* popped = self->head;
  self->head = self->head->next;
  return popped;
}

tiny_list_node_t* tiny_list_pop_back(tiny_list_t* self) {
  info_t info;
  find_node_info(self, NULL, &info);

  if(info.previous == NULL) {
    self->head = NULL;
  }
  else {
    info.previous->next = info.current->next;
  }

  return info.current;
}

void tiny_list_remove(tiny_list_t* self, tiny_list_node_t* node) {
  if(self->head == node) {
    self->head = node->next;
  }
  else {
    info_t info;
    find_node_info(self, node, &info);

    if(info.current == node) {
      info.previous->next = node->next;
    }
  }
}

bool tiny_list_contains(tiny_list_t* self, tiny_list_node_t* node) {
  info_t info;
  find_node_info(self, node, &info);
  return info.current == node;
}

uint16_t tiny_list_index_of(tiny_list_t* self, tiny_list_node_t* node) {
  info_t info;
  find_node_info(self, node, &info);
  return info.index;
}

void tiny_list_for_each(tiny_list_t* self, tiny_list_for_each_t callback, void* context) {
  tiny_list_node_t* current = self->head;
  uint16_t index = 0;

  while(current) {
    if(!callback(current, index, context)) {
      return;
    }

    current = current->next;
    index++;
  }
}
