Layout
------
ESP1
    Lift: 1 EN 1 DIR 4 PUL
    Drive: 1 EN 2 DIR 2 PUL
    Lift Limit Switch: 4 pins

ESP2
    Turn: 1 EN 4 DIR 4 PUL
    Turn Limit Switch: 4 pins

General Framework
-----------------

struct Motors {
    Motor * wheel1
    Motor * wheel2
    Motor * wheel3
    Motor * wheel4
}