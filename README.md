# Mysqlcppcon RAII examples

## Introduction

Mysqlcppcon provides raw owning pointers for objects like the connection instance, statement instance, query result instance.
This code encapsulates the raw pointers in C++ smart pointers using the RAII builtin in the language, so you do not need to remember to use the delete operator.
The idea is to automate the delete when the object goes out-of-scope, making harder to create memory leaks.

### Dependencies
- Install the mysqlcppcon-dev (or devel package, search for your distro package)
```
sudo apt install libmysqlcppconn-dev
```

- Requires CMake
```
sudo apt install cmake
```

- Tested with clang/LLVM compiler
```
sudo apt install clang clang-tidy
```

### Import the example db
```
mysql -h YOURHOST -P HOSTPORT -u HOSTUSER -p example < example.sql
```

### Build
```
mkdir build
cd build
cmake ..
make -j
```

### Configure your environment
Configure the database information
```
cp ../.env.example .env
nano .env
```

### Running
```
source .env && ./bin/mysqlcppconraii
```



