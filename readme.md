

---

# FreeRTOS Educational Project

This project is designed for educational purposes and is inspired by Nayak Kiran's Udemy course, **"Mastering RTOS: Hands-on FreeRTOS and STM32Fx with Debugging"**. It demonstrates a FreeRTOS application that provides hands-on experience with real-time operating system concepts using an STM32 microcontroller.

## Overview

The application allows users to:

- Control LED lighting modes.
- Configure the date and time via a terminal menu. (RTC)

Communication between the STM32 board and the computer is achieved through **UART**, with menu options displayed on the terminal (e.g., Tera Term).

### Key Learning Outcomes

This project introduces several essential FreeRTOS concepts, such as:

- **Task Management**
- **Task Notifications**
- **Queue Management**

## Project Details

The application consists of **five FreeRTOS tasks**, each serving a specific purpose:

### 1. **Menu Task**

- Displays a menu on the terminal.
- Captures and processes user input to determine their choice.

### 2. **LED Task**

- Handles LED control based on user input.
- Provides a sub-menu to configure LED modes.

### 3. **RTC Task**

- Manages the Real-Time Clock (RTC) peripheral of the STM32 MCU.
- Updates the date and time settings based on user input.

### 4. **Print Task**

- Responsible for handling UART communication.
- Dequeues messages from a **Print Queue** and sends them to the terminal for display.

### 5. **Command Handling Task**

- Notified by the **UART ISR** after user input is received.
- Processes input data stored in an **Input Data Queue**.
- Detects the newline character (`\n`) to identify complete commands and executes the corresponding task.

![[images/image1.png]]

---

## Requirements

- **Hardware**: STM32Fx MCU board
- **Software**:
    - STM32CubeIDE or any other suitable development environment
    - Tera Term (or any terminal emulator)
- **FreeRTOS Library** : Have a look on "Implementing FreeRTOS with STM32" where i explain how to implement the FreeRTOS on STM32

---

## How does it works ? 
* The **Menu Task** generates the menu and enqueues it into the **Print Queue**, from which the **Print Task** dequeues and transmits the content to the user's terminal via UART. 

![[images/image2.png]]


* Users interact by entering commands through the terminal, which are received by the UART peripheral and handled by the **UART ISR**. The ISR enqueues the input data into the **Input Data Queue** and notifies the **Command Handling Task** upon detecting a complete input (e.g., via newline). The Command Handling Task processes the input, formats it into commands, and notifies relevant tasks, such as the LED Task for controlling LED effects or the RTC Task for managing date and time settings

![[images/image3.png]]

It's a very simple project to start your journey in the FreeRTOS world ! 


This project is based on the course:  
**[Mastering RTOS: Hands-on FreeRTOS and STM32Fx with Debugging](https://www.udemy.com/course/mastering-rtos-hands-on-with-freertos-arduino-and-stm32fx/?srsltid=AfmBOoqyL0AUMeJ_xK0xDSwyz2xb8wy9dr2umhWYIylfvCSNt5FaN6Tf&couponCode=LETSLEARNNOW)**  
by **Kiran Nayak** on Udemy.
