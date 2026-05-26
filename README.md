# Muscle-Controlled-Robotic-Vehicle with Autonomous Safety Override

**Authors:** Harshpreet Singh Bhatti, Sukriti Uppal  
**Development Timeline:** January 2026 - April 2026  
**Focus Domains:** Biopotential Signal Processing, Master-Slave Embedded Architectures, Wireless Telemetry, Autonomous Reactive Control

## 📌 Project Overview
Conventional robotic navigation relies heavily on manual inputs like joysticks or mobile apps. `DeshBot` bridges the gap toward intuitive, hands-free biomechanical control by using **surface electromyography (SEMG) biopotentials** to guide vehicle movement. 

The system utilizes a distributed **Master-Slave topology** over a wireless Bluetooth telemetry link. To ensure asset protection, the vehicle includes an autonomous **Priority Safety Override** that instantly suppresses manual controls if an obstacle is detected within 20cm.

## 🛠️ Technical Architecture

### 1. System Topology
The system decouples biological signal filtering from core motor actuation across two distinct microcontroller stages:

* **Master Processing Stage (Biopotential Bridge):** Powered by an **Arduino R4** paired with an **EXG sensor** (Analog Channel `A0`). It runs a continuous 100ms smoothing window to calculate real-time peak-to-peak differentials ($SignalMax - SignalMin$). Data is streamed at 115200 bps via an **HC-05 Bluetooth module**.
* **Slave Actuation Stage (DeshBot Vehicle):** Powered by an **Arduino Nano** on a specialized DeshBot Dev Board. It decodes the Bluetooth tokens at 9600 bps to control a dual-DC geared motor chassis via an **L298N power bridge**.
* **Safety & Diagnostics:** A front-facing **HC-SR04 Ultrasonic Sensor** operates on a "Distance First" priority loop, while a **16x2 LCD Panel** displays live system diagnostics.

### 2. Muscle Mapping Logic (Three-Zone)
The Master node categorizes analog muscle contractions into three deterministic zones:
* **Zone 1 ($\le 60$):** Baseline Idle Noise $\rightarrow$ **Sends 's' (STOP)**
* **Zone 2 ($61 - 200$):** Moderate Contraction $\rightarrow$ **Sends 'f' (FORWARD)**
* **Zone 3 ($> 200$):** Peak Intentional Contraction $\rightarrow$ **Sends 'b' (REVERSE)**

## 🔌 Hardware Connections & Pin Mapping

### Ultrasonic Sensor Pinout
| Sensor Pin | Dev Board Pin |
|------------|---------------|
| VCC        | 5V            |
| Trig       | 12            |
| Echo       | 2             |
| GND        | GND           |

### HC-05 Bluetooth Module Pinout
| HC-05 Module | DeshBot Dev Board Pin |
|--------------|-----------------------|
| EN/KEY       | EN                    |
| VCC          | 5V                    |
| GND          | GND                   |
| TXD          | TXD                   |
| RXD          | RXD                   |
| STATE        | STATE                 |

> ⚠️ **Note:** The LCD panel framework utilizes isolated digital/analog lines (`A4, A3, A2, A1, A0, 13`), preserving vital hardware interrupts for the ultrasonic echo receiver line (`Pin 2`) to ensure unmasked sensor tracking. Always remove the Bluetooth module while uploading code to the Arduino.

## 🚦 Deterministic Evasive Maneuvering
When an object is detected within exactly **20cm**, the autonomous override cuts user control within **50ms**:
1. It immediately executes a braking sequence (`stopMotors()`).
2. Activates a sustained **1000ms hardware turn** to pivot the vehicle 180 degrees away from danger.
3. Smooth Path Transitioning: Instead of remaining locked or stopping post-turn, the loop logic automatically forces a forward transition command (`'f'`), maintaining continuous spatial movement without manual resetting.

## 🚀 Getting Started

1. **Hardware Setup:** Assemble the chassis, connect the motors to the onboard terminals, and wire the sensors according to the mapping tables above. Power the motors via an external Lithium battery (do not draw motor power directly from the Arduino's 5V supply).
2. **Upload Master Code:** Open `Firmware/Master_Code/Master_Code.ino` in the Arduino IDE and upload it to your Arduino R4. Set the Serial Monitor baud rate to `115200`.
3. **Upload Slave Code:** Disconnect the HC-05 module, open `Firmware/Slave_Code/Slave_Code.ino`, and upload it to your Arduino Nano. Set the Arduino IDE Baud Rate to `9600`. Reconnect the Bluetooth module.
4. **Operation:** Power up the vehicle. The LCD will display `DESHBOT CAR`. Pair your smartphone/Master module with the HC-05 and begin operation!
  
