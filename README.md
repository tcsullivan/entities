# entities
A C++ simple entity component system

## the library
The file that actually contains the library is entities.hpp.

## compared to EntityX
entities:
```
entities create destroy entity with components     5000000         279 ns/op
      [25] entities       25 entities component systems update     1000000        1610 ns/op
      [50] entities       50 entities component systems update      500000        3098 ns/op
     [100] entities      100 entities component systems update      200000        6247 ns/op
     [200] entities      200 entities component systems update      100000       12274 ns/op
     [400] entities      400 entities component systems update       50000       25029 ns/op
     [800] entities      800 entities component systems update       20000       55717 ns/op
    [1600] entities     1600 entities component systems update       10000      116305 ns/op
    [3200] entities     3200 entities component systems update        5000      244461 ns/op
    [5000] entities     5000 entities component systems update        5000      463835 ns/op
   [10000] entities    10000 entities component systems update        1000     1647042 ns/op
   [30000] entities    30000 entities component systems update         200     5784773 ns/op
  [100000] entities   100000 entities component systems update          50    20711117 ns/op
  [500000] entities   500000 entities component systems update          20   103582654 ns/op
 [1000000] entities  1000000 entities component systems update          10   208073004 ns/op
 [2000000] entities  2000000 entities component systems update           5   341977621 ns/op
./entitiesTests 45.328s
```

EntityX:
```
entityx create destroy entity with components     5000000         212 ns/op
      [25] entityx       25 entities component systems update      500000        3025 ns/op
      [50] entityx       50 entities component systems update      200000        6274 ns/op
     [100] entityx      100 entities component systems update      100000       12101 ns/op
     [200] entityx      200 entities component systems update       50000       24227 ns/op
     [400] entityx      400 entities component systems update       50000       48100 ns/op
     [800] entityx      800 entities component systems update       20000       95872 ns/op
    [1600] entityx     1600 entities component systems update       10000      196251 ns/op
    [3200] entityx     3200 entities component systems update        5000      397806 ns/op
    [5000] entityx     5000 entities component systems update        2000      623035 ns/op
   [10000] entityx    10000 entities component systems update        1000     1279807 ns/op
   [30000] entityx    30000 entities component systems update         500     3791401 ns/op
  [100000] entityx   100000 entities component systems update         100    13074271 ns/op
  [500000] entityx   500000 entities component systems update          20    65787539 ns/op
 [1000000] entityx  1000000 entities component systems update          10   137753731 ns/op
 [2000000] entityx  2000000 entities component systems update           5   290476228 ns/op
./entityXTests 45.232s
```
You can find EntityX [here](https://github.com/alecthomas/entityx).
