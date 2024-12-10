#include "advent6.h"
#include <stdbool.h>

#define PUZZLE PUZZLEMINI
#define SIZE PMINI_SIZE

typedef struct coordinates_t {
    int x;
    int y;
} Coordinates;

typedef enum guard_e {
    UP    = '^',
    DOWN  = 'v',
    LEFT  = '<',
    RIGHT = '>'
} GuardDirection;

typedef struct guard_t {
    GuardDirection direction;
    Coordinates coords;
    Coordinates velocity;
    bool blocked;
    int visit_count;
} Guard;

typedef enum state_t {
    OK,
    DONE,
    ROTATED,
} State;

Coordinates add_coords(Coordinates* c1, Coordinates* c2) {
    Coordinates results;
    results.x = c1->x + c2->x;
    results.y = c1->y + c2->y;
    return results;
}

void direction_to_velocity(GuardDirection direciton, Coordinates* velocity) {
    velocity->x = 0;
    velocity->y = 0;
    switch(direciton) {
        case UP:
            velocity->y = -1;
            break;
        case DOWN:
            velocity->y = 1;
            break;
        case LEFT:
            velocity->x = -1;
            break;
        case RIGHT:
            velocity->x = 1;
            break;
        default:
            break;
    }
}

bool start(Guard* guard) {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            const char* c = &(PUZZLE[y][x]);
            if (*c == UP || *c == DOWN || *c == LEFT || *c == RIGHT) {
                printf("CHAR: %c\n", *c);
                guard->coords.x = x;
                guard->coords.y = y;
                guard->direction = *c;
                guard->visit_count = 1;
                guard->blocked = false;
                direction_to_velocity(*c, &(guard->velocity));
                PUZZLE[y][x] = 'X';
                return true;
            }
        }
    }
    return false;
}

bool check_exit(Guard* guard) {
    Coordinates* coords = &(guard->coords);
    return (coords->x < 0 || coords->x >= SIZE || coords->y < 0 || coords->y >= SIZE);
}

bool check_exit_projected(Guard* guard) {
    Coordinates projected = add_coords(&(guard->coords), &(guard->velocity));
    return (projected.x < 0 || projected.x >= SIZE || projected.y < 0 || projected.y >= SIZE);
}

void rotate_right(Guard* guard) {
    if (guard->direction == UP)    { guard->direction = RIGHT; return; }
    if (guard->direction == DOWN)  { guard->direction = LEFT;  return; }
    if (guard->direction == LEFT)  { guard->direction = UP;    return; }
    if (guard->direction == RIGHT) { guard->direction = DOWN;  return; }
}

bool check_obstactle(Guard* guard) {
    if (check_exit_projected(guard)) return false;
    Coordinates projected = add_coords(&(guard->coords), &(guard->velocity));
    if (PUZZLE[projected.y][projected.x] == '#') {
        rotate_right(guard);
        return true;
    }
    return false;
}

bool move(Guard* guard) {
    guard->coords = add_coords(&(guard->coords), &(guard->velocity));
    if(check_exit(guard)) return true;
    if (PUZZLE[guard->coords.y][guard->coords.x] != 'X') {
        PUZZLE[guard->coords.y][guard->coords.x] = 'X';
        guard->visit_count++;
    }
    return false;
}

State step(Guard* guard) {
    if (check_obstactle(guard)) return ROTATED;
    if (move(guard)) return DONE;
    return OK;
}

int main(void) {
    printf("Starting...\n");
    Guard guard;
    if (!start(&guard)) {
        printf("Guard not found!\n");
        return 1;
    }
    while (step(&guard) != DONE) {}
    printf("Stepped: %d\n", guard.visit_count);
    for (int line = 0; line < SIZE; line++) {
        printf("%s\n", PUZZLE[line]);
    }
}