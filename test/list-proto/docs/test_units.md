# Singly Linked-List — Test Units

Legend: ✅ done, ⬜ todo

---

## Init / Deinit

- ✅ `test_simp_init` — list is non-NULL, head is NULL, size is 0

---

## Push (insert at head)

- ✅ `test_insert_head_seq` — push 1..N sequentially, verify reverse order at head
- ✅ `test_insert_head_random` — push N random values, verify same order at head

---

## Pop (remove from head)

- ✅ `test_pop` — composite test unit
  - ~~⬜ pop on empty list~~ *!!! it's ***caller's*** responsibility to check whether it's valid to pop!!!*
  - ✅ push 1, pop 1 → list empty, head NULL, size 0 (`test_pop_once`)
  - ✅ push N, pop N one-by-one → verify head and size after each pop (`test_pop_seq`)
  - ✅ push N, pop all → head NULL (`test_pop_all`)
  - ✅ push‑pop interleaved — push, pop half, push more, pop all (`test_pop_interleaved`)
  - ✅ rebuild after drain — pop all to empty, then push again (`test_pop_rebuild`)
  - ✅ pop after insert — build list via `list__i_insert`, then pop and verify (`test_pop_after_insert`)

---

## Insert (at arbitrary position)

- ✅ `test_insert` — composite test unit
  - ✅ insert at head (`pos = &list->head`) — equivalent to push
  - ✅ insert at tail (`test_insert_tail`)
    - ✅ insert n = 1 — append single element, verify new tail‑>next is NULL
    - ✅ insert n > 1 — append block, verify source order and tail‑>next is NULL
    - ✅ insert n = 0 (no‑op) — list unchanged, returns 0
  - ✅ insert in middle
    - ✅ insert 1 element at exact midpoint (`test_insert_mid_once`)
    - ✅ insert 1 element at position 1, right after head (`test_insert_mid_pos1`)
    - ✅ insert 1 element at position size−1, right before tail (`test_insert_mid_postail`)
    - ✅ insert into 2‑node list, only one middle position (`test_insert_mid_2node`)
  - ✅ insert n > 1 elements in one call
    - ✅ insert n > 1 at midpoint — verify block order matches source array (`test_insert_mid_multi`)
    - ✅ insert n > 1 right after head, position 1 (`test_insert_mid_multi_pos1`)
    - ✅ insert n > 1 right before tail, position size−1 (`test_insert_mid_multi_postail`)
    - ✅ insert n = 0, no‑op — list unchanged, returns 0 (`test_insert_mid_n0`)
  - ✅ insert into empty list (`test_insert_empty`)
    - ✅ insert n = 1 — head points to new node, size = 1
    - ✅ insert n > 1 — block appears in source order, tail‑>next is NULL
    - ✅ insert n = 0 (no‑op) — list stays empty, head NULL, size 0, returns 0
  - ⬜ malloc failure: list reverts to pre‑call state
    - ⬜ **prerequisite:** add malloc‑failure injection mechanism (no hook exists yet)
    - ⬜ n = 1, first malloc fails — returns non‑zero, size unchanged, list content intact
    - ⬜ n > 1, malloc fails mid‑block — partial nodes cleaned up, list = pre‑call state
    - ⬜ insert position restored on failure — *pos points to original next node

---

## Erase

- ✅ `test_erase` — composite test unit
  - ✅ erase 1 from head
  - ✅ erase 1 from tail
  - ✅ erase 1 from middle
  - ✅ erase n elements (n > 1)
  - ✅ erase all (n = size) → empty list
  - ✅ erase 0 elements (no-op)

---

## Find

- ✅ `test_find` — composite test unit
  - ✅ find value at head (`test_find_hit`)
  - ✅ find value at middle (`test_find_hit`)
  - ✅ find value at tail (`test_find_hit`)
  - ✅ find non-existing value → NULL (`test_find_miss`)
  - ✅ find in empty list → NULL (`test_find_empty`)
  - ✅ find in single‑element list — head == tail (`test_find_hit`)
  - ✅ find with NULL comparator — built‑in memcmp fallback (`test_find_null_cmp`)
  - ✅ find with custom comparator (`test_find_custom_cmp`)
  - ✅ find when multiple nodes match → first occurrence returned (`test_find_dupes`)
  - ✅ resume search after match → find second occurrence (`test_find_dupes`)
  - ✅ find from non‑head starting position — earlier nodes skipped (`test_find_start_at`)

---

## Size invariant

- ✅ `test_size` — composite test unit
  - ✅ per-operation baseline
    - ✅ init → size = 0
    - ✅ push → size += 1
    - ✅ pop → size -= 1
    - ✅ insert n → size += n
    - ✅ erase n → size -= n
  - ✅ no-op preservation
    - ✅ insert n = 0 → size unchanged, returns 0
    - ✅ erase n = 0 → size unchanged
  - ✅ size-structure consistency invariant
    - ✅ `size == 0` ⇔ `head == NULL` (check both directions)
    - ✅ `size > 0` ⇔ `head != NULL`
    - ✅ manual node-count by traversal matches `size` field
  - ✅ rebuild: pop all to empty, push again, verify size recovers correctly
  - ✅ size after mixed-operation sequence — push + insert + erase + pop interleaved with known expected count at each step
  - ⬜ size unchanged on failed insert (requires malloc-failure injection)
  - ⬜ size unchanged on failed push (requires malloc-failure injection)

---

## Stress

- ✅ `test_stress` — composite test unit
  - ✅ large-N push / pop cycle — verify head and size invariants at scale
  - ✅ random mixed-operation sequence — push, pop, insert, erase, find interleaved with size and content invariant checks after every step; use seeded RNG for reproducibility
  - ✅ drain-and-refill cycles — fill to N, pop all to empty, repeat many times; validates no corruption or leak accumulates across full-drain boundaries
  - ✅ insert/erase churn — repeatedly insert and erase at randomly chosen middle positions on a mid-sized list; stresses pointer rewiring away from head/tail edges
  - ✅ large-N find — find existing and non-existing values on a large list; stresses traversal correctness at scale (read-only path)
  - ✅ degenerate access patterns — rapid-fire operations always targeting the same edge (head-only push/pop, tail-only insert/erase, position-1-only insert/erase); catches boundary pointer bugs that only surface under sustained repetition
  - ✅ allocate-deallocate stress — allocate N nodes, deallocate all, repeat many times; verifies no memory leaks and that allocator pressure does not corrupt metadata
