// i made this file unusable after trying to optimize it once i got the correct answers lol oops

#include <stdlib.h>
#include <stdbool.h>

#include "advent9.h"

#define NUM(CHAR) ((size_t)(CHAR-48))
#define NO_ID -1

typedef struct node_t Node;

typedef enum node_type_e {
    FREE,
    USED,
} NodeType;

typedef struct node_t {
    int id;
    size_t size;
    NodeType type;
    Node* next;
    Node* prev;
} Node;

typedef struct arena_t {
    Node* start;
    Node* end;
    Node* first_free;
} Arena;

Arena* deseralize(const char* nodemap, const int mapsize) {
    int id = 0;
    int offset = 0;
    Arena* arena = malloc(sizeof(Arena));
    Node* node = malloc(sizeof(Node));
    node->id = id;
    node->type = USED;
    node->size = NUM(nodemap[offset]);
    node->next = NULL;
    node->prev = NULL;
    arena->start = node;
    arena->end = node;
    arena->first_free = NULL;
    offset++;
    id++;
    while (offset < mapsize) {
        if (nodemap[offset] == '\0') break; // null-terminated
        if (NUM(nodemap[offset]) != 0) {
            Node* add = malloc(sizeof(Node));
            add->id = NO_ID;
            add->type = FREE;
            add->size = NUM(nodemap[offset]);
            add->next = NULL;
            add->prev = arena->end;
            arena->end->next = add;
            arena->end = add;
            if (arena->first_free == NULL) {
                arena->first_free = add;
            }
        }
        Node* add = malloc(sizeof(Node));
        add->id = id;
        add->type = USED;
        add->size = NUM(nodemap[offset+1]);
        add->next = NULL;
        add->prev = arena->end;
        arena->end->next = add;
        arena->end = add;
        offset += 2;
        id++;
    }
    return arena;
}

void recalc_first_free(Arena* arena) {
    arena->first_free = NULL;
    Node* node = arena->start;
    while(node) {
        if (node->type == FREE) {
            arena->first_free = node;
            break;
        }
        node = node->next;
    }
}

void merge_free_nodes(Node* node, bool left, bool right) {
    if (left) {
        Node* nodeL = node->prev;
        if (nodeL->prev) {
            nodeL->prev->next = node;
        }
        node->prev = nodeL->prev;
        node->size += nodeL->size;
        free(nodeL);
    }
    if (right) {
        Node* nodeR = node->next;
        if (nodeR->next) {
            nodeR->next->prev = node;
        }
        node->next = nodeR->next;
        node->size += nodeR->size;
        free(nodeR);
    }
}

void yank_out_node(Arena* arena, Node* node) {
    Node* free = malloc(sizeof(Node));
    bool left_free = false, right_free = false;
    free->id = NO_ID;
    free->size = node->size;
    free->type = FREE;
    if (node->prev) {
        left_free = (node->prev->type == FREE);
        node->prev->next = free;
    } else {
        arena->start = free;
    }
    if (node->next) {
        right_free = (node->next->type == FREE);
        node->next->prev = free;
    } else {
        arena->end = free;
    }
    free->next = node->next;
    free->prev = node->prev;
    printf("frees? left:%d right:%d \n",left_free, right_free);
    if (left_free || right_free) {
        merge_free_nodes(free, left_free, right_free);
    }
    recalc_first_free(arena);
}

void insert(Arena* arena, Node** free_node_ptr, Node* node) {
    Node* free_node = (*free_node_ptr);
    if (node->size < free_node->size) {
        if (!free_node->prev) { 
            arena->start = node;
        } else {
            free_node->prev->next = node;
        }
        node->next = free_node;
        node->prev = free_node->prev;
        free_node->prev = node;
        free_node->size -= node->size;
    } else {
        if (free_node == arena->end)   arena->end = node;
        if (free_node == arena->start) arena->start = node;
        node->next = free_node->next;
        node->prev = free_node->prev;
        if (free_node->next) {
            free_node->next->prev = node;
        }
        if (free_node->prev) {
            free_node->prev->next = node;
        }
        if (free_node == arena->first_free){
            printf("free node is arena first free!\n");
            recalc_first_free(arena);
        }
        printf("node of size %llu inserted on free node of size %llu\n", node->size, free_node->size);
        printf("freeing %p!\n", free_node);
        free(free_node);
    }
}

void break_apart_node(Node* part1, size_t size) {
    Node* part2 = malloc(sizeof(Node));
    part1->size -= size;
    part2->id = part1->id;
    part2->size = size;
    part2->next = part1->next;
    part2->prev = part1;
    part2->type = USED;
    part1->next = part2;
}

void insert_recurse_breakapart(Arena* arena, Node* node) {
    int free_size = arena->first_free->size;
    if (node->size <= free_size) {
        printf("2\n");
        printf("(insert_recurse_breakapart) node: %p, free: %p\n", node, arena->first_free);
        insert(arena, &(arena->first_free), node);
    } else {
        printf("3\n");
        printf("broke apart node from node->size: %llu", node->size);
        break_apart_node(node, free_size);
        printf(" to %llu and %llu\n", node->size, node->next->size);
        printf("(insert_recurse_breakapart) node: %p, free: %p\n", node, arena->first_free);
        printf("4\n");
        insert_recurse_breakapart(arena, node->next);
        printf("(insert_recurse_breakapart) node: %p, free: %p\n", node, arena->first_free);
        printf("5\n");
        insert_recurse_breakapart(arena, node);
        printf("6\n");
    }
}

void print_arena(Arena* arena) {
    Node* node = arena->start;
    int length = 0;
    while (node) {
        for(size_t loops = 0; loops < node->size; loops++) {
            if (node->type == FREE) { 
                printf("."); 
            } else {
                printf("%d", node->id);
            }
            length++;
            if (length >= 50) { 
                printf("\n");
                length = 0;
            }
        }
        node = node->next;
    }
    printf("\n");
}

void move_file(Arena* arena, Node* node) {
    Node* search = arena->start;
    while (search) {
        if (search == node) return;
        if (search->type == FREE && search->size >= node->size) {
            Node** free_guarantee = &(search->prev->next);
            yank_out_node(arena, node);
            insert(arena, free_guarantee, node);
            return;
        }
        search = search->next;
    }
}

void move_file_pieces(Arena* arena, Node* node) {
    yank_out_node(arena, node);
    insert_recurse_breakapart(arena, node);
}

bool defragment(Arena* arena, bool split) {
    if (!arena->first_free) return false;
    Node* node = arena->end;
    int current_id = -1;
    while (node) {
        if (arena->first_free == node) {
            break;
        }
        if (arena->first_free == arena->end) {
            break;
        }
        if (node->type == FREE) {
            node = node->prev;
            continue;
        }
        if (split) {
            printf("node: %p\n", node);
            printf("arena->end: %p\n", arena->end);
            printf("arena->first_free: %p\n", arena->first_free);
            printf("arena->end->prev: %p\n", arena->end->prev);
            move_file_pieces(arena, node);
            node = arena->end;
        } else {
            if (current_id == -1) {
                current_id = node->id;
            }
            Node* next_file = NULL;
            if (node->prev) {
                if (node->prev->type == FREE) {
                    if (node->prev->prev) {
                        next_file = node->prev->prev;
                    }
                } else {
                    next_file = node->prev;
                }
            }
            if (node->id == current_id && node != arena->start) {
                move_file(arena, node);
                current_id--;
            }
            node = next_file;
        }
    }
    return true;
}

unsigned long long checksum(Arena* arena) {
    Node* node = arena->start;
    unsigned long long checksum = 0;
    int position = 0;
    while(node) {
        if (node->type != FREE) {
            for (int i = 0; i < node->size; i++) {
                checksum += node->id * (position + i);
            }
        }
        position += node->size;
        node = node->next;
    }
    return checksum;
}

int main(void) {
    Arena* arena;
    //arena = deseralize(large, sizeof(large));
    //defragment(arena, false);
    //printf("checksum for answer 2: %llu\n", checksum(arena));
    //free(arena);
    arena = deseralize(large, sizeof(large));
    defragment(arena, true);
    printf("checksum for answer 1: %llu\n", checksum(arena));
    free(arena);
    return 0;
}