#include <cstring>
#include <iostream>

class ClassWithArray {
    public:
    char a[10];
};

void func() {
    ClassWithArray a;
    strcpy(a.a, "Hello");
    ClassWithArray b(a);
    std::cout << b.a << std::endl;
    return;
}
