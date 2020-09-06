/*!
 * @file
 * @brief
 */

#ifndef tiny_list_iterator_h
#define tiny_list_iterator_h

#include "tiny_list.h"
#include "tiny_utils.h"

typedef struct
{
  tiny_list_t* list;
  tiny_list_node_t* current;
} tiny_list_iterator_t;

/*!
 * Initialize an iterator for the provided list.
 */
void tiny_list_iterator_init(tiny_list_iterator_t* self, tiny_list_t* list);

/*!
 * Return a pointer to the next node or NULL if there are no more nodes.
 */
tiny_list_node_t* tiny_list_iterator_next(tiny_list_iterator_t* self);

#define tiny_list_iterate(_list, _type, _item, ...)          \
  do {                                                       \
    tiny_list_iterator_t _it;                                \
    tiny_list_iterator_init(&_it, _list);                    \
    _type* _item;                                            \
    while((_item = (_type*)tiny_list_iterator_next(&_it))) { \
      __VA_ARGS__                                            \
    }                                                        \
  } while(0)

#endif
