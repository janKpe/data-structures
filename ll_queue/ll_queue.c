#include "ll_queue.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h>

ll_queue* ll_queue_init(size_t item_size) {
  ll_queue* q = malloc(sizeof(ll_queue));

  RETURN_NULL_IF_NULL(q);

  q->size = 0;
  q->item_size = item_size;
  q->head = NULL;
  q->tail = NULL;
  return q;
}

void ll_queue_enqueue(ll_queue* const q, const void* item) {
  RETURN_IF_NULL(q, item);

  ll_queue_node* node = malloc(sizeof(ll_queue_node));
  RETURN_IF_NULL(node);

  node->data = malloc(q->item_size);
  if (node->data == NULL) {
    free(node);
    return;
  }

  memcpy(node->data, item, q->item_size);
  node->next = NULL;

  if (q->tail == NULL) {
    // Queue was empty — head and tail both point to the new node.
    q->head = node;
    q->tail = node;
  } else {
    q->tail->next = node;
    q->tail = node;
  }

  q->size++;
}

void ll_queue_dequeue(ll_queue* const q) {
  RETURN_IF_NULL(q);

  if (q->head == NULL) {
    return;
  }

  ll_queue_node* old_head = q->head;
  q->head = old_head->next;

  if (q->head == NULL) {
    // Queue is now empty — clear the tail pointer too.
    q->tail = NULL;
  }

  free(old_head->data);
  free(old_head);
  q->size--;
}

void* ll_queue_peek(ll_queue* const q) {
  RETURN_NULL_IF_NULL(q);

  if (q->head == NULL) {
    return NULL;
  }

  return q->head->data;
}

void* ll_queue_peek_copy(ll_queue* const q) {
  RETURN_NULL_IF_NULL(q);

  if (q->head == NULL) {
    return NULL;
  }

  void* copy = malloc(q->item_size);
  if (copy == NULL) {
    return NULL;
  }

  memcpy(copy, q->head->data, q->item_size);
  return copy;
}

bool ll_queue_is_empty(const ll_queue* const q) {
  if (q == NULL) {
    return true;
  }
  return q->size == 0;
}

void ll_queue_free(ll_queue* const q) {
  RETURN_IF_NULL(q);

  ll_queue_node* current = q->head;
  while (current != NULL) {
    ll_queue_node* next = current->next;
    free(current->data);
    free(current);
    current = next;
  }

  free(q);
}
