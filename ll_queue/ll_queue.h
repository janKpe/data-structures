#include <stdbool.h>
#include <stdlib.h>

#ifndef LL_QUEUE_H
#define LL_QUEUE_H

typedef struct ll_queue_node {
  void* data;
  struct ll_queue_node* next;
} ll_queue_node;

typedef struct {
  size_t size;
  size_t item_size;
  ll_queue_node* head; // front — dequeue side
  ll_queue_node* tail; // back  — enqueue side
} ll_queue;

// Create a new empty queue for items of the given size.
ll_queue* ll_queue_init(size_t item_size);

// Enqueue a copy of `item` at the back of the queue.
void ll_queue_enqueue(ll_queue* const q, const void* item);

// Remove the front item. Does nothing if the queue is empty.
void ll_queue_dequeue(ll_queue* const q);

// Return a pointer to the front item without removing it.
// Returns NULL if the queue is empty. Does NOT make a copy — use with caution.
void* ll_queue_peek(ll_queue* const q);

// Return a heap-allocated copy of the front item without removing it.
// Returns NULL if the queue is empty. Caller is responsible for freeing.
void* ll_queue_peek_copy(ll_queue* const q);

// Return true if the queue has no elements.
bool ll_queue_is_empty(const ll_queue* const q);

// Free all nodes and the queue itself.
void ll_queue_free(ll_queue* const q);

#endif
