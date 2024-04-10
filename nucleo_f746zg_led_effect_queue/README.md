# RTOS example application

Platform: Nucleo-F746ZG
CPU:

Goal: This program communicates with the user over uart2 (PA2,PA3) with baud 115200. The user then is presented with a menu which allows the user
to select an LED blinking mode, see the real time clock and configure the clock.

Implementation:

Running Tasks:
    LEDTask - blinks the 3 user LED according to the blinking mode. The blinking mode changes the way that the LEDs blink.
    RTCTask
    PrintTask - prints any application data over uart to the user
    CommandParseTask - Parses the user command and notifies the appropriate task with the action to be taken
    MenuTask
    Idle

Interrupts:
    UART2 - gets user data over uart and notifies the CommandParseTask once a command is present to be proccessed
