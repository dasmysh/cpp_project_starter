# @READABLE_NAME
@DESCRIPTION
- Clone repository with submodules recursive:

  ```git clone --recurse-submodules -j8 https://github.com/dasmysh/@PROJECT_NAME.git ```
  ```cd @PROJECT_NAME```

- Start CMake from this folder with:

  ```cmake -S . --preset=default```

- CMake GUI can be used after the first configure step is done with the correct toolchain file.
