#!/usr/bin/env python
import paho.mqtt.client as mqtt
import sys

if __name__ == "__main__":

	# note - angle in degrees, counterclocksize (like unit circle)
	dist = 0
	tm = 0
	ta = 0

	if len(sys.argv) != 1:
		dist = float(sys.argv[1])
		tm = float(sys.argv[2])
		ta = float(sys.argv[3])

	client = mqtt.Client()
	client.connect("mqtt.eclipse.org", 1883, 60)

	odom_string = '{\"sequence\": 0, \"distance\": %f, theta_move\": %f, \"theta_arm\": %f}'%(dist, tm, ta)

	client.publish("rover/odom", odom_string)

	client.disconnect()