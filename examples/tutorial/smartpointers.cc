#include <iostream>
#include <memory>

int
main()
{
    std::unique_ptr<int> u1(new int(1));
    std::unique_ptr<int> u2(new int(2));
    std::unique_ptr<int> u3 = std::move(u1); // moves the pointer from u1 to u3
    // std::unique_ptr<int> u4 = u2; // error 'cuz assignment operator doesn't work on unique
    // pointers

    if (u1)
    {
        // will not be printed as u1 is null
        std::cout << *u1 << std::endl;
    }
    if (u2)
    {
        std::cout << *u2 << std::endl;
    }
    if (u3)
    {
        std::cout << *u3 << std::endl;
    }

    return 0;
}