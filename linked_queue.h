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

/**
 * @brief Advances the head pointer to the next node in the linked queue.
 *
 * This function moves the head pointer to the next node in the queue,
 * updates the size and tail pointers accordingly, frees the current head node,
 * and decrements the size hint. If the head pointer or the node it points to is NULL,
 * the function does nothing.
 *
 * @param head Double pointer to the head node of the linked queue.
 */
static inline void linked_queue_next(linked_queue_t **head)
{
    // Guard against NULL pointer
    if (!head || !*head)
    {
        return; // Nothing to advance
    }

    // Move the head pointer to the next node in the queue
    linked_queue_t *next_node = (*head)->next;

    // Relocate the size hint
    next_node->size = (*head)->size - 1; // Decrement the size hint

    // Relocate the tail
    next_node->tail = (*head)->tail; // Set the tail pointer to the current tail

    free(*head); // Free the current head node
    *head = next_node; // Update head to point to the next node
}

/**
 * @brief Appends a new node to the end of the linked queue.
 *
 * Allocates a new node, initializes it, and appends it to the tail of the queue.
 * If the queue has no tail, the head is set as the tail. The function updates
 * the tail pointer and increments the size hint. The data pointer for the new
 * node is not set in this function; it must be set by the caller if needed.
 *
 * @param head Pointer to the head node of the linked queue.
 * @param data Pointer to the data to be stored in the new node (not set in this function).
 * @return TRUE if the node was successfully appended, FALSE otherwise.
 */
static inline bool linked_queue_append(linked_queue_t *head, void *data)
{
    // Guard against NULL pointer
    if (!head)
    {
        return FALSE; // Nothing to append to
    }

    // If we have a tail, append to the tail's next
    linked_queue_t *new_node = malloc(sizeof(linked_queue_t));
    if (!new_node)
    {
        return FALSE; // Memory allocation failed
    }

    linked_queue_init(new_node); // Initialize the new node
    new_node->data = data; // Set the data pointer to the provided data

    // Check if we have a tail node
    if (!head->tail)
    {
        // If there is no tail, set the head as the tail
        head->tail = head;
    }

    head->tail->next = new_node; // Link the new node to the tail
    head->tail = new_node; // Update the tail pointer
    head->size++; // Increment the size hint
    return TRUE; // Successfully appended the new node
}

/**
 * @brief Prepends a new node to the beginning of the linked queue.
 *
 * Allocates a new node, initializes it, and inserts it at the front of the queue.
 * The new node becomes the new head of the queue. The data pointer for the new
 * node is set to the provided data. The function does not update the tail or size
 * members; these must be managed by the caller if needed.
 *
 * @param head_ptr Double pointer to the head node of the linked queue.
 * @param data Pointer to the data to be stored in the new node.
 * @return TRUE if the node was successfully prepended, FALSE otherwise.
 */
static inline bool linked_queue_prepend(linked_queue_t **head_ptr, void *data)
{
    // Guard against NULL pointer
    if (!head_ptr || !*head_ptr)
    {
        return FALSE; // Nothing to prepend to
    }

    // Allocate a new node
    linked_queue_t *new_node = malloc(sizeof(linked_queue_t));
    if (!new_node)
    {
        return FALSE; // Memory allocation failed
    }

    // Get the head
    linked_queue_t *head = *head_ptr;

    // Initialize the new node
    linked_queue_init(new_node);
    new_node->data = data; // Set the data pointer to the provided data

    // Move the tail pointer to the new node
    new_node->tail = head->tail; // Set the tail pointer to the current tail

    // Move the size hint to the new node
    new_node->size = head->size + 1; // Increment the size hint

    // Set the head's tail to NULL and the size to 0
    head->tail = NULL; // Clear the tail pointer of the current head
    head->size = 0; // Reset the size hint of the current head

    // Set the head as the next node of the new node
    new_node->next = head; // Link the new node to the current head

    // Update the head pointer to point to the new node
    *head_ptr = new_node; // Update head to point to the new node

    return TRUE; // Successfully prepended the new node
}

/*
 * @brief Frees all nodes in a linked queue.
 *
 * Iterates through the linked queue starting from the given head node,
 * freeing each node in the queue. If the head pointer is NULL, the function
 * does nothing. This function does not free the data pointed to by the
 * `data` member of each node; it only frees the nodes themselves.
 *
 * @param head Pointer to the head node of the linked queue to free.
 */
static inline void linked_queue_free(linked_queue_t *head)
{
    // Guard against NULL pointer
    if (!head)
    {
        return; // Nothing to free
    }

    // Define a current pointer to iterate through the queue
    linked_queue_t *current = head;

    // Iterate through the queue until we reach the end
    while (current)
    {
        // Get the next node before freeing the current one
        linked_queue_t *next_node = current->next; // Store the next node

        // Free the current node
        free(current);

        // Move to the next node
        current = next_node;
    }
}

#endif //FLUENT_LIBC_LINKED_QUEUE_LIBRARY_H