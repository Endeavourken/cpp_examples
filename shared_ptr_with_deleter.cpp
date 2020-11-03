#include <memory>
#include <iostream>

void sharedPtrWithDeleter() {
    auto mem = new int[10];
    std::cout << (void*)mem << std::endl;
    std::shared_ptr<int> s_ptr(mem, [](int *p) {
            std::cout << "delete array " << (void*)p << std::endl;
            delete []p; 
            });
}
