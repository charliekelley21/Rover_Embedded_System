#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import time
import json
import math
import numpy as np
import matplotlib.pyplot as plt

def on_message(client, userdata, message):
	# print("HI")
	# print(message.topic, message.payload)

	if message.topic == 'spencer/lidar':
		# add data point to our global array, and process it when it gets to certain size
		# print(message.payload)
		# extract the data
		data = json.loads(message.payload.decode("utf-8"))

		if data['d'] != 0 and data['q'] > 10:
			a = -1.*float(data['a'] / 64.0) * (2.*3.1415/360.) # rads
			d = float(data['d'] / 4.0) / 1000.
			LIDAR_points.append([d*math.cos(a), d*math.sin(a)])

client = mqtt.Client()
client.on_message = on_message

client.connect("mqtt.eclipse.org",1883, 60)
client.loop_start()
client.subscribe("spencer/lidar")

# keep a list of points, append in the on_message function
# FORMAT:		quality, angle, distance [int, float, float]
LIDAR_points = []

SCALE = 2.5

while 1:
	time.sleep(0.25)

	# publish a scan every 20 points
	if len(LIDAR_points) > 250:

		xy = np.array(LIDAR_points)
		plt.clf()
		plt.scatter(0, 0)
		plt.scatter(xy[:, 0], xy[:, 1])
		plt.xlim(-1*SCALE, SCALE)
		plt.ylim(-1*SCALE, SCALE)
		plt.pause(0.05)
		LIDAR_points = []

plt.show()
