#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "arena.h"
#include "type_defs.h"

u32* foo(mem_arena_t* arena, u32 n) {
    mem_arena_temp_t scratch = arena_scratch_get(&arena, 1);

    u32* other_data = PUSH_ARRAY(scratch.arena, u32, n);

    u32* nums = PUSH_ARRAY(arena, u32, n);

    arena_scratch_release(scratch);

    return nums;
}

void bar(void) {
    mem_arena_temp_t scratch = arena_scratch_get(NULL, 0);

    u32* nums = foo(scratch.arena, 100);

    arena_scratch_release(scratch);
}

int main(void) {
    mem_arena_t* perm_arena = arena_create(GiB(1), MiB(1));
 
    while(1) {
        arena_push(perm_arena, MiB(16), false);
        getc(stdin);
    }

    arena_destroy(perm_arena);

    return 0;
}