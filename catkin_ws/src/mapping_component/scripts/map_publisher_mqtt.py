#!/usr/bin/env python
import paho.mqtt.client as mqtt
import time
import json

# ros stuff
import rospy, tf
from nav_msgs.msg import OccupancyGrid

# mqtt connection
client = mqtt.Client()
client.connect("mqtt.eclipse.org", 1883, 60)

rospy.init_node('mqtt_map_publisher')

l = tf.TransformListener()

def rover_get_cur_pos():

	transform = [0, 0, 0]

	try:
		transform = l.lookupTransform('map', 'base_link', rospy.Time.now())
		rospy.loginfo(transform)
	except:
		pass

	return transform

def pub_callback(data):

	seq = data.header.seq
	time = data.header.stamp
	frame = data.header.frame_id
	map_load = data.info.map_load_time
	width = data.info.width
	height = data.info.height
	resolution = data.info.resolution
	px = data.info.origin.position.x
	py = data.info.origin.position.y
	pz = data.info.origin.position.z
	qx = data.info.origin.orientation.x
	qy = data.info.origin.orientation.y
	qz = data.info.origin.orientation.z
	qw = data.info.origin.orientation.w

	map_string = "{\"seq\": %d, \"stamp_secs\": %d, \"stamp_nsecs\": %d, \"frame_id\": \"%s\", "%(seq, time.secs, time.nsecs, frame) + \
					"\"map_load_secs\": %d, \"map_load_nsecs\": %d, \"width\": %d, \"height\": %d, "%(map_load.secs, map_load.nsecs, width, height) +	\
					"\"px\": %f, \"py\": %f, \"pz\": %f, \"qx\": %f, \"qy\": %f, \"qz\": %f, "%(px, py, pz, qx, qy, qz) + \
					"\"qw\": %f, \"resolution\": %f, \"data\": [%s]}" %(qw, resolution, str(data.data)[1:-1])

	# print(map_string)

	client.publish("lidar/map", map_string)
	rospy.loginfo("MAP_PUBLISHER_MQTT: published map to mqtt (size = "+str(len(map_string))+")")

	# todo: publish current position for navigator (MQTT)
	x, y, a = rover_get_cur_pos()
	pub_string = "{\"sequence\": %d, \"x\": %f, \"y\": %f, \"a\": %f}"%(0, x, y, a)
	client.publish("rover/cur_pos", pub_string, qos=2, retain=True)

rospy.Subscriber('/map', OccupancyGrid, pub_callback)

# rospy.Subscriber('/odom')
rospy.spin()