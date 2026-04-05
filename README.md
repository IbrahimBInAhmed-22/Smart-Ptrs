# 🧠 Smart Pointer Library & Multi-Threaded Matrix Processor
> A from-scratch C++ implementation of `unique_ptr`, `shared_ptr`, and `weak_ptr` — with reference counting, a custom memory control block, and a POSIX-threaded matrix processing engine.

---

## 📁 Table of Contents

1. [Project Overview](#1-project-overview)
2. [File Structure](#2-file-structure)
3. [Architecture Diagram](#3-architecture-diagram)
4. [Module 1 — Custom Smart Pointer Library](#4-module-1--custom-smart-pointer-library)
   - [memory\<T\> — The Control Block](#41-memoryt--the-control-block)
   - [shared\_ptr\<T\>](#42-shared_ptrt)
   - [weak\_ptr\<T\>](#43-weak_ptrt)
   - [unique\_ptr\<T\>](#44-unique_ptrt)
5. [Module 2 — Multi-Threaded Matrix Processor](#5-module-2--multi-threaded-matrix-processor)
   - [Operation Hierarchy](#51-operation-hierarchy)
   - [threadData & threadManager](#52-threaddata--threadmanager)
   - [MatrixDemo](#53-matrixdemo)
6. [How Ownership Works — Analogy](#6-how-ownership-works--analogy)
7. [Demo Walkthrough (demoSmartPtr)](#7-demo-walkthrough-demosmartsptr)
8. [Known Issues & Bugs](#8-known-issues--bugs)
9. [Build Instructions](#9-build-instructions)
10. [Key Concepts Tested](#10-key-concepts-tested)

---

## 1. Project Overview

This project is a **dual-module C++ systems programming assignment** that proves mastery of:

- **Manual memory management** via custom smart pointer types built without any `<memory>` standard-library usage
- **Object-oriented design** through polymorphism, operator overloading, templates, and the Rule of Three/Five
- **Concurrency** via POSIX `pthread` threads applied to a matrix operation pipeline

The entry point (`main.cpp`) presents a console menu that lets you switch between:
- `1` → Smart Pointer Demo (`demoSmartPtrs`)
- `2` → Matrix Threading Demo (`MatrixDemo`)
- `-1` → Exit

---

## 2. File Structure

```
project/
│
├── main.cpp                  # Entry point — menu-driven launcher
│
├── ptr_memory.h              # memory<T> control block — declaration
├── ptr_memory.cpp            # memory<T> control block — implementation
│
├── shared_ptr.cpp            # shared_ptr<T> — class declaration (misnamed .cpp, acts as .h)
├── shared_ptr.tpp            # shared_ptr<T> — full template implementation
│
├── weak_ptr.h                # weak_ptr<T> — full declaration + implementation
│
├── unique_ptr.cpp            # unique_ptr<T> — full declaration + implementation (misnamed .cpp)
│
├── demoSmartPtr.cpp          # demoSmartPtrs class declaration (misnamed .cpp, acts as .h)
├── demoSmartPtr.tpp          # demoSmartPtrs::demo() — all test cases
│
├── thread.cpp                # Operation hierarchy, matrix, threadManager, MatrixDemo
└── main                      # Pre-compiled ELF binary (Linux x86-64)
```

> **Note on naming:** Several `.cpp` files in this project act as headers (they are `#include`-d by other files and contain `#ifndef` guards). This is intentional for the template system but is non-standard — normally `.h` or `.hpp` extensions are used.

---

## 3. Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                          main.cpp                           │
│         choice == 1               choice == 2               │
└────────────┬──────────────────────────────┬─────────────────┘
             │                              │
             ▼                              ▼
   ┌──────────────────┐         ┌─────────────────────────┐
   │  demoSmartPtrs   │         │      MatrixDemo          │
   │  (demoSmartPtr   │         │  (thread.cpp / .h)       │
   │   .cpp / .tpp)   │         └────────────┬────────────┘
   └────────┬─────────┘                      │
            │                       ┌────────┴────────┐
            │                       │                 │
            ▼                       ▼                 ▼
  ┌──────────────────┐      ┌──────────────┐  ┌────────────────┐
  │   unique_ptr<T>  │      │   matrix     │  │  Operation     │
  │   (exclusive     │      │  (data +     │  │  (abstract)    │
  │    ownership)    │      │   print)     │  │                │
  └──────────────────┘      └──────────────┘  └──────┬─────────┘
  ┌──────────────────┐                               │
  │   shared_ptr<T>  │◄────┐              ┌──────────┼──────────┐
  │   (shared        │     │              ▼          ▼          ▼
  │    ownership)    │     │         addOp  subtractOp  multiplyOp
  └────────┬─────────┘     │                         squareOp
           │               │
           ▼               │      ┌──────────────────────┐
  ┌──────────────────┐     │      │    threadManager     │
  │  memory<T>       │     │      │  (static methods)    │
  │  (control block) │     │      │  - threadFunction()  │
  │  - ref count     │     │      │  - processMatrix()   │
  │  - shared list   │     │      └──────────────────────┘
  │  - weak list     │     │
  └──────────────────┘     │
  ┌──────────────────┐     │
  │   weak_ptr<T>    │─────┘
  │  (non-owning     │
  │   observer)      │
  └──────────────────┘
```

---

## 4. Module 1 — Custom Smart Pointer Library

### 4.1 `memory<T>` — The Control Block

**File:** `ptr_memory.h` / `ptr_memory.cpp`

#### What it is
`memory<T>` is the **shared bookkeeper** for one heap allocation. Think of it like a **hotel front desk**: it keeps a guest register of everyone checked into the same room (`shared_ptr`s), plus a visitor log of non-guests who may visit (`weak_ptr`s). When the last guest checks out, the room is demolished.

#### Data Members

| Member | Type | Purpose |
|---|---|---|
| `reference_count` | `int` | Number of live `shared_ptr` owners |
| `shared_memory` | `vector<shared_ptr<T>*>` | Raw pointers to all live `shared_ptr` objects |
| `weak_memory` | `vector<weak_ptr<T>*>` | Raw pointers to all live `weak_ptr` objects |

#### Methods

| Method | Behaviour |
|---|---|
| `assign(shared_ptr<T>&)` | Push pointer into `shared_memory`, increment `reference_count` |
| `free(shared_ptr<T>&)` | Remove from `shared_memory`, decrement `reference_count` |
| `assign(weak_ptr<T>&)` | Push pointer into `weak_memory` — **no count change** |
| `free(weak_ptr<T>&)` | Erase from `weak_memory` |
| `free()` | Full cleanup: null every `weak_ptr` via its `reset()`, clear both vectors, zero count |
| `count()` | Return `reference_count` |

---

### 4.2 `shared_ptr<T>`

**Files:** `shared_ptr.cpp` (declaration), `shared_ptr.tpp` (implementation)

#### What it is
Shared ownership — like **co-owners of a bank account**. Any number of `shared_ptr` instances can point to the same allocation. The memory is freed only when the last co-owner walks away.

#### Private Members

```cpp
T*         _ptr;   // Raw pointer to the heap allocation
memory<T>* block;  // Pointer to the shared control block
```

#### Key Operations

| Operation | Behaviour |
|---|---|
| `shared_ptr()` | Null init — `_ptr = nullptr`, `block = nullptr` |
| `shared_ptr(int bytes)` | `new T[bytes]`, create `memory<T>`, register self |
| `shared_ptr(const shared_ptr<T>& p)` | Copy `_ptr` and `block`, register self with block → count++ |
| `operator=(const shared_ptr<T>& p)` | Free self from old block (delete if count → 0), then join new block |
| `count()` | Delegates to `block->count()` |
| `operator*()` | Dereference; throws `runtime_error` if `_ptr == nullptr` |
| `operator->()` | Returns `_ptr` |
| `operator[](int)` | Index access with bounds check |
| `operator==(...)` | Compares both `_ptr` and `block` |
| `make_shared()` | Static factory: allocates 1 element, returns a `shared_ptr<T>` |
| `reset()` | Detach from block; if count → 0, deallocate block and data |
| `~shared_ptr()` | Same as reset — decrements count, frees if last owner |

> **`friend class weak_ptr<T>`** — grants `weak_ptr` access to `_ptr` and `block` directly so it can hook into the same control block without going through the public API.

---

### 4.3 `weak_ptr<T>`

**File:** `weak_ptr.h`

#### What it is
A **non-owning observer** — like a **visitor badge** that lets you enter the building but gives you no authority over it. A `weak_ptr` does not contribute to the reference count. If the last `shared_ptr` dies, the `weak_ptr` is nulled automatically (via `memory<T>::free()`).

#### Private Members

```cpp
T*         _ptr;   // Raw pointer (same as shared_ptr's)
memory<T>* block;  // Same control block (non-owning reference)
```

#### Key Operations

| Operation | Behaviour |
|---|---|
| `weak_ptr(const shared_ptr<T>& p)` | Copy `_ptr` and `block`; register self with block as a weak observer |
| `operator=(const shared_ptr<T>& p)` | Same as above — re-register |
| `operator*()` | Dereference; throws `out_of_range` if `_ptr == nullptr` or `block == nullptr` |
| `operator->()` | Returns `_ptr` |
| `operator[](int)` | Index access with null check |
| `reset()` | Null both `_ptr` and `block` (called automatically by `memory<T>::free()`) |
| `operator==(weak_ptr<T>& p)` | Compares both `_ptr` and `block` |

---

### 4.4 `unique_ptr<T>`

**File:** `unique_ptr.cpp`

#### What it is
**Exclusive ownership** — like a **physical house key**: there is exactly one. You cannot photocopy it (copy is `= delete`), but you can hand it to someone else (move semantics).

#### Private Members

```cpp
T* _ptr;   // The one and only raw pointer
```

#### Key Operations

| Operation | Behaviour |
|---|---|
| `unique_ptr()` | `_ptr = nullptr` |
| `unique_ptr(int bytes)` | `new T[bytes]` |
| `unique_ptr(const unique_ptr<T>&)` | **`= delete`** — copying is a compile-time error |
| `operator=(const unique_ptr<T>&)` | **`= delete`** |
| `unique_ptr(unique_ptr<T>&&)` | Move: take ownership, leave source null |
| `operator=(unique_ptr<T>&&)` | Move-assign: same semantics |
| `operator*()` | Dereference; throws `out_of_range` if null |
| `operator->()` | Returns `_ptr` |
| `operator[](int)` | Index access with null check |
| `reset()` | `delete[] _ptr; _ptr = nullptr` |
| `make_unique()` | Static factory — returns a null `unique_ptr<T>` |
| `~unique_ptr()` | `delete[] _ptr` if not null |

---

## 5. Module 2 — Multi-Threaded Matrix Processor

**File:** `thread.cpp`

This module applies different arithmetic operations to different **rows** of a matrix simultaneously, each in its own POSIX thread.

### 5.1 Operation Hierarchy

```
Operation  (abstract base)
    ├── addOperation       — adds a constant to every element in a row
    ├── subtractOperation  — subtracts a constant from every element
    ├── multiplyOperation  — multiplies every element by a constant
    └── squareOperation    — squares every element (value × value)
```

Each subclass overrides:
- `void execute(std::vector<int>& row)` — modifies the row in-place
- `const std::string getName() const` — returns a human-readable label for logging

All operations are passed around as `std::unique_ptr<Operation>` to enforce single ownership while still allowing polymorphic dispatch.

---

### 5.2 `threadData` & `threadManager`

#### `threadData` (struct)
The data packet handed to each thread. Contains:

```cpp
std::vector<std::vector<int>>* mat;       // pointer into the matrix's raw data
int row_index;                            // which row this thread owns
std::unique_ptr<Operation> operation;     // the operation to apply
```

The `unique_ptr<Operation>` ensures the operation object lives exactly as long as the thread data, then is automatically destroyed when `threadData` is destroyed at thread end.

#### `threadManager` (class)

| Method | What it does |
|---|---|
| `static void* threadFunction(void* arg)` | Receives a `threadData*`, wraps it in a `unique_ptr` (so it auto-deletes on exit), calls `operation->execute()` on the target row |
| `static void processMatrix(matrix&, vector<unique_ptr<Operation>>&)` | Creates one `pthread_t` per operation, launches each thread via `pthread_create`, then `pthread_join`s all of them |

**Ownership transfer pattern:** `data.release()` is called before `pthread_create` so the raw pointer crosses the thread boundary safely, and the receiving thread takes ownership by wrapping it back in a `unique_ptr`.

---

### 5.3 `MatrixDemo`

```cpp
MatrixDemo demo(5, 5);
demo.runDemo();
```

1. Constructs a 5×5 matrix filled with sequential values (row `i`, col `j` → `i*cols + (j+1)`)
2. Prints the initial matrix
3. Builds a vector of 5 operations:
   - Row 0: `addOperation(5)`
   - Row 1: `subtractOperation(1)`
   - Row 2: `multiplyOperation(2)`
   - Row 3: `squareOperation`
   - Row 4: `squareOperation`
4. Launches all 5 threads in parallel via `threadManager::processMatrix`
5. Waits for all threads to finish, then prints the result

---

## 6. How Ownership Works — Analogy

| Smart Pointer | Real-world Analogy | Key Property |
|---|---|---|
| `unique_ptr<T>` | A physical house key — only one exists | Non-copyable; moveable |
| `shared_ptr<T>` | Co-owners of a bank account | Ref-counted; last one frees memory |
| `weak_ptr<T>` | A visitor pass to the building | Non-owning; nulled when owners leave |
| `memory<T>` | The building's front desk | Tracks all owners and visitors |

When the last `shared_ptr` calls `reset()` or goes out of scope:
1. It calls `block->free(*this)` → count becomes 0
2. It calls `block->free()` → all weak_ptrs are nulled
3. It `delete`s the control block and the raw heap data

---

## 7. Demo Walkthrough (`demoSmartPtrs`)

The `demoSmartPtrs::demo()` method (in `demoSmartPtr.tpp`) runs 13 labelled tests:

| Test | What is tested |
|---|---|
| 1 | `make_unique()` factory — creates a null unique_ptr |
| 2 | Array allocation and index access via `operator[]` |
| 3 | `reset()` then access — expects `out_of_range` exception |
| 4 | `make_shared()` factory — ref count should be 1 |
| 5 | Copy constructor — ref count should become 2 |
| 6 | Assignment operator with an existing shared_ptr — count becomes 3 |
| 7 | `reset()` on one copy — count returns to 2 |
| 8 | Scope-exit destructor — count returns to 1 after temp leaves scope |
| 9 | `weak_ptr` construction from `shared_ptr` |
| 10 | `operator*` and value access on both shared and weak pointer |
| 11 | Access weak_ptr after `shared_ptr::reset()` — expects exception |
| 12 | Self-assignment edge case — must not corrupt state |
| 13 | Weak pointer to still-live memory — demonstrates safe access path |

---

## 8. Known Issues & Bugs

The following issues exist in the current codebase and are worth noting for review:

### `unique_ptr`
- **`operator==` uses `=` instead of `==`** (`return _ptr = p._ptr;`) — this assigns instead of comparing; always returns true-ish.
- **Move constructor marks `p` as `const`** (`const unique_ptr<T>&&`) — `std::move` on a const rvalue won't actually null the source pointer.
- **`operator=(unique_ptr<T>&&)` missing return** on the non-self-assignment path in some code paths.

### `shared_ptr`
- **`operator[]` dereferences before null check** — `_ptr[index]` is accessed before verifying `_ptr != nullptr`.

### `main.cpp`
- **`else if(choice = -1)`** — uses assignment `=` instead of comparison `==`, so this branch always evaluates as true (non-zero), meaning it always exits even for invalid choices.

### `thread.cpp`
- **`squareOperation` constructor takes an unused `int v` parameter** — the value is silently ignored; squaring uses `val * val` regardless.
- **`std::endl` without `std::` prefix** in `threadFunction` — relies on `using namespace std` being in scope, which is not explicitly present in this file.

---

## 9. Build Instructions

### Prerequisites
- GCC or Clang with C++14 or later (`-std=c++14`)
- POSIX thread library (`-lpthread`)
- Linux / macOS (POSIX `pthread` required)

### Compile

```bash
g++ -std=c++14 -pthread \
    main.cpp \
    ptr_memory.cpp \
    demoSmartPtr.cpp \
    thread.cpp \
    -o smart_ptr_demo
```

> Because several `.cpp` files act as headers and are `#include`-d via `.tpp` chains, do **not** compile `shared_ptr.tpp`, `demoSmartPtr.tpp`, or `unique_ptr.cpp` directly as translation units.

### Run

```bash
./smart_ptr_demo
```

You will be prompted:
```
Enter:
1 for Smart Pointer Demo class or
2 for Matrix operations using threading:
-1 to exit.
```

---

## 10. Key Concepts Tested

| Concept | Where it appears |
|---|---|
| Template classes | `memory<T>`, `shared_ptr<T>`, `weak_ptr<T>`, `unique_ptr<T>` |
| Reference counting | `memory<T>` — `reference_count` + `assign` / `free` |
| Rule of Three (copy ctor, copy assign, destructor) | `shared_ptr<T>` |
| Move semantics (Rule of Five) | `unique_ptr<T>` |
| Deleted functions | `unique_ptr` copy ctor + copy assign |
| Operator overloading | `*`, `->`, `[]`, `==` on all pointer types |
| `friend` keyword | `shared_ptr` grants access to `weak_ptr` |
| Abstract base classes + polymorphism | `Operation` → subclasses |
| POSIX threading | `pthread_create`, `pthread_join` in `threadManager` |
| `unique_ptr` for thread-safe ownership transfer | `data.release()` → `pthread_create` → re-wrap in thread |
| Exception handling | `std::out_of_range`, `std::runtime_error` across all pointer types |

---

*README generated for academic review. All smart pointer implementations are custom — no `<memory>` standard-library types are used in the pointer modules.*
