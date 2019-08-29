# lw-cpp11-logger
Light Weight C++11 Logger

### How to build
```
g++ -o lwtest lwtest.cpp Logger.cpp -std=c++11 -lpthread
```

### Run & see output
```
./lwtest
```

In stdout
```
2019-08-29 05:10:33.507 DEBUG [main]: Hello, world
2019-08-29 05:10:33.507 INFO [main]: This line goes everywhere
2019-08-29 05:10:33.507 WARNING [main]: This line goes to stdout only
```

In file by `less /tmp/lwtest.log`
```
2019-08-29 05:10:33.507 DEBUG [main]: Hello, world
2019-08-29 05:10:33.507 INFO [main]: This line goes everywhere
2019-08-29 05:10:33.507 ERROR [main]: This line goes to file only
```
