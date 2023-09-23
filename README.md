# Electrical Load

## Project Description

The goal of this project is to create an electronic load for testing the power supply of a UPS.

To achieve this, we have designed a board where MOSFETs are employed as the load, along with a combination of low-resistance resistors and operational amplifiers to calculate the power dissipation.

To control the opening of the MOSFET gates, we have utilized an STM microcontroller that reads the current and voltage at 100 kHz to ensure that the system does not overload. Additionally, it sends information via UART protocol to the main controller every second.
