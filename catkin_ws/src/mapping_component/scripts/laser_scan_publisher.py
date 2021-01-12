#!/usr/bin/env python
import paho.mqtt.client as mqtt
import time
import json
import math
import numpy as np

# ros stuff
import rospy
from sensor_msgs.msg import LaserScan

# see if we wanna print things
VERBOSE = True
POINTS_THRESHOLD = 360

# this is a dict of all the angles (0-360, and the distances at each)
scan = {}
# init to all ZEROS
for i in range(0, 360):
	# format - avg distance, num scans (so we can addon appropriately)
	scan[i] = [0, 0]
scan_complete = False
points_in_scan = 0 # only keep if we are above a threshold (360)
seq_recv = 0

def on_message(client, userdata, message):
	global scan
	global points_in_scan
	global scan_complete
	global seq_recv

	if message.topic == 'lidar/start_scan' and VERBOSE:
		rospy.loginfo("Scan STARTED")
		points_in_scan = 0 # reset

	if message.topic == 'lidar/point':
		# add data point to our global array, and process it when it gets to certain size
		# print(message.payload)
		# extract the data
		data = json.loads(message.payload.decode("utf-8"))

		if data['d'] == 1:
			scan_complete = True
		# check to see if we got a good point
		elif data['d'] != 0 and data['q'] > 10:
			points_in_scan += 1
			# seq_recv = data['seq']

			# add to our dict too
			a_deg = -1*int(data['a'] / 64) % 360			# make sure we're in bounds
			d_met = float(data['d'] / 4.0) / 1000.		# distance in meters

			# increment number of scans and get the updated average - this should work
			scan[a_deg][0] = (scan[a_deg][0]*scan[a_deg][1] + d_met) / (scan[a_deg][1]+1)
			scan[a_deg][1] += 1

			scan_complete = False

# returns an array of all our distances (one for each angle)
def dict_to_array():
	global scan
	out = []
	for i in range(0, 360):
		out.append(scan[i][0])
	return out

def clear_scan():
	global scan
	for i in range(0, 360):
		scan[i] = [0, 0]

# MQTT subscribes
client = mqtt.Client()
client.on_message = on_message
client.connect("mqtt.eclipse.org",1883, 60)
client.loop_start()
client.subscribe("lidar/point")
client.subscribe("lidar/start_scan")


rospy.init_node('scan_publisher')
scan_pub = rospy.Publisher('/scan', LaserScan, queue_size=50)



while 1:
	# just depends how fast we want to update TF, shouldn't matter really
	time.sleep(0.5)

	if scan_complete:
		scan_complete = False

		if points_in_scan < POINTS_THRESHOLD:
			rospy.logerr("Bad scan: not enough points - skipping publish")
			clear_scan()
			continue

		if VERBOSE:
			rospy.loginfo("Scan COMPLETE")
			rospy.loginfo("Got: "+str(points_in_scan))
			# rospy.loginfo("Got: "+str(points_in_scan)+"/"+str(seq_recv))
	
		cur_time = rospy.Time.now()

		msg = LaserScan()

		msg.header.stamp = cur_time
		msg.header.frame_id = 'lidar_frame' # for now, assume laser_link is base_link
		msg.angle_min = 0
		msg.angle_max = 2*3.14159
		msg.angle_increment = (msg.angle_max-msg.angle_min) / 360.
		msg.scan_time = 2
		msg.time_increment = msg.scan_time / 360.
		msg.range_min = 0.0
		msg.range_max = 20.0

		msg.ranges = dict_to_array()
		msg.intensities = []
		for i in range(0, 360):
			msg.intensities.append(12)

		scan_pub.publish(msg)

		clear_scan()