#!/usr/bin/env python
import os, sys, math, heapq, time, json
import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import numpy as np

# assume we don't want to plot
__plot__ = False

# start at origin
OBS = []
CUR_POS = [100, 100, 0] # 0 degrees means we point to +x dir [ measured in CM ]
GOAL_POS = [100, 100, 0] # third field is goal identity

class AStar:
    """AStar set the cost + heuristics as the priority
    """
    def __init__(self, s_start, s_goal, heuristic_type):
        self.s_start = s_start
        self.s_goal = s_goal
        self.heuristic_type = heuristic_type

        self.u_set = [(-1, 0), (-1, 1), (0, 1), (1, 1),
                        (1, 0), (1, -1), (0, -1), (-1, -1)]
        self.obs = OBS  # position of obstacles

        self.OPEN = []  # priority queue / OPEN set
        self.CLOSED = []  # CLOSED set / VISITED order
        self.PARENT = dict()  # recorded parent
        self.g = dict()  # cost to come

    def get_obs(self):
        return self.obs

    def set_obs(self):
        self.obs = OBS

    def searching(self):
        """
        A_star Searching.
        :return: path, visited order
        """
        self.PARENT[self.s_start] = self.s_start
        self.g[self.s_start] = 0
        self.g[self.s_goal] = float("inf")#math.inf
        heapq.heappush(self.OPEN,
                       (self.f_value(self.s_start), self.s_start))

        while self.OPEN:
            _, s = heapq.heappop(self.OPEN)
            self.CLOSED.append(s)

            if s == self.s_goal:  # stop condition
                break

            for s_n in self.get_neighbor(s):
                new_cost = self.g[s] + self.cost(s, s_n)

                if s_n not in self.g:
                    self.g[s_n] = float("inf")#math.inf

                if new_cost < self.g[s_n]:  # conditions for updating Cost
                    self.g[s_n] = new_cost
                    self.PARENT[s_n] = s
                    heapq.heappush(self.OPEN, (self.f_value(s_n), s_n))

        return self.extract_path(self.PARENT), self.CLOSED

    def searching_repeated_astar(self, e):
        """
        repeated A*.
        :param e: weight of A*
        :return: path and visited order
        """

        path, visited = [], []

        while e >= 1:
            p_k, v_k = self.repeated_searching(self.s_start, self.s_goal, e)
            path.append(p_k)
            visited.append(v_k)
            e -= 0.5

        return path, visited

    def repeated_searching(self, s_start, s_goal, e):
        """
        run A* with weight e.
        :param s_start: starting state
        :param s_goal: goal state
        :param e: weight of a*
        :return: path and visited order.
        """

        g = {s_start: 0, s_goal: float("inf")}
        PARENT = {s_start: s_start}
        OPEN = []
        CLOSED = []
        heapq.heappush(OPEN,
                       (g[s_start] + e * self.heuristic(s_start), s_start))

        while OPEN:
            _, s = heapq.heappop(OPEN)
            CLOSED.append(s)

            if s == s_goal:
                break

            for s_n in self.get_neighbor(s):
                new_cost = g[s] + self.cost(s, s_n)

                if s_n not in g:
                    g[s_n] = math.inf

                if new_cost < g[s_n]:  # conditions for updating Cost
                    g[s_n] = new_cost
                    PARENT[s_n] = s
                    heapq.heappush(OPEN, (g[s_n] + e * self.heuristic(s_n), s_n))

        return self.extract_path(PARENT), CLOSED

    def get_neighbor(self, s):
        """
        find neighbors of state s that not in obstacles.
        :param s: state
        :return: neighbors
        """

        return [(s[0] + u[0], s[1] + u[1]) for u in self.u_set]

    def cost(self, s_start, s_goal):
        """
        Calculate Cost for this motion
        :param s_start: starting node
        :param s_goal: end node
        :return:  Cost for this motion
        :note: Cost function could be more complicate!
        """

        if self.is_collision(s_start, s_goal):
            return float("inf")#math.inf

        return math.hypot(s_goal[0] - s_start[0], s_goal[1] - s_start[1])

    def is_collision(self, s_start, s_end):
        """
        check if the line segment (s_start, s_end) is collision.
        :param s_start: start node
        :param s_end: end node
        :return: True: is collision / False: not collision
        """

        if s_start in self.obs or s_end in self.obs:
            return True

        if s_start[0] != s_end[0] and s_start[1] != s_end[1]:
            if s_end[0] - s_start[0] == s_start[1] - s_end[1]:
                s1 = (min(s_start[0], s_end[0]), min(s_start[1], s_end[1]))
                s2 = (max(s_start[0], s_end[0]), max(s_start[1], s_end[1]))
            else:
                s1 = (min(s_start[0], s_end[0]), max(s_start[1], s_end[1]))
                s2 = (max(s_start[0], s_end[0]), min(s_start[1], s_end[1]))

            if s1 in self.obs or s2 in self.obs:
                return True

        return False

    def f_value(self, s):
        """
        f = g + h. (g: Cost to come, h: heuristic value)
        :param s: current state
        :return: f
        """

        return self.g[s] + self.heuristic(s)

    def extract_path(self, PARENT):
        """
        Extract the path based on the PARENT set.
        :return: The planning path
        """

        path = [self.s_goal]
        s = self.s_goal

        while True:
            s = PARENT[s]
            path.append(s)

            if s == self.s_start:
                break

        return list(path)

    def heuristic(self, s):
        """
        Calculate heuristic.
        :param s: current node (state)
        :return: heuristic function value
        """

        heuristic_type = self.heuristic_type  # heuristic type
        goal = self.s_goal  # goal node

        if heuristic_type == "manhattan":
            return abs(goal[0] - s[0]) + abs(goal[1] - s[1])
        else:
            return math.hypot(goal[0] - s[0], goal[1] - s[1])

def inflate_obs():
    # only try this if we have obstacles
    if len(OBS) == 0:
        return -1

    # for each point, add neighbors, do this twice
    for point in OBS:

        # compute neighbors
        up_pt = [point[0], point[1] + 1]
        dn_pt = [point[0], point[1] - 1]
        lf_pt = [point[0] - 1, point[1]]
        rt_pt = [point[0] + 1, point[1]]

        neighbors = [up_pt, dn_pt, lf_pt, rt_pt]

        # only add if this is new
        for n_point in neighbors:
            # make sure we're in bounds
            if n_point[0] > -1 and n_point[0] < 201 and n_point[1] > -1 and n_point[1] < 201:
                # check if each of these is in OBS, else add it
                in_OBS = False
                for t_p in OBS:
                    if t_p == n_point:
                        in_OBS = True
                        break
                if in_OBS == False:
                    OBS.append(n_point)

# every time we get a map, compute the path to our goal
def on_message(client, userdata, message):
    global CUR_POS, GOAL_POS, OBS

    if message.topic == 'rover/cur_pos':
        data = json.loads(message.payload.decode('utf-8'))
        CUR_POS = [100 + 10*data['x'], 100 + 10*data['y'], data['a']]
        print("[UPDATED] cur_pose:", CUR_POS)

    if message.topic == 'goal/pos':
        data = json.loads(message.payload.decode('utf-8'))
        GOAL_POS[0] = int(CUR_POS[0] + data['dist']*math.cos(data['angle'] + CUR_POS[2]))
        GOAL_POS[1] = int(CUR_POS[1] + data['dist']*math.sin(data['angle'] + CUR_POS[2]))
        GOAL_POS[2] = data['id']
        print("[UPDATED] goal_pose:", GOAL_POS)

    if message.topic == 'lidar/map':
        data = json.loads(message.payload.decode('utf-8'))
        
        # convert map to list of obstacles, store in global variable
        OBS = [] # reset list when we get a new map

        map_w = data["width"]
        map_h = data["height"]

        # add map edges
        for i in range(map_w):
            OBS.append([i, 0])
            OBS.append([i, map_h])
        for i in range(map_h):
            OBS.append([0, i])
            OBS.append([map_w, i])

        # add obstacles from map
        for row in range(map_h):
            for col in range(map_w):
                ind = row*map_w + col
                if data["data"][ind] > 0:
                    OBS.append([row, col])
        print("MAP - about to inflate")
        # call our clever function to inflate obstacles (operate on global variable)
        # call TWICE to inflate by 20cm to be safe
        # inflate_obs()
        # inflate_obs()

        print("[UPDATED] map")
        print("[COMPUTE] path_plan (start, end):", CUR_POS, GOAL_POS)

        # compute path!
        astar = AStar((CUR_POS[0], CUR_POS[1]), (GOAL_POS[0], GOAL_POS[1]), "euclidean")
        astar.set_obs()
        path, visited = astar.searching()

        print("[RESULT] path:", path)

        if __plot__:
            # plot
            plt.figure()
            p = np.array(path)
            o = np.array(OBS)
            plt.scatter(p[:, 0], p[:, 1])
            plt.scatter(o[:, 0], o[:, 1])
            plt.show()

        # if we computed a path, then send the movement command
        if path != None:
            dx = 0
            dy = 0
            if len(path) > 5: # 5 corresponds to 50 cm!
                dx = 1.0*(path[-5][0] - CUR_POS[0])
                dy = 1.0*(path[-5][1] - CUR_POS[1])
            else:
                dx = 1.0*(path[0][0] - CUR_POS[0])
                dy = 1.0*(path[0][1] - CUR_POS[1])

            # if we're going to an object/dropoff, go all the way
            if GOAL_POS[2] < 2:
                dx = 1.0*(path[0][0] - CUR_POS[0])
                dy = 1.0*(path[0][1] - CUR_POS[1])

            dist = 0.1*math.sqrt(1.0*(dx*dx + dy*dy))
            theta = math.atan2(dy, dx)


            print("[SENDING] movement_cmd: [%f %f]"%(dist, theta))
            pub_string = "{\"sequence\": %d, \"distance\": %f, \"theta\": %f, \"identity\": %d}"%(0, dist, theta - CUR_POS[2], GOAL_POS[2])
            print(pub_string)
            client.publish("rover/movement", pub_string)
        else:
            print("[ERROR] no path (ok if we're at goal")

# mqtt connection
client = mqtt.Client()
client.on_message = on_message
client.connect("mqtt.eclipse.org", 1883, 60)
client.loop_start()

client.subscribe("goal/pos", qos=2) # local coordinates
client.subscribe("rover/cur_pos", qos=2) # x, y
client.subscribe("lidar/map", qos=2)

while True:
    time.sleep(0.5)