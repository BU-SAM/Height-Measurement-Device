# Height Measurement Device (Using a Time of Flight (ToF) sensor and a 128px by 64px OLED screen)

## Overview

This project is a compact embedded system designed to measure height using a Time-of-Flight (ToF) distance sensor and display the result in real-time on an OLED screen. It is built using a microcontroller (ESP32) and optimized for accuracy and responsiveness.
It works by detecting the distance between an object and the TOF sensor then deducting that value from the total height i.e the distance between TOF and ground, then display's it on the OLED screen. 

---

## Features

* Accurate height measurement using ToF sensor
* Real-time display on SSD1306 OLED
* Fast and responsive readings
* Optional smoothing algorithm for stable output
* Simple I2C communication setup

---

## Hardware Components

* ESP32 microcontroller
* VL53L0X Time-of-Flight (ToF) sensor
* SSD1306 OLED Display (128x64)
* Jumper wires
* Breadboard

---

## Pin Connections (ESP32)

| Component | ESP32 Pin |
| --------- | --------- |
| SDA       | GPIO 32   |
| SCL       | GPIO 33   |
| VCC       | 3.3V      |
| GND       | GND       |

> Note: Ensure the sensor and OLED operate at 3.3V logic levels.

---

## Software Requirements

* Arduino IDE
* Required Libraries:

  * Adafruit GFX
  * Adafruit SSD1306
  * Adafruit VL53L0X

---

## Installation & Setup

1. Clone or download this repository
2. Open the project in Arduino IDE
3. Install the required libraries
4. Connect the hardware as shown in the schematic
5. Select the correct board and COM port
6. Upload the code to the microcontroller

---

##  Usage

1. Power the device
2. Place the object/person under the of the sensor
3. The measured height/distance will be displayed on the OLED screen in real time
NOTE: The sensor should be placed at the roof facing down

---

## Code Explanation

* **Sensor Initialization:**
  Initializes VL53L0X using I2C communication

* **Display Setup:**
  OLED initialized using SSD1306 driver

* **Measurement Loop:**
  Continuously reads distance data from the sensor

* **Smoothing (Optional):**
  Averages multiple readings to reduce noise and improve stability

---

## Testing & Performance

* Measurement range: ~30mm to 2000mm
* Accuracy: ±2–5 mm (depending on environment)
* Stable output achieved using smoothing algorithm

---

## Challenges & Solutions

| Challenge                | Solution                         |
| ------------------------ | -------------------------------- |
| Unstable readings        | Implemented averaging/smoothing  |
| I2C not detecting device | Verified wiring and I2C address  |
| OLED not displaying      | Checked power and initialization |

---

## 🔮 Future Improvements

* Add calibration feature
* Battery-powered portable design
* Store measurement data
* Add buzzer or alert system

---

## Author

Samuel Buraimoh

---

## License

This project is open-source and available for educational and personal use.
