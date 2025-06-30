# Smart Energy Meter with ESP32 & Machine Learning

This project combines embedded systems and machine learning to create a smart energy meter capable of identifying which household appliance is currently active based on its power consumption signature. An ESP32 reads voltage/current data from a non-intrusive current sensor (CT clamp), publishes it via MQTT, and receives predictions from a machine-learning classifier. The result is displayed on an OLED screen and can also be integrated into platforms like Home Assistant.

## 🔧 Features

- Real-time monitoring of voltage, current, and power
- data publishing
- Machine learning classifier (Random Forest) trained offline on labeled data
- OLED screen display of live measurements and predicted appliance
- Compatible with Home Assistant 

---

## 🧰 Parts List

| Component               | Quantity | Notes                                                                 |
|------------------------|----------|-----------------------------------------------------------------------|
| ESP32 Dev Board        | 1        | e.g., ESP32-WROOM or ESP32 D1 Mini                                    |
| CT Clamp Sensor        | 1        | e.g., SCT-013-000 (non-invasive current transformer)                  |
| Burden Resistor        | 1        | 33Ω–100Ω, required for analog reading of SCT-013-000                  |
| OLED Display (I2C)     | 1        | 0.96" 128x64, SSD1306 chipset                                         |
| Jumper Wires           | As needed| Male-to-male and female-to-male for connections                       |
| Breadboard             | 1        | For prototyping (optional)                                            |
| USB Cable              | 1        | For uploading firmware to ESP32                                       |
| 3.5mm Jack + Terminal  | 1        | Optional, for easy connection to CT clamp                             |
| Computer with Python   | 1        | For training model + running real-time classifier                     |
| MQTT Broker (e.g. Mosquitto) | 1   | Can run on Raspberry Pi or PC (localhost)                             |

---


## Python dependencies

bash
pip install -r ml/requirements.txt


## License

MIT
