/*
 * Main file for CS 208 assignment: Queues in C
 * Implemented from a lab developed at CMU by R. E. Bryant, 2017-2018
 * in association with harness.h and queue.h.
 * 
 * Code written by Cole Weinstein.
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    if (q == NULL) {
      return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL) {
      return;
    }
    if (q->head != NULL) {
      list_ele_t *curr = q->head;
      list_ele_t *next_node;
      while (curr->next != NULL) {
        next_node = curr->next;
        free(curr->value);
        free(curr);
        curr = next_node;
      }
      free(curr->value);
      free(curr);
    }

    free(q);
}

/*
  Create a new list_ele_t element and initialize its value to a given string.
  Return the new list_ele_t if successful.
  Return NULL if malloc fails (ie. could not allocate enough space).
*/
list_ele_t *q_new_node(char *s) {
    list_ele_t *node;
    char *value;

    if (s == NULL) {
      return NULL;
    }

    node = malloc(sizeof(list_ele_t));
    if (node == NULL) {
      return NULL;
    }

    value = malloc(strlen(s)+1);
    if (value == NULL) {
      free(node);
      return NULL;
    }

    // copies all of s (including \0 char) into value
    strcpy(value, s);

    node->value = value;
    node->next = NULL;
    return node;
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *new_node;
    
    if (q == NULL) {
      return false;
    }

    new_node = q_new_node(s);
    if (new_node == NULL) {
      return false;
    }
    
    // Connect the new node to the front of the queue.
    new_node->next = q->head;
    q->head = new_node;
    q->size++;
    if (q->size == 1) {
      q->tail = new_node;
    }

    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    // TODO implement in similar fashion to q_insert_head
    // You'll certainly want to add a field to queue_t so we can access
    // the tail efficiently.
    list_ele_t *new_node;

    if (q == NULL) {
      return false;
    }

    new_node = q_new_node(s);
    if (new_node == NULL) {
      return false;
    }

    if (q->tail != NULL) {
      q->tail->next = new_node;
    }
    q->tail = new_node;
    q->size++;
    if (q->size == 1) {
      q->head = new_node;
    }

    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL) {
      return false;
    }
    
    if (q->head == NULL) {
      return false;
    }
    if (sp != NULL && bufsize != 0) {
      // copy the string to be removed into sp, then terminate sp with a null character (in case strlen(q->head->value) > bufsize)
      strncpy(sp, q->head->value, bufsize);
      sp[bufsize-1] = '\0';
    }

    free(q->head->value);
    list_ele_t *next_node = q->head->next;
    free(q->head);
    q->head = next_node;
    q->size--;
    if (q->size == 0) {
      q->tail = NULL;
    }

    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL) {
      return 0;
    }
    
    return q->size; // size is 0 if q is empty
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL) {
      return;
    }

    // if q is empty or only has one element, this function will do nothing.
    if (q->head == NULL || q->head->next == NULL) {
      return;
    }

    list_ele_t *curr_node = q->head;
    list_ele_t *prev_node = NULL;
    list_ele_t *next_node = q->head->next;
    q->tail = q->head;
    while (next_node != NULL) {
      curr_node->next = prev_node;
      prev_node = curr_node;
      curr_node = next_node;
      next_node = curr_node->next;
    }
    curr_node->next = prev_node; // we break out of the while loop when curr_node is the tail, so we need to finish the last reversal
    q->head = curr_node;

    return;

}
