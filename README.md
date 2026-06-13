# Autonomous Mini-Sumo Evolution (2019-2025)

This repository documents the iterative hardware and firmware development of a constraint-driven autonomous mini-sumo robot. 

## 2025 Iteration: 1.4cm Z-Height Chassis

Designed under strict personal budget limits and a maximum 1.4cm wedge profile requirement, necessitating non-standard layout and power delivery decisions to maintain operational stability.

### Hardware & Firmware Specifications
* **MCU:** ESP32-S3 (Dual-Core).
* **RTOS:** FreeRTOS (xTasks) separating sensor polling routines from motor control execution loops to minimize timing jitter.
* **Sensing:** 8x8 Matrix ToF sensors utilizing bitwise masking to avoid nested `if` comparisons and simplify the detection logic.
* **Power Delivery Network (PDN):** Split-rail architecture. 6S (22.2V) LiPo rail for motors; dedicated 1S (3.7V) cell for MCU/Sensors. Implemented to bypass the need for high-capacitance bulk decoupling that exceeded the 1.4cm Z-height restriction, successfully preventing MCU brownouts during high-current motor startup events.
* **PCB & EMI:** Custom 2-layer PCB. Due to budget constraints precluding a 4-layer stackup with an internal ground plane, ground patching was utilized. Grounded copper tape shields the motor mounts to attenuate high-frequency switching noise radiating onto adjacent I2C lines.

---

## Engineering Constraints & Resolutions

### 1. EMI Mitigation Under Budget Limits
* **Fault:** 22.2V PWM switching across high-power motors induced significant parasitic coupling on I2C traces, resulting in bus lockups and lost packets (NACKs).
* **Resolution:** Restricted to a 2-layer layout, continuous unbroken return paths were impossible. Added ground patches under critical traces and physical grounded copper tape enclosures around the noise source to protect signal integrity.

### 2. PDN Brownout Management
* **Fault:** High current draw during motor stall/startup caused aggressive voltage drops on the shared power rail, pulling the voltage below the MCU reset threshold.
* **Resolution:** Volumetric limits (1.4cm) prohibited large electrolytic bulk capacitors. Separated the logic load onto an isolated 1S battery, ensuring a stable regulator output irrespective of the motor rail's transient state.

### 3. Execution Health (POST)
* **Implementation:** Firmware initializes with a Power-On Self-Test (POST). Verifies I2C device presence and rail stability. Fails to a safe error loop (rotation + visual flag) if a hardware fault is detected, avoiding unpredictable behavior in the ring.

---

## Iteration History
* **2019-2022:** Base asynchronous logic (`millis()`), GPIO interfacing, bare-metal motor control.
* **2023:** RP2040 integration, custom 2-layer DIY etching, CNC chassis machining.
* **2024:** ESP32-C3 migration, I2C bus multiplexing to resolve hardware address collisions.
* **2025:** SMD reflow implementation, multi-core RTOS task scheduling, aggressive Z-height optimization.

## Media
- [First Robot 2022](media/first_robot.jpeg)
<img src="media/first_robot.jpeg" width="200">

- [Final Robot 2025](media/final_robot.jpeg)
<img src="media/final_robot.jpeg" width="200">

- [PCB Ground Patching Detail](media/pcb_design.jpeg)
<img src="media/pcb_design.jpeg" width="200">
