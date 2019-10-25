#include <iostream>

struct A
{
    static std::string str;
};

std::string A::str = "1";

int main()
{
    std::cout << "A1 : " << A::str << "\n";
    // A::str = 2;
    A::str = "PABLO\n";
    std::cout << "A2 : " << A::str << "\n";
}
