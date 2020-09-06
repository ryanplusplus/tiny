/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "tiny_list_iterator.h"

void tiny_list_iterator_init(tiny_list_iterator_t* self, tiny_list_t* list)
{
  self->list = list;
  self->current = list->head.next;
}

tiny_list_node_t* tiny_list_iterator_next(tiny_list_iterator_t* self)
{
  if(self->current == &self->list->head) {
    return NULL;
  }
  else {
    tiny_list_node_t* item = self->current;
    self->current = self->current->next;
    return item;
  }
}
