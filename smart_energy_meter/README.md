
# Smart Energy Meter with ESP32 & Machine Learning

This project extends Giovanni Aggiustatutto's DIY energy meter by adding a
machine‑learning classifier that learns the power signature of each appliance
in your house.

## Usage workflow

1. **Flash the firmware** (`firmware/`) after editing `config.h`.
2. **Collect data** into CSV `timestamp,voltage,current,power,appliance` while
   switching appliances on/off.
3. **Train** a model with `python ml/train_model.py ml/data/dataset.csv`.
4. **Deploy** real‑time classifier (`ml/realtime_classifier.py`) on any machine
   connected to the MQTT broker. The script publishes the recognised appliance
   label which is displayed on the OLED and in Home Assistant.

## Python dependencies

```bash
pip install -r ml/requirements.txt
```

## License

MIT
