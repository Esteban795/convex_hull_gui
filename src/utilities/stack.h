#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//T-type stack implementation, using linked lists.
// O(1) push and pop. O(n) space complexity
// here, Ts will be ints because we're just storing array indexes.

typedef int T;

struct node {
    T data;
    struct node* next;
};

typedef struct node node;

struct stack {
    int len;
    node* top;
};

typedef struct stack stack;

node* new_node(T d);

stack* stack_new(void);

void free_stack(stack* s);

void stack_push(stack* s,T data);

T stack_pop(stack* s);

T stack_peek(stack* s);

T stack_peek_second(stack* s);

stack* stack_copy(stack* s);

void stack_print(stack* s);

T* stack_to_arr(stack* s);

