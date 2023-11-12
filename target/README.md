The given files can be separted by their task.

1. Hardware Abstraction:
    - [LED](LED.cpp): Abstraction of a LED
    - [Button](Button.cpp): Abstraction of a Button
    - [DS18B20 Temperature Sensor](DS18B20.cpp): Abstraction of the DS18B20 Sensor
    - [LED Blinking](Blinker.cpp): Helper to let LEDs blink asynchronous

2. Protocol:
    - [Server](Server.cpp): TCP Server
    - [Observer](Observer.hpp) and [Subject](Subject.hpp): Observer Pattern
    - [Protocol](Protocol.cpp): Basic protocol message generation

3. Application:
    - [Handler](Handler.cpp): Takes care of the communication and handling of the LEDs
    - [main](main.cpp): Hardware initialization and sensor interaction