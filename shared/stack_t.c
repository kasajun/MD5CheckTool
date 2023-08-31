#include <stdio.h>
#include "stack_t.h"


size_t push_stack(stack_t* stk, size_t input)
{
	if (stk->top < (MAX_STACK_DATA - 1))
	{
		stk->top++;
		stk->arr[stk->top] = input;
		return 0;
	}
	else {
		return (size_t)-1;
	}
}

size_t pop_stack(stack_t* stk)
{
	if (stk->top != 0)
	{
		const size_t temp = stk->top;
		stk->top--;
		return stk->arr[temp];
	}
	else {
		return (size_t)-1;
	}
}

void init_stack(stack_t* stk)
{
	int i;

	for (i = 0; i < MAX_STACK_DATA; i++) {
		stk->arr[i] = 0;
	}
	stk->top = 0;
	return;
}

void show_stack(stack_t* stk)
{
	int i;

	for (i = 0; i < MAX_STACK_DATA; i++) {
		printf("%d: %zd\r\n", i, stk->arr[i]);
	}
	return;
}
