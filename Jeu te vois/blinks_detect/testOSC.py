# Import needed modules from osc4py3
from osc4py3.as_eventloop import *
from osc4py3 import oscbuildparse
import time



# Start the system.
osc_startup()

# Make client channels to send packets.
osc_udp_client("127.0.0.1", 5000, "pd")


# enable user1 action
# OSC send: /user2 1
msg = oscbuildparse.OSCMessage("/user2", ",i", [1])
osc_send(msg, "pd")
osc_process()

# turn ON
# OSC send: /user1 1
msg = oscbuildparse.OSCMessage("/user1", ",i", [1])
osc_send(msg, "pd")
osc_process()

# wait
time.sleep(2.0)

# turn OFF before the 5 seconds
# OSC send: /user1 -1
msg = oscbuildparse.OSCMessage("/user1", ",i", [-1])
osc_send(msg, "pd")
osc_process()

# wait
time.sleep(2.0)

# turn ON again
# OSC send: /user1 1
msg = oscbuildparse.OSCMessage("/user1", ",i", [1])
osc_send(msg, "pd")
osc_process()


# wait
time.sleep(2.0)

# force turn OFF
# OSC send: /user2 -1
msg = oscbuildparse.OSCMessage("/user2", ",i", [-1])
osc_send(msg, "pd")
osc_process()




osc_terminate()