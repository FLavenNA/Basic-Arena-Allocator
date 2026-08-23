#include <stdlib.h>
#include <threads.h> 
#include <stdbool.h>

#include "type_defs.h"

#define ARENA_BASE_POS (sizeof(mem_arena_t))
#define ARENA_ALIGN (sizeof(void*))
#define ALIGN_UP_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))
#define PUSH_STRUCT(arena, T) (T*)arena_push((arena), sizeof(T), false)
#define PUSH_STRUCT_NZ(arena, T) (T*)arena_push((arena), sizeof(T), true)
#define PUSH_ARRAY(arena, T, n) (T*)arena_push((arena), sizeof(T) * (n), false)
#define PUSH_ARRAY_NZ(arena, T, n) (T*)arena_push((arena), sizeof(T) * (n), true)

#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct mem_arena {
    u64 reserve_size;
    u64 commit_size;

    u64 pos;
    u64 commit_pos;
} mem_arena_t;

typedef struct mem_arena_temp {
    mem_arena_t* arena;
    u64 start_pos;
} mem_arena_temp_t;

mem_arena_t* arena_create(u64 reserve_size, u64 commit_size);
void arena_destroy(mem_arena_t* arena);
void* arena_push(mem_arena_t* arena, u64 size, b32 non_zero);
void arena_pop(mem_arena_t* arena, u64 size);
void arena_pop_to(mem_arena_t* arena, u64 pos);
void arena_clear(mem_arena_t* arena);

mem_arena_temp_t arena_temp_begin(mem_arena_t* arena);
void arena_temp_end(mem_arena_temp_t temp);

mem_arena_temp_t arena_scratch_get(mem_arena_t** conflicts, u32 num_conflicts);
void arena_scratch_release(mem_arena_temp_t scratch);

extern thread_local mem_arena_t* _scratch_arenas[2];