#include "AppClass.h"

#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "Hello, world" << std::endl;
    AppClass a;
    a.set_attr(2);
    std::cout << "a.attr() = " << a.attr() << std::endl;
    return 0;
}
