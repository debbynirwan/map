#include "map.hpp"
#include <string>

int main(int argc, char** argv)
{
    Map<int, int> map;
    for (int i = 1; i < 100000000; i++)
        map.Insert(i, 1);
    // map.SaveTree("red-black-tree-" + std::to_string(j - 1) + ".dot");

    return 0;
}
