/*!
 * @file
 * @brief Linked list that contains nodes allocated by clients.
 *
 * Nodes can contain arbitrary data by defining a type that contains
 * a tiny_list_node_t:
 *
 * typedef struct client_node_t {
 *   tiny_list_node_t node;
 *   int data;
 * }
 *
 * This type must be cast to a tiny_list_node_t to be added but can
 * be cast back by the client so that the data can be accessed.
 */

#ifndef tiny_list_h
#define tiny_list_h

#include <stdbool.h>
#include <stdint.h>
#include "tiny_utils.h"

typedef struct tiny_list_node_t {
  struct tiny_list_node_t* next;
} tiny_list_node_t;

typedef struct {
  tiny_list_node_t head;
} tiny_list_t;

typedef struct
{
  tiny_list_t* list;
  tiny_list_node_t* current;
} tiny_list_iterator_t;

/*!
 * Initializes the list.
 */
void tiny_list_init(tiny_list_t* self);

/*!
 * Adds the node to the front of the list.
 */
void tiny_list_push_front(tiny_list_t* self, tiny_list_node_t* node);

/*!
 * Adds the node to the back of the list.
 */
void tiny_list_push_back(tiny_list_t* self, tiny_list_node_t* node);

/*!
 * Removes the node from the front of the list. Returns the node.
 */
tiny_list_node_t* tiny_list_pop_front(tiny_list_t* self);

/*!
 * Removes the node at the back of the list. Returns the node.
 */
tiny_list_node_t* tiny_list_pop_back(tiny_list_t* self);

/*!
 * Removes a specified node if present in the list.
 */
void tiny_list_remove(tiny_list_t* self, tiny_list_node_t* node);

/*!
 * Returns the number of nodes contained in the list.
 */
uint16_t tiny_list_count(tiny_list_t* self);

/*!
 * Returns true if the specified node is in the list and false otherwise.
 */
bool tiny_list_contains(tiny_list_t* self, tiny_list_node_t* node);

/*!
 * Gives the index of a given node in the list.
 */
uint16_t tiny_list_index_of(tiny_list_t* self, tiny_list_node_t* node);

/*!
 * Initialize an iterator for the provided list.
 */
void tiny_list_iterator_init(tiny_list_iterator_t* self, tiny_list_t* list);

/*!
 * Return a pointer to the next node or NULL if there are no more nodes.
 */
tiny_list_node_t* tiny_list_iterator_next(tiny_list_iterator_t* self);

#define tiny_list_for_each(_list, _type, _item, ...)         \
  do {                                                       \
    tiny_list_iterator_t _it;                                \
    tiny_list_iterator_init(&_it, _list);                    \
    _type* _item;                                            \
    while((_item = (_type*)tiny_list_iterator_next(&_it))) { \
      __VA_ARGS__                                            \
    }                                                        \
  } while(0)

#endif
