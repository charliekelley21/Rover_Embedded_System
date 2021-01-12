#!/usr/bin/env python
import paho.mqtt.client as mqtt

# ros stuff
import rospy
import tf2_ros
import tf_conversions
import geometry_msgs.msg

import time, json, math

# global - track these over time
odom_x = 0
odom_y = 0
odom_angle = 0


tot_dist_moved = 0


def on_message(client, userdata, message):
	global tot_dist_moved

	if message.topic == 'rover/odom':
		# extract the data
		try:
			data = json.loads(message.payload.decode("utf-8"))

			dist = data['distance']
			theta_move = data['theta_move']
			theta_arm = data['theta_arm']
			# if theta_arm == 0:
			# 	dist *= 2

			pub_odom(dist, theta_move, theta_arm)
			tot_dist_moved += dist

			print("Total Distance Moved (ft):", tot_dist_moved*3.1)

			# publish message to tell the lidar to start scan
			client.publish("lidar/start_scan", 'start')

			# give odometry to SLAM algorithm
			# pub_odom(data['dx'], data['dy'], data['dangle'])

		except:
			rospy.logerr("Odometry JSON is malformed - skipping scan")

def pub_odom(dist, dangle_m, dangle_arm):
	global odom_x
	global odom_y
	global odom_angle

	# rotate, move, rotate
	odom_angle += dangle_m
	odom_x += dist*math.cos(odom_angle)
	odom_y += dist*math.sin(odom_angle)
	odom_angle += dangle_arm

	# if dx != 0:
	# 	print("Relative to rover frame:", dx, dy)
	# d_odom_x = dx*math.cos(odom_angle) + dy*math.sin(odom_angle)
	# d_odom_y = dx*math.sin(odom_angle) + dy*math.cos(odom_angle)
	
	# if dx != 0:
	# 	print("Relative to global frame:", d_odom_x, d_odom_y)

	# update odometry
	# odom_x += d_odom_x
	# odom_y += d_odom_y
	# odom_angle = odom_angle + dangle + dangle_arm

	# PUBLISH tf for odometry
	br = tf2_ros.TransformBroadcaster()
	t = geometry_msgs.msg.TransformStamped()
	t.header.stamp = rospy.Time.now()
	t.header.frame_id = "odom"
	t.child_frame_id = "base_link"
	t.transform.translation.x = odom_x
	t.transform.translation.y = odom_y
	t.transform.translation.z = 0
	q = tf_conversions.transformations.quaternion_from_euler(0, 0, odom_angle)
	t.transform.rotation.x = q[0]
	t.transform.rotation.y = q[1]
	t.transform.rotation.z = q[2]
	t.transform.rotation.w = q[3]

	if dist != 0:
		print(odom_x, odom_y, odom_angle)
	br.sendTransform(t)

	t.header.stamp = rospy.Time.now()
	t.header.frame_id = "base_link"
	t.child_frame_id = "lidar_frame"
	t.transform.translation.x = 0
	t.transform.translation.y = 0
	t.transform.translation.z = 0
	q = tf_conversions.transformations.quaternion_from_euler(0, 0, 0)
	t.transform.rotation.x = q[0]
	t.transform.rotation.y = q[1]
	t.transform.rotation.z = q[2]
	t.transform.rotation.w = q[3]

	br.sendTransform(t)

	

client = mqtt.Client()
client.on_message = on_message
client.connect("mqtt.eclipse.org", 1883, 60)
client.loop_start()
client.subscribe("rover/odom")

rospy.init_node('odom_publisher')


while 1:
	time.sleep(0.5)

	# publish something, just to keep the transform
	pub_odom(0, 0, 0)
