# Test Suite Type Parameterization — Design Report

## 1. Problem Statement

The test suite under `test/list-proto/` currently tests only `List__i` (the `int` variant). Every test file hard-codes:

- Type names (`List__i`, `List_Node__i`)
- Function names (`list__i_init`, `list__i_push`, …)
- C types (`int` for variables, arrays, comparators)
- Value literals (`0`, `42`, `-1`, `0xAB`, `0xCD`)
- Assertion macros (`TEST_ASSERT_EQUAL_INT`)

Meanwhile, `ds.h` and `ds.c` already instantiate `List__f` (the `float` variant), and future types (`double`, `char`, structs) are expected. The test suite needs to run the same battery of tests against every instantiated type.

## 2. Design Goals

1. **Single source of truth** — test logic is written once, not copy-pasted per type.
2. **Compile-time instantiation** — no runtime polymorphism overhead; each type gets its own compiled test functions.
3. **Minimal boilerplate per new type** — adding a type requires a short (5–15 line) shim plus one hook in `test_units.h` / `main.c` / `Makefile`.
4. **Type-specific comparison** — `int` uses `==`; `float`/`double` need epsilon; future struct types may need custom comparators.
5. **Type-specific value generation** — sequential `0, 1, 2, …` for int; `0.0f, 1.0f, 2.0f, …` for float; configurable for future types.
6. **Type-specific sentinel values** — `-1`, `0xAB`, `0xCD` for int; `-1.0f`, `3.14f`, `99.0f` for float.
7. **Backward compatible** — existing int tests must continue to pass unchanged.
8. **Makefile-compatible** — the build system lists source files explicitly, so new shim `.c` files must be added to `SRCS`.

## 3. Inventory: What Must Be Parameterized

Every type-dependent element in the test suite falls into one of these categories:

### 3.1 Names (token-pasted from suffix)

| Pattern | Example for `int` (`S=i`) | Example for `float` (`S=f`) |
|---|---|---|
| List typedef | `List__i` | `List__f` |
| Node pointer typedef | `List_Node__i` | `List_Node__f` |
| Struct tag | `struct list__i` | `struct list__f` |
| Node struct tag | `struct list_node__i` | `struct list_node__f` |
| init | `list__i_init` | `list__f_init` |
| deinit | `list__i_deinit` | `list__f_deinit` |
| push | `list__i_push` | `list__f_push` |
| pop | `list__i_pop` | `list__f_pop` |
| insert | `list__i_insert` | `list__f_insert` |
| erase | `list__i_erase` | `list__f_erase` |
| find | `list__i_find` | `list__f_find` |
| head | `list__i_head` | `list__f_head` |
| size | `list__i_size` | `list__f_size` |

### 3.2 C type and sizeof

- Variable/array declarations: `int val` → `float val`
- `sizeof(int)` → `sizeof(float)` for `malloc`
- Comparator cast: `*(const int *)a` → `*(const float *)a`

### 3.3 Value generation

| Use case | int | float |
|---|---|---|
| Sequential fill `0..sz-1` | `i` (implicit cast) | `(float)i` |
| Random values | `rand()` | `(float)rand()` or a scaled float |
| Sentinel "not in list" | `-1` | `-1.0f` |
| Sentinel marker A | `0xAB` | `3.14f` |
| Sentinel marker B | `0xCD` | `99.0f` |
| Multi-insert values | `{-3, -2, -1}` | `{-3.0f, -2.0f, -1.0f}` |
| Insert-on-empty values | `10, 20, 30` | `10.0f, 20.0f, 30.0f` |
| Size-test multi values | `{100, 200, 300}` | `{100.0f, 200.0f, 300.0f}` |

### 3.4 Equality / assertion

| Aspect | int | float |
|---|---|---|
| Equality check | `(a) == (b)` | `fabsf((a)-(b)) < 1e-6f` |
| Unity assertion | `TEST_ASSERT_EQUAL_INT` | `TEST_ASSERT_EQUAL_FLOAT` |

### 3.5 Comparators (for `find` with custom cmp)

| Comparator | int | float |
|---|---|---|
| `cmp` (actual comparison) | `int_cmp`: cast to `const int*`, subtract | `float_cmp`: cast to `const float*`, subtract |
| `never_match` | type-agnostic (returns 1, ignores args) — can be shared |

### 3.6 Test function names (public entry points)

Current names: `test_simp_init`, `test_push`, `test_insert`, etc.

After parameterization: `test_simp_init__i`, `test_simp_init__f`, `test_push__i`, `test_push__f`, etc.

## 4. Approaches Considered

### Approach A: Copy-Paste Per Type

Duplicate each test `.c` file and replace `i` → `f`, `int` → `float`, etc.

- **Pros**: Zero design work; each file is simple and debuggable.
- **Cons**: ~8×N files for N types; any bug fix or test logic change requires N parallel edits; divergence is inevitable.
- **Verdict**: Reject. Unsustainable beyond 2 types.

### Approach B: Runtime Polymorphism (Type Descriptor Struct)

Define a `TypeDescriptor` vtable with function pointers (`gen_value`, `cmp`, `assert_eq`). Tests pass values as `void*`. One binary tests all types by iterating over a descriptor array.

- **Pros**: Clean C, no macros, one small binary.
- **Cons**: Loses compile-time type checking; `void*`/`memcpy` everywhere makes tests hard to read; function-pointer overhead on every operation; cannot use Unity's type-specific assertion macros directly.
- **Verdict**: Reject. Over-engineered for a compile-time code generation problem.

### Approach C: Scripted Code Generation

A Python/shell script reads a template and emits per-type `.c` files.

- **Pros**: Generated files are plain C, fully debuggable.
- **Cons**: Extra build step; generated files must be checked in or generated at build time; script maintenance burden.
- **Verdict**: Reject. Adds infrastructure complexity disproportionate to the problem.

### Approach D: X-Macro with Per-Type Shim Files (Recommended)

Each test's logic moves to a **template header** (`.template.h`) that uses token-pasting macros for all type-dependent identifiers. Per-type **shim `.c` files** define the type macros and `#include` the template.

- **Pros**: Single source of truth; compile-time generation; no runtime overhead; shim files are ~8 lines each; native C preprocessor, no external tools; type-specific comparison handled cleanly via macros.
- **Cons**: Macro-heavy code can be harder to debug (compiler errors point into the template, not the shim); `#include` trick may confuse some IDEs.
- **Verdict**: **Accept.** This is the same pattern `List_temp.h` itself uses — consistent with the codebase philosophy.

## 5. Recommended Design

### 5.1 Per-type macro contract

Each type defines a header that sets these macros before including a template:

```c
// type/int.h — macros for int
#ifndef LIST_TYPE_INT_H
#define LIST_TYPE_INT_H

#define LIST_SUFFIX       i
#define LIST_TYPE         int
#define LIST_EQ(a, b)     ((a) == (b))
#define LIST_ASSERT_EQ(a, b)  TEST_ASSERT_EQUAL_INT((a), (b))
#define LIST_VAL_FROM_IDX(i)  ((LIST_TYPE)(i))
#define LIST_RAND()           ((LIST_TYPE)rand())
#define LIST_SENTINEL_NEG     ((LIST_TYPE)-1)
#define LIST_SENTINEL_A       ((LIST_TYPE)0xAB)
#define LIST_SENTINEL_B       ((LIST_TYPE)0xCD)
#define LIST_SENTINEL_MULTI   { -3, -2, -1 }
#define LIST_SENTINEL_INSERT  { 10, 20, 30 }
#define LIST_SENTINEL_SIZE    { 100, 200, 300 }

#define LIST_CMP_DEREF(p)  (*(const LIST_TYPE *)(p))

#endif
```

```c
// type/float.h — macros for float
#ifndef LIST_TYPE_FLOAT_H
#define LIST_TYPE_FLOAT_H

#include <math.h>

#define LIST_SUFFIX       f
#define LIST_TYPE         float
#define LIST_EQ(a, b)     (fabsf((a) - (b)) < 1e-6f)
#define LIST_ASSERT_EQ(a, b)  TEST_ASSERT_EQUAL_FLOAT((a), (b))
#define LIST_VAL_FROM_IDX(i)  ((LIST_TYPE)(i))
#define LIST_RAND()           ((LIST_TYPE)rand() / (LIST_TYPE)RAND_MAX)
#define LIST_SENTINEL_NEG     (-1.0f)
#define LIST_SENTINEL_A       (3.14f)
#define LIST_SENTINEL_B       (99.0f)
#define LIST_SENTINEL_MULTI   { -3.0f, -2.0f, -1.0f }
#define LIST_SENTINEL_INSERT  { 10.0f, 20.0f, 30.0f }
#define LIST_SENTINEL_SIZE    { 100.0f, 200.0f, 300.0f }

#define LIST_CMP_DEREF(p)  (*(const LIST_TYPE *)(p))

#endif
```

### 5.2 Token-pasting helpers

Each template includes (or a shared header provides) these building blocks:

```c
// Concatenation indirection (required by C preprocessor)
#define LIST__CAT2(a, b)       a##b
#define LIST__CAT3(a, b, c)    a##b##c
#define LIST__CAT4(a, b, c, d) a##b##c##d

// Type names
#define LIST__TYPEDEF          LIST__CAT2(List__, LIST_SUFFIX)           // List__i
#define LIST__NODE_TYPEDEF     LIST__CAT3(List_Node__, LIST_SUFFIX, )   // List_Node__i
#define LIST__STRUCT_TAG       LIST__CAT2(list__, LIST_SUFFIX)          // list__i
#define LIST__NODE_STRUCT_TAG  LIST__CAT3(list_node__, LIST_SUFFIX, )   // list_node__i

// Function names
#define LIST__FUNC(name)       LIST__CAT4(list__, LIST_SUFFIX, _, name) // list__i_init

// Test function names (public entry points)
#define LIST__TEST(name)       LIST__CAT3(name, __, LIST_SUFFIX)        // test_push__i
```

### 5.3 Template file structure

Each template (e.g., `test_push.template.h`) follows this pattern:

```c
// test_push.template.h
// Expected macros before #include:
//   LIST_SUFFIX, LIST_TYPE, LIST_EQ, LIST_ASSERT_EQ,
//   LIST_VAL_FROM_IDX, LIST_RAND, LIST_SENTINEL_*, LIST_CMP_DEREF
// Provides one public function: LIST__TEST(test_push)

#include "ds.h"
#include "unity.h"
#include <stdlib.h>
#include <time.h>

/*--------------------------- Private Declarations ---------------------------*/
static void test_push_seq(size_t sz);
static void test_push_random(size_t sz);
/*------------------------- Private Declarations END -------------------------*/

/*-------------------------------- Test Unit ---------------------------------*/
void LIST__TEST(test_push)(void)
{
   for (size_t list_sz = 1; list_sz <= 0x100000; list_sz *= 2)
   {
      test_push_seq(list_sz);
      test_push_random(list_sz);
   }
}
/*------------------------------ Test Unit END -------------------------------*/

/*-------------------------------- Test Cases --------------------------------*/
static void test_push_seq(size_t sz)
{
   TEST_ASSERT_NOT_EQUAL_size_t(0, sz);
   LIST__TYPEDEF list = LIST__FUNC(init)();
   TEST_ASSERT_MESSAGE(list, "`" #LIST__FUNC(init) "' failure");
   TEST_ASSERT_NULL_MESSAGE(list->head,
                            "`list->head' not initialized to NULL");
   TEST_ASSERT_EQUAL_UINT_MESSAGE(0, list->size,
                                  "`list->size' not initialized to 0");

   for (size_t i = 0; i < sz; i++)
      TEST_ASSERT_EQUAL_INT(0, LIST__FUNC(push)(list, LIST_VAL_FROM_IDX(i)));
   TEST_ASSERT_EQUAL_size_t(sz, list->size);
   TEST_ASSERT_NOT_NULL(list->head);

   LIST__NODE_TYPEDEF *iter = LIST__FUNC(head)(list);
   TEST_ASSERT_NOT_NULL(iter);

   for (size_t i = sz; i; iter = &(*iter)->next)
      LIST_ASSERT_EQ(LIST_VAL_FROM_IDX(--i), (*iter)->value);

   LIST__FUNC(deinit)(list);
}

static void test_push_random(size_t sz)
{
   // ... same pattern, using LIST_RAND() and LIST_ASSERT_EQ ...
}
/*------------------------------ Test Cases END ------------------------------*/
```

### 5.4 Shim file structure

Each type gets one shim `.c` per template:

```c
// insert__i.c — int instantiation of push + insert tests
#include "type/int.h"
#include "template/test_push.template.h"
#include "template/test_insert.template.h"
```

```c
// insert__f.c — float instantiation of push + insert tests
#include "type/float.h"
#include "template/test_push.template.h"
#include "template/test_insert.template.h"
```

### 5.5 Updated `test_units.h`

```c
#ifndef __TEST_UNITS_H
#define __TEST_UNITS_H

void test_simp_init__i(void);
void test_simp_init__f(void);
void test_push__i(void);
void test_push__f(void);
void test_insert__i(void);
void test_insert__f(void);
void test_pop__i(void);
void test_pop__f(void);
void test_erase__i(void);
void test_erase__f(void);
void test_find__i(void);
void test_find__f(void);
void test_size__i(void);
void test_size__f(void);
void test_stress__i(void);
void test_stress__f(void);

#endif
```

### 5.6 Updated `main.c`

```c
int main(void)
{
   UNITY_BEGIN();

   RUN_TEST(test_simp_init__i);
   RUN_TEST(test_simp_init__f);
   RUN_TEST(test_push__i);
   RUN_TEST(test_push__f);
   RUN_TEST(test_insert__i);
   RUN_TEST(test_insert__f);
   // ... etc

   return UNITY_END();
}
```

(Optionally, group by type with a comment banner for readability.)

### 5.7 Updated Makefile

```makefile
SRCS += main.c ds.c \
        simp_init__i.c simp_init__f.c \
        insert__i.c insert__f.c \
        pop__i.c pop__f.c \
        erase__i.c erase__f.c \
        find__i.c find__f.c \
        size__i.c size__f.c \
        stress__i.c stress__f.c
```

The `type/*.h` and `template/*.h` files are only `#include`d — they are not compilation units and do not appear in `SRCS`.

### 5.8 Directory layout

```
test/list-proto/
  src/
    type/
      int.h                # Macros for int
      float.h              # Macros for float
    template/
      test_simp_init.template.h
      test_push.template.h
      test_insert.template.h
      test_pop.template.h
      test_erase.template.h
      test_find.template.h
      test_size.template.h
      test_stress.template.h
    ds.h                   # List__declare for all types
    ds.c                   # List__define for all types
    test_units.h           # Declares test_X__i + test_X__f
    main.c                 # RUN_TEST for all
    simp_init__i.c         # Shim: int → test_simp_init
    simp_init__f.c         # Shim: float → test_simp_init
    insert__i.c            # Shim: int → test_push + test_insert
    insert__f.c            # Shim: float → test_push + test_insert
    pop__i.c
    pop__f.c
    erase__i.c
    erase__f.c
    find__i.c
    find__f.c
    size__i.c
    size__f.c
    stress__i.c
    stress__f.c
```

Original `.c` files (`simp_init.c`, `insert.c`, `pop.c`, `erase.c`, `find.c`, `size.c`, `stress.c`) are **removed** — their logic has moved into templates.

### 5.9 Comparator handling

Comparators like `int_cmp` and `never_match` currently live inside `find.c` and `stress.c` as `static` functions. In the template:

```c
static int LIST__FUNC(cmp)(const void *a, const void *b)
{
   LIST_TYPE va = LIST_CMP_DEREF(a), vb = LIST_CMP_DEREF(b);
   return (va > vb) - (va < vb);
}

static int LIST__FUNC(never_match)(const void *a, const void *b)
{
   (void)a; (void)b;
   return 1;
}
```

These are referenced as `LIST__FUNC(cmp)` and `LIST__FUNC(never_match)` in test bodies.

### 5.10 Direct struct access consideration

Tests directly access `list->head`, `list->size`, `node->value`, `node->next`. These are **type-agnostic** — the field names are the same regardless of `T`. The only type-dependent aspect is the type of `.value` (which is `LIST_TYPE`), and that is handled by using `LIST_ASSERT_EQ` for comparisons and `LIST_TYPE` for variable declarations.

## 6. Implementation Plan

### Phase 1: Create type definition headers

1. Create `src/type/int.h` with all macros for `int`.
2. Create `src/type/float.h` with all macros for `float`.
3. Verify they compile in isolation (e.g., a throwaway `.c` that includes each and exercises a few macros).

### Phase 2: Convert one test to template (pilot)

4. Choose the simplest test — `simp_init.c`.
5. Create `src/template/test_simp_init.template.h` with the parameterized logic.
6. Create shims `simp_init__i.c` and `simp_init__f.c`.
7. Update `test_units.h` to declare `test_simp_init__i` and `test_simp_init__f`.
8. Update `main.c` to register both.
9. Update `Makefile` to add the new shims, remove the old `simp_init.c`.
10. `make && ../../bin/list-proto` — verify both int and float variants pass.

### Phase 3: Convert remaining tests

11. For each remaining test file, repeat the pilot process:
    - `pop.c` → `test_pop.template.h` (1 template, 2 shims)
    - `insert.c` → `test_push.template.h` + `test_insert.template.h` (2 templates, 2 shims each = 4 shim files total)
    - `erase.c` → `test_erase.template.h`
    - `find.c` → `test_find.template.h`
    - `size.c` → `test_size.template.h`
    - `stress.c` → `test_stress.template.h`

12. Remove all original `.c` files.
13. Final `make clean && make && ../../bin/list-proto` — full test run.

### Phase 4: Validate float-specific behavior

14. Verify that `test_find_null_cmp` for float uses values with unique bit patterns (avoid NaN, avoid `-0.0f` vs `0.0f` since `memcmp` will treat them as distinct).
15. Verify that `LIST_EQ` with epsilon doesn't cause false positives in miss tests.

## 7. Adding a Future Type (Example: `double`)

To add `List__d` (double-precision float):

1. Add `List__declare(double, d)` to `ds.h`.
2. Add `List__define(double, d)` to `ds.c`.
3. Create `src/type/double.h` (copy `float.h`, change `float` → `double`, `fabsf` → `fabs`, `1e-6f` → `1e-9`, `##f` literals to bare).
4. Create 8 shim files: `simp_init__d.c`, `insert__d.c`, `pop__d.c`, `erase__d.c`, `find__d.c`, `size__d.c`, `stress__d.c`.
5. Add `test_*__d` declarations to `test_units.h`.
6. Add `RUN_TEST(test_*__d)` calls to `main.c`.
7. Add shim `.c` files to `Makefile` `SRCS`.

**Total new lines of code: ~80** (one type header + 8 two-line shims + declarations + runner calls).

## 8. Risks and Edge Cases

### 8.1 Float `memcmp` in default comparator

`list__f_find` with `cmp=NULL` uses `memcmp` with `sizeof(float)`. This compares bit patterns exactly. Risks:

- **`-0.0f` vs `0.0f`**: different bit patterns, `memcmp` says not equal. Tests that push `0.0f` and search with `memcmp` must ensure the searched value has the same bit pattern. Using `LIST_VAL_FROM_IDX(0)` consistently avoids this.
- **NaN**: `memcmp` on NaN may or may not match; avoid NaN in find tests.

Mitigation: For the default-comparator tests (`test_find_null_cmp`), use sequential integer-valued floats (e.g., `0.0f, 1.0f, 2.0f, …`) which have well-defined unique bit patterns.

### 8.2 Random values for float

`(float)rand() / (float)RAND_MAX` produces values in `[0, 1]`. For stress tests that track values in an array and search for them later, this is fine — any value is valid. For "miss" tests that search for a value known to be absent, use a sentinel like `-1.0f` (which won't appear from `rand() / RAND_MAX`).

### 8.3 Epsilon comparison in find-miss tests

If `LIST_EQ` uses epsilon (`fabsf(a-b) < 1e-6f`), a miss test searching for `-1.0f` in a list of values in `[0, 1]` will correctly miss. But a miss test searching for `0.5000001f` in a list containing `0.5f` could falsely appear as a hit. Mitigation: use sentinel values far from the generated range.

### 8.4 Macro debugability

Compiler errors in templates will report line numbers inside the template file, not the shim. Using `static` functions (which get distinct names via `LIST__FUNC`) rather than anonymous blocks helps — the function name appears in error messages and reveals which type is being compiled. GCC/Clang's `-E` flag can be used to inspect the preprocessed output when debugging.

### 8.5 Unity assertion macro compatibility

`TEST_ASSERT_EQUAL_FLOAT` may not exist in all Unity versions. If unavailable, use `TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual)` instead. The `LIST_ASSERT_EQ` macro isolates this to one place per type.

### 8.6 `insert.c` splitting

The current `insert.c` contains two public test units: `test_push` and `test_insert`. In the template approach, these become two separate templates (`test_push.template.h` and `test_insert.template.h`), but both can be `#include`d from the same shim file (`insert__i.c` and `insert__f.c`) since they share the same type macros.

## 9. Summary

The recommended approach follows the existing `List_temp.h` pattern: **macro-parameterized templates + per-type shim files**. This is the lightest-weight solution that achieves single-source-of-truth for test logic, requires minimal boilerplate for new types (~80 lines per new type), and introduces zero runtime overhead. The cost is moderate macro complexity in the templates, mitigated by the fact that the templates are mechanically derived from the existing, well-understood test code.
