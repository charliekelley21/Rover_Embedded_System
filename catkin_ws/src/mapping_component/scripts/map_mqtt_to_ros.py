#!/usr/bin/env python
import paho.mqtt.client as mqtt
import time
import json

# ros stuff
import rospy
from nav_msgs.msg import OccupancyGrid


# ros publisher, global
rospy.init_node("map_mqtt_to_ros")
pub = rospy.Publisher('rover/map_received', OccupancyGrid, queue_size=10)


def on_message(client, userdata, message):

	if message.topic == 'lidar/map':
		rospy.loginfo("MAP_MQTT_TO_ROS: recevied map")

		# parse and republish as ROS message
		json_map = json.loads(message.payload)
		# print(json_map)
		map_ = OccupancyGrid()

		map_.header.seq = json_map["seq"]
		map_.header.stamp.secs = json_map["stamp_secs"]
		map_.header.stamp.nsecs = json_map["stamp_nsecs"]
		map_.header.frame_id = json_map["frame_id"]

		map_.info.map_load_time.secs = json_map["map_load_secs"]
		map_.info.map_load_time.nsecs = json_map["map_load_nsecs"]
		map_.info.width = json_map["width"]
		map_.info.height = json_map["height"]
		map_.info.resolution = json_map["resolution"]

		map_.info.origin.position.x = json_map["px"]
		map_.info.origin.position.y = json_map["py"]
		map_.info.origin.position.z = json_map["pz"]
		map_.info.origin.orientation.x = json_map["qx"]
		map_.info.origin.orientation.y = json_map["qy"]
		map_.info.origin.orientation.z = json_map["qz"]
		map_.info.origin.orientation.w = json_map["qw"]

		map_.data = json_map["data"]

		pub.publish(map_)


# define our MQTT client
client = mqtt.Client()
client.on_message = on_message
client.connect("mqtt.eclipse.org", 1883, 60)
client.loop_start()
client.subscribe("lidar/map")

while 1:
	time.sleep(0.5)