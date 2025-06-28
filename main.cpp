#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>

#include "ptr_memory.h"
#include "weak_ptr.h"
#include "shared_ptr.h"
#include "unique_ptr.h"



int main()
{
    std::cout << "\n--- UNIQUE_PTR TESTS ---\n";

    // Test 1: Basic creation using make_unique
    unique_ptr<int> unq_ptr = std::move(unique_ptr<int>::make_unique());
    std::cout << "Unique pointer created using make_unique.\n";

    // Test 2: Allocate array
    unique_ptr<int> arr_ptr(5); // allocating array of 5 ints
    arr_ptr[0] = 10;
    arr_ptr[1] = 20;
    arr_ptr[2] = 30;
    std::cout << "arr_ptr[1] = " << arr_ptr[1] << "\n";

    // Test 3: Reset and access after reset
    arr_ptr.reset();
    try {
        std::cout << "arr_ptr[1] after reset = " << arr_ptr[1] << "\n"; // Should throw
    } catch (const std::exception& ex) {
        std::cout << "Caught exception (expected): " << ex.what() << "\n";
    }

    std::cout << "\n--- SHARED_PTR TESTS ---\n";

    // Test 4: make_shared
    
    shared_ptr<int> sp1 = shared_ptr<int>::make_shared();
    std::cout << "Shared pointer created using make_shared. Ref count: " << sp1.count() << "\n";
    // Test 5: Copy shared_ptr
    shared_ptr<int> sp2 = sp1;
    std::cout << "Copied shared_ptr. Ref count: " << sp1.count() << " (should be 2)\n";

    // Test 6: Assignment operator
    shared_ptr<int> sp3(3); // Allocate 3 ints
    sp3 = sp1;
    std::cout << "Assigned shared_ptr. Ref count: " << sp1.count() << " (should be 3)\n";

    // Test 7: Reset shared_ptr
    sp2.reset();
    std::cout << "After sp2.reset(). Ref count: " << sp1.count() << " (should be 2)\n";
    sp3.reset();
    // Test 8: Destructor test (sp3 goes out of scope)
    {
        shared_ptr<int> temp = sp1;
        std::cout << "Inside new scope. Ref count: " << sp1.count() << "\n";
    }
    std::cout << "After temp goes out of scope. Ref count: " << sp1.count() << "\n";

    std::cout << "\n--- WEAK_PTR TESTS ---\n";

    // Test 9: Weak pointer creation
    weak_ptr<int> wp1 = sp1;
    std::cout << "Weak pointer created from sp1.\n";

    // Test 10: Operator-> and operator* on weak_ptr
    try {
        *sp1 = 42;
        std::cout << "sp1 value: " << *sp1 << "\n";
        std::cout << "wp1 value: " << *wp1 << "\n"; // Should match sp1
    } catch (const std::exception& ex) {
        std::cout << "Exception accessing weak_ptr: " << ex.what() << "\n";
    }

    // Test 11: Reset shared_ptr and access weak_ptr
    sp1.reset();
    try {
        std::cout << "wp1 value after sp1 reset: " << *wp1 << "\n"; // Likely undefined or exception
    } catch (const std::exception& ex) {
        std::cout << "Expected exception from weak_ptr after shared_ptr reset: " << ex.what() << "\n";
    }

    std::cout << "\n--- EDGE CASES ---\n";

    // Test 12: shared_ptr self-assignment
    shared_ptr<int> sp_self(2);
    sp_self = sp_self; // Should not cause error
    std::cout << "Self-assignment done. Ref count: " << sp_self.count() << "\n";

    // Test 13: Weak pointer to deleted memory
    shared_ptr<int> sp_temp(1);
    weak_ptr<int> wp_temp = sp_temp;
    sp_temp.reset(); // Should delete memory
    try {
        std::cout << "Access wp_temp: " << *wp_temp << "\n"; // Should fail
    } catch (const std::exception& ex) {
        std::cout << "Caught exception after shared_ptr reset: " << ex.what() << "\n";
    }

    return 0;
}
