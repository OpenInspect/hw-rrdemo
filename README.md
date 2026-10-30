# Overview

Testing tool for two A4988 stepper drivers on an ESP32-C6.
Controlled by a web page with simple buttons for direction and number of steps.

# Wiring
| Signal | Motor 1 | Motor 2 | A4988 pin |
|--------|---------|---------|-----------|
| STEP   | GPIO 4  | GPIO 7  | STEP      |
| DIR    | GPIO 5  | GPIO 1  | DIR       |
| EN     | GPIO 6  | GPIO 12 | ENABLE    |

