Data structures
=====
A small collection of frequently used data structures implemented in c.

How to add to a repo
----
### 1. Add as a git submodule
To add this repo as a submodule un libs/data-structures run:

```bash
git submodule add https://github.com/janKpe/data-structures.git libs/data-structures
git commit -m "Add data-structures submodule"
```

Note that if you now clone the repo, you have to either use the `--recurse-submodules` flag with git clone:
```bash
git clone --recurse-submodules <url to your repo>
```
or, if the repo is already cloned:  
```bash
git submodule update --init --recursive
```

### 2. Integrate into CMake
Add these lines to your CMakeLists.txt

```Cmake
# Pull in the submodule
add_subdirectory(libs/data-structures)

# Link it to your target
target_link_libraries(your-target data-structures)
```

### 3. Use
Now you can just include the headers:

```c
#include "hashmap/hashmap.h"
```
