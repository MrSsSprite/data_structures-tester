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

- ⬜ `test_pop` — composite test unit
  - ⬜ pop on empty list
  - ⬜ push 1, pop 1 → list empty, head NULL, size 0
  - ⬜ push N, pop N one-by-one → verify head and size after each pop
  - ⬜ push N, pop all → head NULL

---

## Insert (at arbitrary position)

- ⬜ `test_insert` — composite test unit
  - ✅ insert at head (`pos = &list->head`) — equivalent to push
  - ⬜ insert at tail (`pos` after last node)
  - ⬜ insert in middle
    - ✅ insert 1 element at exact midpoint (`test_insert_mid_once`)
    - ✅ insert 1 element at position 1, right after head (`test_insert_mid_pos1`)
    - ✅ insert 1 element at position size−1, right before tail (`test_insert_mid_postail`)
    - ✅ insert into 2‑node list, only one middle position (`test_insert_mid_2node`)
  - ⬜ insert n > 1 elements in one call
    - ✅ insert n > 1 at midpoint — verify block order matches source array (`test_insert_mid_multi`)
    - ✅ insert n > 1 right after head, position 1 (`test_insert_mid_multi_pos1`)
    - ✅ insert n > 1 right before tail, position size−1 (`test_insert_mid_multi_postail`)
    - ✅ insert n = 0, no‑op — list unchanged, returns 0 (`test_insert_mid_n0`)
  - ✅ insert into empty list (`test_insert_empty`)
    - ✅ insert n = 1 — head points to new node, size = 1
    - ✅ insert n > 1 — block appears in source order, tail‑>next is NULL
    - ✅ insert n = 0 (no‑op) — list stays empty, head NULL, size 0, returns 0
  - ⬜ malloc failure: list reverts to pre-call state

---

## Erase

- ⬜ `test_erase` — composite test unit
  - ⬜ erase 1 from head
  - ⬜ erase 1 from tail
  - ⬜ erase 1 from middle
  - ⬜ erase n elements (n > 1)
  - ⬜ erase all (n = size) → empty list
  - ⬜ erase 0 elements (no-op)

---

## Find

- ⬜ `test_find` — composite test unit
  - ⬜ find value at head / middle / tail
  - ⬜ find non-existing value → NULL
  - ⬜ find in empty list → NULL
  - ⬜ find with custom comparator
  - ⬜ find when multiple nodes match → first occurrence returned

---

## Size invariant

- ⬜ `test_size` — composite test unit
  - ⬜ size after push, pop, insert, erase
  - ⬜ size after mixed-operation sequence (push + insert + erase + pop interleaved)

---

## Stress

- ⬜ `test_stress` — composite test unit
  - ⬜ large-N push / pop cycle
  - ⬜ random mixed-operation sequence with size and content invariant checks
