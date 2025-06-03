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
// Linked Queue Utility
// ----------------------------------------
// Singly linked queue data structure for generic use.
//
// Provides:
//   - linked_queue_init(node)           – Initializes a queue node
//   - linked_queue_next(&head)          – Advances the head node, freeing the previous one
//   - linked_queue_append(head, data)   – Adds a new node to the tail
//   - linked_queue_prepend(&head, data) – Inserts a new node at the front
//   - linked_queue_free(head)           – Frees the entire queue
//
// Behavior:
//   - Each node contains a void* to arbitrary data, a next pointer, and metadata like size/tail
//   - Append maintains tail and size metadata
//   - Prepend does not modify tail/size – caller must manage these
//
// Memory Management:
//   - All node allocations use malloc()
//   - Caller must manually free data stored in each node (if needed)
//   - linked_queue_free() only frees node structures
//
// Dependencies:
//   - Fluent Lib C: <types.h>, <std_bool.h>
//   - Standard Lib: <stdlib.h>
//
// Example:
// ----------------------------------------
//   linked_queue_t head;
//   linked_queue_init(&head);
//
//   linked_queue_append(&head, strdup("Item 1"));
//   linked_queue_append(&head, strdup("Item 2"));
//
//   while (head.size > 0) {
//       printf("Data: %s\n", (char*)head.data);
//       linked_queue_next(&head);
//   }
//
//   linked_queue_free(head.next);  // If manually managing head + tail
//

// ============= INCLUDES =============
#ifndef FLUENT_LIBC_RELEASE
#    include <types.h>
#    include <std_bool.h>
#else
#   include <fluent/types/types.h>
#   include <fluent/std_bool/std_bool.h>
#endif
#include <stdlib.h>

// ============= TYPED LINKED QUEUE MACRO =============
#define DEFINE_LINKED_QUEUE(V, NAME)                              \
    typedef struct linked_##NAME##_queue_t                        \
    {                                                             \
        V data;                                                   \
        struct linked_##NAME##_queue_t *next;                     \
        struct linked_##NAME##_queue_t *tail;                     \
        size_t size;                                              \
    } linked_##NAME##_queue_t;                                    \
                                                                  \
    static inline void linked_##NAME##_queue_init(linked_##NAME##_queue_t *head) \
    {                                                             \
        head->data = (V){0};                                      \
        head->next = NULL;                                        \
        head->tail = NULL;                                        \
        head->size = 0;                                           \
    }                                                             \
                                                                  \
    static inline void linked_##NAME##_queue_next(linked_##NAME##_queue_t **head) \
    {                                                             \
        if (!head || !*head)                                      \
        {                                                         \
            return;                                               \
        }                                                         \
                                                                  \
        linked_##NAME##_queue_t *next_node = (*head)->next;       \
        next_node->size = (*head)->size - 1;                      \
        next_node->tail = (*head)->tail;                          \
                                                                  \
        free(*head);                                              \
        *head = next_node;                                        \
    }                                                             \
                                                                  \
    static inline bool linked_##NAME##_queue_append(linked_##NAME##_queue_t *head, V data) \
    {                                                             \
        if (!head)                                                \
        {                                                         \
            return FALSE;                                         \
        }                                                         \
                                                                  \
        linked_##NAME##_queue_t *new_node = malloc(sizeof(linked_##NAME##_queue_t)); \
        if (!new_node)                                            \
        {                                                         \
            return FALSE;                                         \
        }                                                         \
                                                                  \
        linked_##NAME##_queue_init(new_node);                     \
        new_node->data = data;                                    \
        if (!head->tail)                                          \
        {                                                         \
            head->tail = head;                                    \
        }                                                         \
                                                                  \
        head->tail->next = new_node;                              \
        head->tail = new_node;                                    \
        head->size++;                                             \
        return TRUE;                                              \
    }                                                             \
                                                                  \
    static inline bool linked_##NAME##_queue_prepend(linked_##NAME##_queue_t **head_ptr, V data) \
    {                                                             \
        if (!head_ptr || !*head_ptr)                                  \
        {                                                         \
            return FALSE;                                         \
        }                                                         \
                                                                  \
        linked_##NAME##_queue_t *new_node = malloc(sizeof(linked_##NAME##_queue_t)); \
        if (!new_node)                                            \
        {                                                         \
            return FALSE;                                         \
        }                                                         \
                                                                  \
        linked_##NAME##_queue_t *head = *head_ptr;                \
                                                                  \
        linked_##NAME##_queue_init(new_node);                     \
        new_node->data = data;                                    \
        new_node->tail = head->tail;                              \
        new_node->size = head->size + 1;                          \
                                                                  \
        head->tail = NULL;                                        \
        head->size = 0;                                           \
        new_node->next = head;                                    \
        *head_ptr = new_node;                                     \
        return TRUE;                                              \
    }                                                             \
                                                                  \
    static inline void linked_##NAME##_queue_free(linked_##NAME##_queue_t *head) \
    {                                                             \
        if (!head)                                                \
        {                                                         \
            return;                                               \
        }                                                         \
                                                                  \
        linked_##NAME##_queue_t *new_node = malloc(sizeof(linked_##NAME##_queue_t)); \
        if (!new_node)                                            \
        {                                                         \
            return;                                               \
        }                                                         \
                                                                  \
        linked_##NAME##_queue_t *current = head;                  \
                                                                  \
        while (current)                                           \
        {                                                         \
            linked_##NAME##_queue_t *next_node = current->next;   \
            free(current);                                        \
        }                                                         \
    }

#ifndef LINKED_QUEUE_GENERIC_DEFINED
    DEFINE_LINKED_QUEUE(void *, generic);
#   define LINKED_QUEUE_GENERIC_DEFINED 1
#endif

#endif //FLUENT_LIBC_LINKED_QUEUE_LIBRARY_H