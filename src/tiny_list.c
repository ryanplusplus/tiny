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
  info->current = &self->head;
  info->target = node;
  info->index = 0;
  tiny_list_for_each(self, for_each_info, info);
}

void tiny_list_init(tiny_list_t* self) {
  self->head.next = &self->head;
}

uint16_t tiny_list_count(tiny_list_t* self) {
  tiny_list_node_t* current = self->head.next;
  uint16_t count = 0;

  while(current != &self->head) {
    count++;
    current = current->next;
  }

  return count;
}

void tiny_list_push_front(tiny_list_t* self, tiny_list_node_t* node) {
  node->next = self->head.next;
  self->head.next = node;
}

void tiny_list_push_back(tiny_list_t* self, tiny_list_node_t* node) {
  info_t info;
  find_node_info(self, &self->head, &info);
  info.current->next = node;
  node->next = &self->head;
}

tiny_list_node_t* tiny_list_pop_front(tiny_list_t* self) {
  tiny_list_node_t* popped = self->head.next;
  self->head.next = self->head.next->next;
  return popped;
}

tiny_list_node_t* tiny_list_pop_back(tiny_list_t* self) {
  info_t info;
  find_node_info(self, &self->head, &info);
  info.previous->next = info.current->next;
  return info.current;
}

void tiny_list_remove(tiny_list_t* self, tiny_list_node_t* node) {
  info_t info;
  find_node_info(self, node, &info);

  if(info.current == node) {
    info.previous->next = node->next;
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
  tiny_list_node_t* current = self->head.next;
  uint16_t index = 0;

  while(current != &self->head) {
    if(!callback(current, index, context)) {
      return;
    }

    current = current->next;
    index++;
  }
}
