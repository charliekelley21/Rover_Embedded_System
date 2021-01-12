#!/usr/bin/env python
import paho.mqtt.client as mqtt
import time

def on_message(client, userdata, message):
	print(message.topic)
	print(message.payload)

client = mqtt.Client()
client.on_message = on_message
client.connect("mqtt.eclipse.org", 1883, 60)
client.loop_start()
client.subscribe("lidar/analytics")


while 1:
	time.sleep(0.5)