#include "stack.h"

//T-type stack implementation, using linked lists.
// O(1) push and pop. O(n) space complexity
// here, Ts will be ints because we're just storing array indexes.


node* new_node(T d){
    node* n = malloc(sizeof(node));
    n->data = d;
    n->next = NULL;
    return n;
}

stack* stack_new(void){
    stack* s = malloc(sizeof(stack));
    s->len = 0;
    s->top = NULL;
    return s;
}

void free_list(node* n){
    if (n == NULL) return;
    free_list(n->next);
    free(n);
}

void free_stack(stack* s){
    free_list(s->top);
    free(s);
}

void stack_push(stack* s,T data){
    node* n = new_node(data);
    if (s->len > 0){
        node* previous_top = s->top;
        n->next = previous_top;
    }
    s->len++;
    s->top = n;
}

T stack_pop(stack* s){
    assert(s->len > 0);
    node* top = s->top;
    T result = top->data;
    s->top = top->next;
    s->len = s->len - 1;
    free(top);
    return result;
}

T stack_peek(stack* s){
    assert(s->len >0);
    return s->top->data;
}

T stack_peek_second(stack* s){
    assert(s->len > 1);
    return s->top->next->data;
}

void add_rec(stack* s,node* n){
    if (n == NULL) return;
    add_rec(s,n->next);
    stack_push(s,n->data);
}

stack* stack_copy(stack* s){
    stack* new_stack = stack_new();
    add_rec(new_stack,s->top);
    return new_stack;
}

void print_rec(node* n){
    if (n != NULL){
        printf("Data : %d\n",n->data);
        print_rec(n->next);
    }
}

void stack_print(stack* s){
    print_rec(s->top);
}

T* stack_to_arr(stack* s){
    T* arr = malloc(sizeof(int) * s->len);
    stack* c = stack_copy(s);
    for (int i = 0; i < s->len;i++){
        arr[i] = stack_pop(c);
    }
    free_stack(c);
    return arr;
}