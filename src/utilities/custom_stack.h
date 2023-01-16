#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//T-type custom_stack implementation, using linked lists.
// O(1) push and pop. O(n) space complexity
// here, Ts will be ints because we're just storing array indexes.

typedef int T;

struct node {
    T data;
    struct node* next;
};

typedef struct node node;

struct custom_stack {
    int len;
    node* top;
};

typedef struct custom_stack custom_stack;

node* new_node(T d);

custom_stack* stack_new(void);

void free_stack(custom_stack* s);

void stack_push(custom_stack* s,T data);

T stack_pop(custom_stack* s);

T stack_peek(custom_stack* s);

T stack_peek_second(custom_stack* s);

custom_stack* stack_copy(custom_stack* s);

void stack_print(custom_stack* s);

T* stack_to_arr(custom_stack* s);

