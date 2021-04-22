/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "tiny_list.h"
#include "tiny_utils.h"

extern inline void tiny_list_init(tiny_list_t* self);

uint16_t tiny_list_count(tiny_list_t* self)
{
  tiny_list_node_t* current = self->head.next;
  uint16_t count = 0;

  while(current != &self->head) {
    count++;
    current = current->next;
  }

  return count;
}

extern inline void tiny_list_push_front(tiny_list_t* self, tiny_list_node_t* node);

void tiny_list_push_back(tiny_list_t* self, tiny_list_node_t* node)
{
  tiny_list_node_t* current = self->head.next;

  while(current->next != &self->head) {
    current = current->next;
  }

  current->next = node;
  node->next = &self->head;
}

extern inline tiny_list_node_t* tiny_list_pop_front(tiny_list_t* self);

tiny_list_node_t* tiny_list_pop_back(tiny_list_t* self)
{
  tiny_list_node_t* previous = self->head.next;
  tiny_list_node_t* current = self->head.next;

  while(current->next != &self->head) {
    previous = current;
    current = current->next;
  }

  previous->next = &self->head;
  return current;
}

void tiny_list_remove(tiny_list_t* self, tiny_list_node_t* node)
{
  tiny_list_node_t* previous = &self->head;
  tiny_list_node_t* current = self->head.next;

  while(current != &self->head) {
    if(current == node) {
      previous->next = current->next;
      return;
    }

    previous = current;
    current = current->next;
  }
}

bool tiny_list_contains(tiny_list_t* self, tiny_list_node_t* node)
{
  tiny_list_node_t* current = self->head.next;

  while(current != &self->head) {
    if(current == node) {
      return true;
    }

    current = current->next;
  }

  return false;
}

uint16_t tiny_list_index_of(tiny_list_t* self, tiny_list_node_t* node)
{
  tiny_list_node_t* current = self->head.next;
  uint16_t index = 0;

  while(current != node) {
    index++;
    current = current->next;
  }

  return index;
}

extern inline void tiny_list_iterator_init(tiny_list_iterator_t* self, tiny_list_t* list);

extern inline tiny_list_node_t* tiny_list_iterator_next(tiny_list_iterator_t* self, tiny_list_t* list);
