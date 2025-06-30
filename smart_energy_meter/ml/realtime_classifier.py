
#!/usr/bin/env python3
"""Realtime MQTT appliance classifier."""

import json, joblib, paho.mqtt.client as mqtt

BROKER = 'localhost'
PORT   = 1883
SUB    = 'home/energy/+/telemetry'
PUB    = 'home/energy/{device}/appliance'

bundle = joblib.load('model.joblib')
clf = bundle['model']
features = bundle['features']

def vec(d):
    return [d.get(f,0) for f in features]

def on_connect(client, userdata, flags, rc, props=None):
    client.subscribe(SUB)

def on_message(client, userdata, msg):
    try:
        data = json.loads(msg.payload)
        device = msg.topic.split('/')[2]
        pred = clf.predict([vec(data)])[0]
        client.publish(PUB.format(device=device), pred, retain=True)
        print(device, pred)
    except Exception as e:
        print('Err', e)

cli = mqtt.Client()
cli.on_connect = on_connect
cli.on_message = on_message
cli.connect(BROKER, PORT, 60)
cli.loop_forever()
