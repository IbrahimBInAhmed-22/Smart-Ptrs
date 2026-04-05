# Custom C++ Systems Toolkit: Smart Pointers & Multithreading

A comprehensive C++ project demonstrating low-level **Memory Management** (re-implementing standard smart pointers from scratch) and **Parallel Computing** (multithreading mathematical matrices).

---

## 📖 Table of Contents
1. [Introduction (The Big Picture)](#introduction-the-big-picture)
2. [Module 1: Smart Pointers (Memory Management)](#module-1-smart-pointers-memory-management)
    - [Unique Pointer](#1-unique-pointer-unique_ptrh)
    - [Shared Pointer & Memory Block](#2-shared-pointer-shared_ptrh--ptr_memoryh)
    - [Weak Pointer](#3-weak-pointer-weak_ptrh)
3. [Module 2: Multithreaded Matrix Engine](#module-2-multithreaded-matrix-engine-threadh)
    - [The Strategy Pattern](#1-the-strategy-design-pattern)
    - [Parallel Processing](#2-parallel-processing)
4. [How to Run](#-how-to-run)
5. [Technical Concepts Mastered](#-technical-concepts-mastered)

---

## 🌍 Introduction (The Big Picture)

In older programming languages like C, developers had to manually allocate memory (`new`) and manually delete it (`delete`). If a developer forgot to delete it, the computer would run out of RAM—this is called a **Memory Leak**. 

This project solves that by building **Smart Pointers**: Objects that act like "babysitters" for raw data, automatically deleting it when it's no longer needed.

Furthermore, it tackles **Multithreading**—teaching the computer how to use multiple CPU cores to run heavy calculations at the exact same time.

---

## 🧠 Module 1: Smart Pointers (Memory Management)

All pointers in this project use **Templates** (`template <class T>`), meaning they can hold any data type (integers, strings, custom objects, etc.).

### 1. Unique Pointer (`unique_ptr.h`)
*   **The Analogy:** Think of this as a **single-user key**. Only one person can hold the key to a house at any time.
*   **How it works in the code:** 
    - The copy constructor is explicitly deleted (`unique_ptr(const unique_ptr<T>&p) = delete;`). You cannot copy it. 
    - To pass ownership, you must **Move** it using `std::move`.
    - When the `unique_ptr` goes out of scope, its destructor (`~unique_ptr()`) automatically deletes the array. No memory leaks!

### 2. Shared Pointer & Memory Block (`shared_ptr.h` & `ptr_memory.h`)
*   **The Analogy:** Think of a **library book**. Multiple people can read it. The library keeps a counter of how many people have it. The book is only thrown away when the counter hits zero.
*   **How it works in the code:**
    - I implemented a custom `memory` class (known as a Control Block). 
    - It maintains a `reference_count`. 
    - Every time a new `shared_ptr` looks at the data, the count goes up. When a pointer is destroyed, it goes down. When it reaches 0, the data is wiped from the RAM.

### 3. Weak Pointer (`weak_ptr.h`)
*   **The Analogy:** A person checking the library computer to see if a book is on the shelf. They don't check the book out, they just observe it.
*   **How it works in the code:**
    - A `weak_ptr` points to the data but does **not** increase the reference count. 
    - This is vital to prevent **Circular Dependencies** (where two pointers point to each other and get stuck in a loop, never getting deleted).

---

## ⚡ Module 2: Multithreaded Matrix Engine (`thread.h`)

This module takes a mathematical Grid (Matrix) and applies math operations to it using background workers (threads).

### 1. The Strategy Design Pattern
Instead of writing a different function for addition, subtraction, multiplication, and squaring, I used the **Object-Oriented Strategy Pattern**:
- **Base Class:** `Operation`
- **Derived Classes:** `addOperation`, `subtractOperation`, `multiplyOperation`, `squareOperation`.
- **Benefit:** The engine doesn't care *what* the math operation is; it just runs it. This makes the code incredibly easy to expand.

### 2. Parallel Processing
Imagine having a 5-row matrix. Doing the math one row at a time is slow.
- **The Solution:** The `threadManager` creates a unique **POSIX Thread (Pthread)** for every single row.
- **Concurrency:** All rows are calculated at the exact same fraction of a second on separate CPU cores.
- The `pthread_join` command ensures the main computer program pauses and waits for all workers to finish their jobs before printing the final matrix.

---

## 🚀 How To Run

### 🖥️ Compilation
You will need a C++ compiler (like `g++`). Open your terminal and run:

```bash
g++ main.cpp -lpthread -o systems_app
