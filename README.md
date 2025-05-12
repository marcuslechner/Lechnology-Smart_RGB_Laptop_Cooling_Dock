
# Lechnology Smart RGB Laptop Cooling Dock

**An open-source, smart, RGB laptop cooling dock** engineered from the ground up with 3D-printed components, a custom-designed PCB, and embedded C firmware on an STM32 Microcontroller. It features **PID-controlled fan speed regulation** based on real-time temperature sensing, ensuring optimal cooling performance. With individually addressable RGB lighting, tactile rotary encoder input, and an OLED display interface, this dock balances function and flair for any gaming or productivity setup.

📦 **Proudly sponsored by [PCBWay](https://pcbway.com/g/QN1n73)** — all PCBs in this project were professionally fabricated through their prototyping service.  

![3D Printed RGB Laptop Dock](assets/laptop_dock_thumbnail.png)

[![Watch on YouTube](https://img.shields.io/badge/Watch%20Video-%F0%9F%8E%A5-red?logo=youtube)](https://youtu.be/KlnJxFxes5I)  
[![GitHub Stars](https://img.shields.io/github/stars/marcuslechner/Lechnology-Smart_RGB_Laptop_Cooling_Dock?style=social)](https://github.com/marcuslechner/Lechnology-Smart_RGB_Laptop_Cooling_Dock)

---

## 🎥 Design and Build

📺 **[Click to watch the full project video on YouTube](https://youtu.be/KlnJxFxes5I)**  
Covers mechanical CAD, circuit design, PCB layout, assembly, and embedded firmware development.

---

## 🔧 Features

- 🌬️ **PID-Controlled Dual Fan System** — Intelligent fan speed adjustment using a PID algorithm for stable thermal regulation  
- 🌡️ **Temperature Monitoring** — Real-time temperature sensing feeds the PID loop for adaptive cooling  
- 💡 **RGB Illumination** — Individually addressable WS2812B LEDs with programmable visual effects  
- 🌀 **Custom PCB** — Professionally designed circuit board integrating power regulation, sensors, encoder, and display  
- 🔁 **Rotary Encoder Input** — Allows cycling between lighting modes, adjusting target temperature, or manual override  
- 📟 **OLED Display** — Shows current temperature, fan speed, and system status  
- 🧠 **STM32F103C8 MCU** — Affordable 32-bit microcontroller running robust, well-structured embedded firmware
- 🧱 **3D-Printed Enclosure** — Custom-designed chassis optimized for airflow, electronics mounting, and cable routing  

---

## 🧰 Bill of Materials

| Category      | Item                                        |
|---------------|---------------------------------------------|
| Microcontroller | [STM32F103C8T6](https://amzn.to/4fDvRRE)     |
| Cooling        | [Computer Fans](https://amzn.to/4hZPmWb)     |
| Lighting       | [WS2812B RGB LED Strip](https://amzn.to/3ZeDNmJ) |
| Input          | [Rotary Encoder](https://amzn.to/3CElFd5)     |
| Display        | [OLED Screen](https://amzn.to/40X8Cxr)        |
| Power Reg.     | [Buck-Boost Converter](https://amzn.to/3BdbzzH) |
| Sensors        | [Temperature Sensor](https://amzn.to/3ASJWLU) |
| Housing        | [3D Printed (PLA)](https://amzn.to/3velPAv)   |
| Fastening      | [Heat Inserts](https://amzn.to/4eLyRu2)       |

---

## 🖨️ Mechanical Design

All mechanical parts were modeled in **SolidWorks** and 3D printed on an **Ender 3**.  
🔗 [Thingiverse Model Files](https://www.thingiverse.com/thing:6853587)

---

## 📡 Circuit Design & PCB

Schematics and PCB were designed in **KiCad**, and manufactured via **PCBWay**.

🧾 **Get $5 off your first order at PCBWay**:  
👉 [https://pcbway.com/g/QN1n73](https://pcbway.com/g/QN1n73)  
📄 [Public PCB project link](https://www.pcbway.com/project/shareproject/Lechnology_RGB_Laptop_Cooler.html)

---

## 🧠 Firmware Overview

The embedded control firmware is written in C and built with **STM32CubeIDE**, running on the STM32F103C8T6 microcontroller. It includes:

- ✅ **PID Temperature Control Loop**: A closed-loop control system that modulates fan speed based on measured temperature vs. target setpoint (in progress)
- ✅ **I²C OLED UI Module**: Live system readouts including temp, PWM duty, and user settings  
- ✅ **Rotary Encoder with Debounce**: Interrupt-driven input for precise user interactions  
- ✅ **WS2812B LED Control**: Uses DMA and timer peripherals to generate precise LED signal without CPU-intensive bit-banging
- ✅ **BME280 Sensor via I²C**: Interfaces with an external BME280 sensor to measure ambient temperature (and optionally humidity/pressure)
- ✅ **Modular Code Structure**: Organized into reusable drivers and logic layers for easy expansion  

🎯 Source code is modular and documented in the `/Core` folder.

---

## 🚀 Getting Started

1. Clone the repo:
   ```bash
   git clone https://github.com/marcuslechner/Lechnology-Smart_RGB_Laptop_Cooling_Dock.git
   ```

2. Open the `.ioc` file in **STM32CubeIDE**
3. Compile and flash the firmware using a **ST-Link** or **USB-UART** bootloader

---

## 📜 License

This project is licensed under the **Lechnology Non-Commercial MIT + Beerware License**:

> If you use this and we meet in person, you're invited to buy me a coffee or beer ☕🍺  
> Commercial use is prohibited without prior approval.

See [LICENSE.md](LICENSE.md) for full terms.

---

## ❤️ Support the Channel

Using these links helps support **Lechnology Engineering** at no cost to you:

🛠️ [Recommended Tools](https://amzn.to/3SStW2l)  
🔌 [Electronics Kits](https://amzn.to/3SStW2l)  
🎥 [More Projects on YouTube](https://youtube.com/@lechnologyengineering)
