/*
 * Copyright 2023 Debby Nirwan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
