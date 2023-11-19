The given files can be separted by their task.

1. Hardware Abstraction (see `hardware/`):
    - [LED](hardware/LED.cpp): Abstraction of a LED
    - [Button](hardware/Button.cpp): Abstraction of a Button
    - [DS18B20 Temperature Sensor](hardware/DS18B20.cpp): Abstraction of the DS18B20 Sensor
    - [LED Blinking](hardware/Blinker.cpp): Helper to let LEDs blink asynchronous

2. Communication (see `communication/`):
    - [Server](communication/Server.cpp): TCP Server
    - [Observer](communication/Observer.hpp) and [Subject](communication/Subject.hpp): Observer Pattern
    - [Protocol](communication/Protocol.cpp): Basic protocol message generation

3. Application:
    - [Handler](Handler.cpp): Takes care of the communication and handling of the LEDs
    - [main](main.cpp): Hardware initialization and sensor interaction