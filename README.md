# entities
A C++ simple entity component system

## the library
The file that actually contains the library is entities.hpp.

## compared to EntityX
entities:
```
entityx create destroy entity with components     5000000         254 ns/op
      [25] entityx       25 entities component systems update     1000000        1572 ns/op
      [50] entityx       50 entities component systems update      500000        3009 ns/op
     [100] entityx      100 entities component systems update      200000        5995 ns/op
     [200] entityx      200 entities component systems update      100000       11848 ns/op
     [400] entityx      400 entities component systems update       50000       23631 ns/op
     [800] entityx      800 entities component systems update       20000       51669 ns/op
    [1600] entityx     1600 entities component systems update       10000      112446 ns/op
    [3200] entityx     3200 entities component systems update        5000      225504 ns/op
    [5000] entityx     5000 entities component systems update        5000      368282 ns/op
   [10000] entityx    10000 entities component systems update        1000     1240502 ns/op
   [30000] entityx    30000 entities component systems update         200     5196731 ns/op
  [100000] entityx   100000 entities component systems update         100    18331731 ns/op
  [500000] entityx   500000 entities component systems update          20    93182044 ns/op
 [1000000] entityx  1000000 entities component systems update          10   191315282 ns/op
 [2000000] entityx  2000000 entities component systems update           5   299790721 ns/op
./test 43.175s
```

EntityX:
```
entityx create destroy entity with components     5000000         200 ns/op
      [25] entityx       25 entities component systems update      500000        2903 ns/op
      [50] entityx       50 entities component systems update      200000        5721 ns/op
     [100] entityx      100 entities component systems update      100000       11338 ns/op
     [200] entityx      200 entities component systems update       50000       22622 ns/op
     [400] entityx      400 entities component systems update       50000       45560 ns/op
     [800] entityx      800 entities component systems update       20000       92718 ns/op
    [1600] entityx     1600 entities component systems update       10000      180159 ns/op
    [3200] entityx     3200 entities component systems update        5000      360127 ns/op
    [5000] entityx     5000 entities component systems update        2000      563316 ns/op
   [10000] entityx    10000 entities component systems update        1000     1130659 ns/op
   [30000] entityx    30000 entities component systems update         500     3431964 ns/op
  [100000] entityx   100000 entities component systems update         100    11680312 ns/op
  [500000] entityx   500000 entities component systems update          20    59996331 ns/op
 [1000000] entityx  1000000 entities component systems update          10   128663563 ns/op
 [2000000] entityx  2000000 entities component systems update           5   271582063 ns/op
./xtest 41.912s
```
You can find EntityX [here](https://github.com/alecthomas/entityx).
