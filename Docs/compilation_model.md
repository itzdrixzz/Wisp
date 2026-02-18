# Wisp Compilation Model (v0.0.1)

This document describes how Wisp source files are transpiled to C++

## Module to C++ Mapping

Each `.wisp` file represents one module.

Example:
- `src/math.wisp`

Will Generate:
- `build/math.hpp`
- `build/math.cpp`

All Module declarations inside a Wisp file are placed inside a C++ namespace matching

Example (`math.wisp`):

```wisp
function add(a: Number, b: Number): Number {
    return a + b;
}
```

Transpiles to C++:

```CPP
// build/math.hpp
namespace math {
    double add(double a, double b);
}

// build/math.cpp
#include "math.hpp"

namespace math {
    double add(double a, double b){
        return a + b;
    }
}
```

## Import Behavior

```wisp
import math;
```

compiler behavior:
1. Resolves to ``` src/math.wisp ```
2. Generates ``` build/math.hpp ``` and ``` build/math.cpp ```
3. Adds ``` #include "math.hpp" ``` to the generated entry file’s C++ output (ex: ``` build/main.cpp ```)

## Member Access Mapping

Wisp namespaced access:
```wisp
math.add(4, 5);
```

Transpiles to C++
```CPP
math::add(4, 5);
```

## Entry File Behavior

The file containing: 
```wisp
app main() {...}
```

Generates:
```CPP
int main() {...}
```
