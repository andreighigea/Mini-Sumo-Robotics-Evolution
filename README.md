# My Autonomous Mini-Sumo Robot (2019-2025)

I’ve been building autonomous mini-sumo robots since 2019. This repository covers my latest iteration, where my main challenge was squeezing everything into a chassis just 2.5cm tall while working with a tight high school budget. 

Because of the strict size limit, I couldn't always rely on standard hardware solutions. I had to get a bit creative with my circuit board layout and power delivery to keep the robot stable in the ring.

### Hardware & Firmware Specs
* **Brain:** ESP32-S3. I took advantage of its multicore processing to split up the workload. One core is completely dedicated to polling the sensors, while the other handles the motor control loops. This keeps the timing tight so they don't interrupt each other.
* **Sensors:** Time-of-Flight matrix sensors running on an I2C bus.
* **Power:** A split-rail setup. I used a 6S (22.2V) battery exclusively for the motors, and a separate, much smaller 1S (3.7V) battery just for the logic (the ESP32 and sensors). 
* **Board:** A custom 2-layer PCB I designed for the project.

---

### Issues I Ran Into & How I Fixed Them

**1. I2C Bus Freezing (EMI Issues)**
* **The Fault:** The motors run on a lot of voltage, and switching them on and off rapidly created a ton of Electromagnetic Interference (EMI). This electrical noise was bleeding into my I2C data lines, causing the sensors to lock up and drop packets. 
* **The Fix:** A 4-layer PCB with an internal ground plane would have easily blocked this noise, but that was out of my budget. Instead, I carefully routed ground patches under my critical I2C traces on the 2-layer board. I also shielded the motor mounts with grounded copper tape. It's a bit of a DIY workaround, but it successfully stopped the interference.

**2. The Robot Kept Resetting (Brownouts)**
* **The Fault:** When the motors stalled out while pushing another robot, they pulled huge spikes of current. This caused the voltage on the shared power rail to drop so low that the ESP32 would lose power and reset right in the middle of a match.
* **The Fix:** Normally, you'd fix this by adding large electrolytic capacitors to the board to smooth out the power dips. But because my robot had to stay under 2.5cm tall, big capacitors simply wouldn't fit. My solution was separating the logic load onto its own isolated 1S battery, ensuring the ESP32 stays on no matter how hard the motors are working.

**3. Unpredictable Behavior in the Ring**
* **The Fault:** In older versions, if a wire came loose or an I2C sensor died, the robot would act unpredictably and often just drive itself out of the ring.
* **The Fix:** I added a startup check (Power-On Self-Test) to the firmware. Now, when I power it on, the robot pings all the I2C devices to make sure they are responding and checks that the power rails are stable. If it detects a hardware fault, it fails safely into an error loop—spinning and flashing a warning light—instead of going rogue in a match.

---

### Project Evolution

Just to show how this project has progressed over the years:
* **2019-2022:** Started out learning Arduino basics, simple GPIO sensors, and getting bare-metal motor control working.
* **2023:** Upgraded to an RP2040 chip, learned how to etch my own 2-layer circuit boards at home, and CNC machined some basic chassis parts.
* **2024:** Migrated to the ESP32-C3 and figured out I2C bus multiplexing so I could use multiple sensors that shared the exact same hardware address without them conflicting.
* **2025:** Learned surface-mount (SMD) soldering, implemented multicore processing for smoother performance, and finalized the aggressive 2.5cm low-profile design.
