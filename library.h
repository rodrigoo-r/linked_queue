/*
 * This code is distributed under the terms of the GNU General Public License.
 * For more information, please refer to the LICENSE file in the root directory.
 * -------------------------------------------------
 * Copyright (C) 2025 Rodrigo R.
 * This program comes with ABSOLUTELY NO WARRANTY; for details type show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type show c' for details.
*/

#ifndef FLUENT_LIBC_LINKED_QUEUE_LIBRARY_H
#define FLUENT_LIBC_LINKED_QUEUE_LIBRARY_H

// ============= FLUENT LIB C =============

// ============= INCLUDES =============
#ifndef FLUENT_LIBC_RELEASE
#    include <types.h>
#else
#   include <fluent/types/types.h>
#endif
#include <stdlib.h>

/**
 * @brief Structure representing a linked queue.
 *
 * This structure is used to implement a linked queue, where each node contains
 * a pointer to the data, a pointer to the next node, a pointer to the tail node,
 * and the size of the data stored.
 *
 * Members:
 *   void *data
 *     Pointer to the data stored in the node.
 *   struct linked_queue_t *next
 *     Pointer to the next node in the queue.
 *   struct linked_queue_t *tail
 *     Pointer to the tail node in the queue.
 *   size_t size
 *     Size of the data stored in the node.
 */
typedef struct linked_queue_t
{
    void *data; // Pointer to the data stored in the node
    struct linked_queue_t *next; // Pointer to the next node in the queue
    struct linked_queue_t *tail; // Pointer to the tail node in the queue
    size_t size; // Size of the data stored in the node
} linked_queue_t;

/**
 * @brief Initializes a linked queue node.
 *
 * Sets all members of the given linked queue node to their default values:
 * - data: NULL
 * - next: NULL
 * - tail: NULL
 * - size: 0
 *
 * @param head Pointer to the linked queue node to initialize.
 */
static inline void linked_queue_init(linked_queue_t *head)
{
    head->data = NULL; // Initialize data pointer to NULL
    head->next = NULL; // Initialize next pointer to NULL
    head->tail = NULL; // Initialize tail pointer to NULL
    head->size = 0; // Initialize size to 0
}

#endif //FLUENT_LIBC_LINKED_QUEUE_LIBRARY_H