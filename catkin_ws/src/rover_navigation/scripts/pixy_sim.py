#!/usr/bin/env python
import paho.mqtt.client as mqtt
import sys

if __name__ == "__main__":

	# note - angle in degrees, counterclocksize (like unit circle)
	ident = 0
	dist = 0
	theta = 0

	if len(sys.argv) != 1:
		dist = float(sys.argv[1])
		theta = float(sys.argv[2])
		ident = int(sys.argv[3])

	client = mqtt.Client()
	client.connect("mqtt.eclipse.org", 1883, 60)
	pixy_string = '{\"sequence\": 0, \"distance\": %f, \"theta\": %f, \"identity\": %d}'%(dist, theta, ident)
	print("[PUBLISH] christos/pixycam/data -", pixy_string)
	client.publish("christos/pixycam/data", pixy_string)

	client.disconnect()