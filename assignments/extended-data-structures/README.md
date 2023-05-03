# ICS4U0 - Extending Data Structures Assignment
Completes [this assignment](https://github.com/johnfraserss/ICS4U/wiki/Extending-Data-Structures)

More info can be seen in the main file

## Build Instructions
You can either use cmake, or run this command:
(THIS SHOULD BE BE UPDATED)
```shell
g++ -std=c++20 -s -DNDEBUG -O2 -static -static-libgcc -static-libstdc++ -Iinclude/ .\src\BankAccount.cpp .\src\main.cpp src/Person.cpp src/util.cpp src/Vehicle.cpp src/VehicleDealership.cpp -o FILENAME
```