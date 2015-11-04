#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

extern int foo();
int main()
{
    std::string s = "lnkub";
    std::cout << s << std::endl;
    std::vector<int> a;
    for (int i = 0; i < 100; ++i)
         a.push_back(i);
    for (auto x : a)
	std::cout << x << " ";
    //std::unique_ptr<std::string> ptr = std::make_unique<std::string>("jbkubkb");

    return foo();
}
