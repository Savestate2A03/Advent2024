#include "advent4.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SIZE_X 140
#define SIZE_Y 140
#define DEFAULT_QUEUE_SIZE 4

static const char SEQUENCE[] = {'X', 'M', 'A', 'S'};

typedef struct node_data_t NodeData;
typedef void (*NodeFn)(NodeData*);

typedef struct node_data_t {
    NodeFn  fn;
    uint8_t depth;
    uint8_t x;
    uint8_t y;
    uint8_t _padding;
} NodeData;

typedef struct queue_t {
    NodeData* nodes;
    uint32_t front;
    uint32_t back;
    size_t size;
} NodeQueue;

// Movement options, typedefed for callbacks in queue
void move_l(NodeData* node) { node->x -= 1; node->depth++; node->fn = move_l; }
void move_r(NodeData* node) { node->x += 1; node->depth++; node->fn = move_r; }
void move_u(NodeData* node) { node->y -= 1; node->depth++; node->fn = move_u; }
void move_d(NodeData* node) { node->y += 1; node->depth++; node->fn = move_d; }
void move_ul(NodeData* node) { node->x -= 1; node->y -= 1; node->depth++; node->fn = move_ul; }
void move_dl(NodeData* node) { node->x -= 1; node->y += 1; node->depth++; node->fn = move_dl; }
void move_ur(NodeData* node) { node->x += 1; node->y -= 1; node->depth++; node->fn = move_ur; }
void move_dr(NodeData* node) { node->x += 1; node->y += 1; node->depth++; node->fn = move_dr; }

// Add a node to the queue. Performs a shallow copy based on the provided node
NodeData* queue_node(NodeQueue* queue, NodeData** base_node, int base_node_index) {
    // If this is the first time adding to the queue, create it
    if (queue->size == 0) {
        queue->nodes = malloc(sizeof(NodeData) * DEFAULT_QUEUE_SIZE);
        queue->size = DEFAULT_QUEUE_SIZE;
        queue->back = 0;
        queue->front = 0;
    } else {
        // Position the back index for recieving the new node
        queue->back++;
    }

    // If we have exceeded the allocated size for our node queue, reallocate space for it
    if (queue->back >= queue->size) {
        NodeData* new_ptr = realloc(queue->nodes, (queue->size + queue->size) * sizeof(NodeData));
        if (new_ptr) {
            // Sanity checking
            queue->nodes = new_ptr;
            queue->size += queue->size;
            if (base_node_index >= 0) {
                *base_node = &(queue->nodes[base_node_index]);
            }
        } else {
            // Unlikely to happen, but if it does, you will quickly find out lol
            return NULL;
        }
    }

    // Shallow copy the provided node data for the new node
    // and place it at the back of the processing queue
    NodeData* node = &(queue->nodes[queue->back]);
    node->fn    = (*base_node)->fn;
    node->depth = (*base_node)->depth;
    node->x     = (*base_node)->x;
    node->y     = (*base_node)->y;

    // Chain effects if you so wish
    return node;
}

// Breadth first search, adding new visitations to the back of the queue
void bfs(NodeQueue* queue, int* successes) {
    NodeData* node;
    // Once the front of the queue reaches the back, we have exhausted all options
    while(queue->front <= queue->back) {
        // Get the front-most node
        node = &(queue->nodes[queue->front]);
        // If it has a progression function assigned, continue on to processing
        if (node->fn) {
            // If the node is within the bounds of our puzzle...
            if (node->x >= 0 && node->x < SIZE_X && node->y >= 0 && node->y < SIZE_Y) {
                // If the character the node is representing matches the desired sequence...
                if (SEQUENCE[node->depth] == ADVENT4INPUT[node->y][node->x]) {
                    // If we have NOT reached our required depth of analysis
                    if (node->depth < sizeof(SEQUENCE) - 1) {
                        // Process the node once again and add it to the queue
                        node->fn(node);
                        queue_node(queue, &node, queue->front);
                    } else {
                        // Otherwise this means we have matched all requirements!
                        *successes += 1;
                    }
                }
            }
        } else {
            // For nodes that have just been added to the queue, kick them
            // into gear with progression functions to analyze the board
            move_l(queue_node(queue,  &node, queue->front));
            move_r(queue_node(queue,  &node, queue->front));
            move_u(queue_node(queue,  &node, queue->front));
            move_d(queue_node(queue,  &node, queue->front));
            move_ul(queue_node(queue, &node, queue->front));
            move_dl(queue_node(queue, &node, queue->front));
            move_ur(queue_node(queue, &node, queue->front));
            move_dr(queue_node(queue, &node, queue->front));
        }
        queue->front++; // Next node in the queue please
    }
}

int main(void) {
    // Make our base node and queue
    NodeQueue queue = {0};
    NodeData node = {0};
    int successes = 0;

    // Pointer to node pointer needed for queue kick-start
    NodeData* node_ptr = &node;

    // Set default values
    node_ptr->depth = 0;
    node_ptr->fn = NULL;
    
    for (uint8_t y = 0; y < SIZE_Y; y++) {
        for (uint8_t x = 0; x < SIZE_X; x++) {
            // If we find an X on the board, create a node that
            // will search starting from those coordinates
            if (ADVENT4INPUT[y][x] == 'X') {
                node_ptr->x = x;
                node_ptr->y = y;
                queue_node(&queue, &node_ptr, -1);
            }
        }
    }

    // Kick off the breadth first search
    bfs(&queue, &successes);

    printf("Successes: %d\n", successes);

    return 0;
}