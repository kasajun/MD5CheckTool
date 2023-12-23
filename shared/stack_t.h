#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __STACK_T_H_6C4C5AF4_8605_4862_ADBB_A821A160BA8A__
#define __STACK_T_H_6C4C5AF4_8605_4862_ADBB_A821A160BA8A__

#include <stdlib.h>

#define MAX_STACK_DATA 0x8000
typedef struct stack_st
{
	size_t arr[MAX_STACK_DATA];
	int top;
} stack_t;

#ifdef __cplusplus
extern "C" {
#endif

size_t push_stack(stack_t* stk, size_t input);
size_t pop_stack(stack_t* stk);
void init_stack(stack_t* stk);
void show_stack(stack_t* stk);

#ifdef __cplusplus
}
#endif
#endif
