# Type Expansion Plan — Parameterized List Tests

## 1. Current State

The test suite uses **X-macro type parameterization**: a type-specific header defines 11 macros, and 8 template files use those macros to generate per-type test functions at compile time. Two types are currently instantiated:

| Type | Suffix | Header |
|------|--------|--------|
| `int` | `i` | `test/list/src/type/int.h` |
| `float` | `f` | `test/list/src/type/float.h` |

Each new type requires touching 7 files: type header, shim `.c`, `ds.h`, `ds.c`, `test_units.h`, `main.c`, `Makefile`.

## 2. Target Types

### Phase 1 — Primitive types (low effort, no template changes)

| Type | Suffix | Notes |
|------|--------|-------|
| `double` | `d` | Copy from `float.h`, adjust epsilon to `1e-9`, use `TEST_ASSERT_EQUAL_DOUBLE` |
| `char` | `c` | Integer-like; `LIST_VAL_FROM_IDX` wraps mod 256. Use `TEST_ASSERT_EQUAL_INT` via cast |
| `unsigned int` | `u` | Copy from `int.h`, signed-cast sentinel values in range `UINT_MAX - N` |
| `long` | `l` | Copy from `int.h` |

### Phase 2 — Struct type (requires template fixes)

| Type | Suffix | Notes |
|------|--------|-------|
| `struct Point { int x; int y; }` | `p` | Exercise member-wise equality, compound literals, custom comparator |

## 3. Template Fixes Required for Struct Types

Three templates use operations that are valid for primitives but fail for structs:

### 3.1 `test_erase.temp.h` — identity comparison (`==`)

**Lines 137 and 178** use `==` to skip erased values during iteration:

```c
// Line 137 (test_erase_mid): erasing 1 element at mid position
if (LIST_VAL_FROM_IDX(val) == erased_val) continue;

// Line 178 (test_erase_multi): erasing n=3 elements
if (val == e1 || val == e2 || val == e3) continue;
```

Structs cannot use `==`. This must become a type-provided macro:

```c
#define LIST_VALUE_IDENTITY(a, b) /* … */
// For primitives: (a) == (b)
// For structs:   memcmp(&(a), &(b), sizeof(LIST_TYPE)) == 0
```

Additionally, `test_erase_multi` stores erased values as `size_t` (`e1 = (*pos)->value`), which only works for values where `LIST_VAL_FROM_IDX(i)` is a bijection from `size_t` to `LIST_TYPE`. For structs, the erased values must be stored as `LIST_TYPE` and compared with `LIST_VALUE_IDENTITY`.

### 3.2 `test_stress.temp.h` — negative-sentinel cast

**Lines 156, 228, 379, 419** cast a negative `int` to `LIST_TYPE` to produce a "missing" sentinel:

```c
(LIST_TYPE)(-(int)(i + 2))   // line 156 — find-missing
(LIST_TYPE)(-(int)(i + 1))   // line 228 — churn insert value
(LIST_TYPE)(-(int)(i + 1))   // line 379 — degenerate tail insert
(LIST_TYPE)(-(int)(i + 1))   // line 419 — degenerate position-1 insert
```

This cast is meaningless for structs. Replace with a type-provided macro:

```c
#define LIST_SENTINEL_FROM_IDX(i) /* … */
// For primitives: (LIST_TYPE)(-(int)(i + 1))
// For structs:   a designated "invalid" struct value
```

### 3.3 `test_stress.temp.h` — comparator using `>` / `<`

**Line 41 (`stress_cmp`)** uses relational operators:

```c
static int stress_cmp(const void *a, const void *b)
{
   LIST_TYPE va = *(const LIST_TYPE *)a, vb = *(const LIST_TYPE *)b;
   return (va > vb) - (va < vb);
}
```

Structs don't support `>` / `<`. Replace with a type-provided macro:

```c
#define LIST_CMP_GT(a, b) /* … */
// For primitives: (a) > (b)
// For structs:   total ordering via member comparison
```

### Summary of new macros needed

| Macro | Used in | Primitive definition | Struct definition |
|-------|---------|---------------------|-------------------|
| `LIST_VALUE_IDENTITY(a, b)` | `test_erase.temp.h` | `(a) == (b)` | `memcmp(&(a), &(b), sizeof(LIST_TYPE)) == 0` |
| `LIST_SENTINEL_FROM_IDX(i)` | `test_stress.temp.h` | `(LIST_TYPE)(-(int)(i + 1))` | `(struct Point){-1, -1}` or similar |
| `LIST_CMP_GT(a, b)` | `test_stress.temp.h` | `(a) > (b)` | `(a).x != (b).x ? (a).x > (b).x : (a).y > (b).y` |
| `LIST_CMP_LT(a, b)` | `test_stress.temp.h` | `(a) < (b)` | `(a).x != (b).x ? (a).x < (b).x : (a).y < (b).y` |

**Backward compatibility**: The new macros must be added to existing type headers (`int.h`, `float.h`) as well, with the trivial definitions listed above.

## 4. Per-Type File Checklist

Adding one new type touches these 7 files:

| # | File | Change |
|---|------|--------|
| 1 | `test/list/src/type/<name>.h` | **Create** — define all 15 type macros (11 original + 4 new) |
| 2 | `test/list/src/<name>.c` | **Create** — shim that includes the type header then all 8 templates |
| 3 | `test/list/src/ds.h` | Add `List__declare(T, s)` and `_DS_STRUCT_REVEAL(s)` |
| 4 | `test/list/src/ds.c` | Add `List__define(T, s)` |
| 5 | `test/list/src/test_units.h` | Declare 8 test functions (`test_*__s`) |
| 6 | `test/list/src/main.c` | Register 8 test functions (`RUN_TEST(test_*__s)`) |
| 7 | `test/list/Makefile` | Add `<name>.c` to `INST_SRCS` |

Files 3–6 grow linearly with each type. Consider future macro-generation (see §6).

## 5. Implementation Order

### Step 0 — Add new macros to templates and existing type headers

Before any new type is added, update the templates and existing type headers with the 4 new macros. This is a pure refactor — zero behavioral change for `int` and `float`.

Files changed:
- `test/list/src/type/int.h` — add `LIST_VALUE_IDENTITY`, `LIST_SENTINEL_FROM_IDX`, `LIST_CMP_GT`, `LIST_CMP_LT`
- `test/list/src/type/float.h` — same
- `test/list/src/template/test_erase.temp.h` — `==` → `LIST_VALUE_IDENTITY`, `size_t` → `LIST_TYPE` for erased values
- `test/list/src/template/test_stress.temp.h` — cast → `LIST_SENTINEL_FROM_IDX`, relational ops → `LIST_CMP_GT`/`LIST_CMP_LT`

### Step 1 — Add `double` (suffix `d`)

Copy `float.h` → `double.h`, adjust:
- `LIST_TYPE` → `double`
- `LIST_SUFFIX` → `d`
- Epsilon in `LIST_EQ` → `1e-9`
- `LIST_ASSERT_EQ` → `TEST_ASSERT_EQUAL_DOUBLE`
- `LIST_RAND()` → `(double)rand() / (double)RAND_MAX`
- Sentinel values → `double` literals (`-1.0`, `3.14`, `99.0`, etc.)

Then register in ds.h, ds.c, test_units.h, main.c, Makefile, create shim `double.c`.

### Step 2 — Add `char` (suffix `c`), `unsigned int` (suffix `u`), `long` (suffix `l`)

Same pattern. These are integer-like, so `LIST_EQ` and `LIST_ASSERT_EQ` are straightforward.

### Step 3 — Add `struct Point` (suffix `p`)

This exercises the struct path. The type header must:
- Use compound literals for all value macros
- Define `LIST_VALUE_IDENTITY` with `memcmp` (struct has no padding)
- Define `LIST_CMP_GT`/`LIST_CMP_LT` with member-wise total ordering
- Use `TEST_ASSERT_TRUE(LIST_EQ(a, b))` for `LIST_ASSERT_EQ`

If `Point` has padding, either pack it or accept that `memcmp`-based find (NULL comparator) requires padding-aware comparison. Packing is simpler:

```c
struct __attribute__((packed)) Point { int x; int y; };
```

## 6. Future Automation Opportunities

### 6.1 X-macro registration table

The repetitive registration in `ds.h`, `ds.c`, `test_units.h`, and `main.c` could be reduced with an X-macro table:

```c
// types.def — single source of truth
#define FOR_EACH_LIST_TYPE(X) \
   X(int, i)                  \
   X(float, f)                \
   X(double, d)               \
   /* ... */

// ds.h
#define LIST_DECLARE(T, s) List__declare(T, s);
FOR_EACH_LIST_TYPE(LIST_DECLARE)
#undef LIST_DECLARE

// main.c
#define LIST_RUN_TEST(T, s) RUN_TEST(test_simp_init__##s);
FOR_EACH_LIST_TYPE(LIST_RUN_TEST)
#undef LIST_RUN_TEST
```

This is optional — only worth doing once 4+ types make the repetition noisy.

### 6.2 Template dependency generation

Currently the Makefile uses a wildcard for all templates. A stricter approach would extract `#include` directives from each instantiator `.c` to compute precise per-file template dependencies (via `gcc -MM`). Not needed at current scale.

## 7. Verification

After each new type:
1. `make -C test/list clean && make -C test/list` — clean build succeeds with no warnings
2. `./bin/list` — all tests pass
3. `touch test/list/src/template/simp_init.temp.h && make -C test/list -n` — confirms only instantiator `.o` files rebuild
