#!/usr/bin/env python
import paho.mqtt.client as mqtt
import time
import json
import math

# pseudo code:
#	1. WAIT for lidar/start_scan message, then pause so map update occurs:
	#	1. recv mqtt message from pixycam
	#	2. if object, make that our setpoint and send move_base the command
	#	3. if not, then set move_base command far away

WAIT = 0
SEND = 1

# usually, wait, unless we need to update the goal
MODE = WAIT

# local coordinates!
NAV_GOAL = [0, 0, 0]

convergence_threshold = 0.10

def on_message(client, userdata, message):
	global MODE, NAV_GOAL, convergence_threshold

	if message.topic == 'lidar/start_scan':
		print("[RECV] lidar/start_scan - waiting for map update then sending nav_goal", NAV_GOAL)
		MODE = SEND

	if message.topic == 'christos/pixycam/data':
		print("[RECV] christos/pixycam/data")

		# parse JSON and take this object
		json_data = json.loads(message.payload)

		dist = float(json_data['distance'])
		NAV_GOAL[2] = int(json_data['identity'])

		# check if we found an object, if so, update our setpoint (LOCAL coordinates)
		if NAV_GOAL[2] != 2:
			if abs(dist) < convergence_threshold:
				print("[CONVERGED] reached destination")
				NAV_GOAL[0] = 1000 # dummy values to show we converged
				NAV_GOAL[1] = 1000
			else:
				print("[UPDATE] nav_goal from Pixy [%f %f]"%(dist, float(json_data['theta'])))
				# update setpoint
				NAV_GOAL[0] = 10*dist
				NAV_GOAL[1] = float(json_data['theta'])
		print("exit christos")
# MQTT subscribes
client = mqtt.Client()
client.on_message = on_message
client.connect("mqtt.eclipse.org", 1883, 60)
client.loop_start()
client.subscribe("lidar/start_scan", qos=2)
client.subscribe("christos/pixycam/data", qos=2)

while True:

	if MODE == SEND:
		# wait for map update - but send nav_goal first
		time.sleep(3)

		###################################################################################################
		### FRONTIER_NAV ##################################################################################
		###################################################################################################
		if NAV_GOAL[0] == 0 and NAV_GOAL[1] == 0:
			print("[PUBLISH] goal/pos - Frontier nav_goal")
			goal_string = "{\"dist\": %f, \"angle\": %f, \"id\": %d}"%(25, 0, 2) # go 1M diagonally
			print(goal_string)
			client.publish('goal/pos', goal_string, qos=2)
		
		###################################################################################################
		### CONVERGED #####################################################################################
		###################################################################################################
		elif NAV_GOAL[0] == 1000 and NAV_GOAL[1] == 1000:
			print("[PUBLISH] goal/pos - don't move, we're at destination")
			goal_string = "{\"dist\": %f, \"angle\": %f, \"id\": %d}"%(0, 0, NAV_GOAL[2])
			print(goal_string)
			client.publish('goal/pos', goal_string, qos=2)

		###################################################################################################
		### PIXY_NAV_GOAL #################################################################################
		###################################################################################################
		else:
			print("[PUBLISH] goal/pos - Pixy nav_goal")
			goal_string = "{\"dist\": %f, \"angle\": %f, \"id\": %d}"%(NAV_GOAL[0], NAV_GOAL[1], NAV_GOAL[2])
			print(goal_string)
			client.publish('goal/pos', goal_string, qos=2)

		# reset values and wait for next nav_goal
		NAV_GOAL = [0, 0, 0]
		MODE = WAIT