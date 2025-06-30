# Smart-Energy-NILM-Web

Tiny-ML powered DIY smart energy meter that **identifies which appliance
just turned on** and streams live data to:

* **MQTT / Home-Assistant**
* **Built-in ESP32 web dashboard** (Server-Sent Events)

![dashboard screenshot](docs/dashboard.png)

## 🔧 Hardware

| Qty | Part | Notes |
| --- | ---- | ----- |
| 1 | **JSY-MK-194G** energy meter | Single-phase, Modbus-RTU UART |
| 1 | **ESP32 DevKit** | Any module with 2 free UART pins |
| 1 | 5 V isolated PSU | e.g. Meanwell APV-8-5 |

Wiring: JSY TX→GPIO16, JSY RX→GPIO17, 3V3 & GND common.

## 🚀 Flash

```bash
# clone & build
git clone https://github.com/<you>/smart-energy-nilm-web.git
cd smart-energy-nilm-web
pio run -t upload          # flash firmware
pio run -t uploadfs        # optional: assets → SPIFFS
pio device monitor         # serial log
