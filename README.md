# map
Simple implementation of Map in C++. It's mainly intended to show how maps are implemented by libraries like STL.

# How to build and install the library
It is a header-only library. You can copy it directly into your repository or build and install it as follows:
```cmake
> mkdir build && cd build
> cmake -DCMAKE_BUILD_TYPE=Release ..
> make
> make install
```

To build the example you have to set the BUILD_EXAMPLE option ON. By default it is not built.
```cmake
> cmake -DBUILD_EXAMPLE=ON ..
```

To build python bindings you have to set the BUILD_PYTHON_BINDINGS option ON. By default it is not built.
```cmake
> cmake -DBUILD_PYTHON_BINDINGS=ON ..
```

# Examples
To use the library simply include and use it. 
```cpp
#include "map.hpp"

Map<int, int> map;

map.Insert(1, 5);
auto val = map.At(1);
map.Remove(1);

```
see [example](example/main.cpp) for more.

If you build and install python bindings, you can use it too.
```python
import map_module

map = map_module.Map()
map.insert(1, 5)
val = map.at(1)
map.remove(1)
```

# Documentation
If you're interested in understanding the details you can read my blog post [here](https://debby-nirwan.medium.com/how-is-c-map-implemented-8cc10c93684a).

# Issues
Please report issues if you found bugs or raise a Pull Request.
