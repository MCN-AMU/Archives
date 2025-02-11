# ======================================================================================================================
# detect_blinks.py
# version 2.1
#
# based on "Eye blink detection with OpenCV, Python, and dlib" by Adrian Rosebrock
# @ https://www.pyimagesearch.com/2017/04/24/eye-blink-detection-opencv-python-dlib/
#
#
# usage
# -----
#
# 	-h or --help 				show the help message and exit
# 	-u or --user 				the OSC user name
#	-c or --camera 				the camera number
#	-v or --video 				the path to input video file, if not set use camera input
#	-p or --shape-predictor 	the path to the facial landmark predictor data, by default 'shape_predictor_68_face_landmarks.dat'
#
# simple example: python3 ./detect_blinks.py -u user1 -c0
# ======================================================================================================================




# import the necessary packages
from scipy.spatial import distance as dist
from imutils.video import FileVideoStream
from imutils.video import VideoStream
from imutils import face_utils
import numpy as np
import argparse
import imutils
import time
import dlib
import cv2
import os
import platform




hostname = os.uname()[1]
osarch = os.uname()[4]

print("[INFO] running on {} platform - arch {}".format(platform.system(), osarch ))
print("[INFO] hostname '{}'".format(hostname))



# ----------------------------------------------------------------------------------------------------------------------
# Raspberry Pi PiCamera module suppprt

usePiCameraModule = False
RPI = False

if osarch == 'armv7l':
	print("[INFO] running on Raspberry PI")
	RPI = True
	# PiCamera module: https://picamera.readthedocs.io/en/release-1.10/index.html
	# imutils video stream options : https://github.com/jrosebr1/imutils/blob/master/imutils/video/videostream.py
	usePiCameraModule = False

if usePiCameraModule:
	print("[INFO] using native PiCamera stream instead imutils video stream")
	from picamera.array import PiRGBArray
	from picamera import PiCamera




# ----------------------------------------------------------------------------------------------------------------------
# Capture KeyboardInterrupt (Ctrl+C) or kill SIGINT signal
import signal
import sys

RUNNING = True
def signal_handler(signal, frame):
	global RUNNING
	RUNNING = False

signal.signal(signal.SIGINT, signal_handler)



# ----------------------------------------------------------------------------------------------------------------------
# OSC SUPPORT
# you need to install the osc4py3 module @ https://osc4py3.readthedocs.io/en/latest/ : pip install --user osc4py3
from osc4py3 import as_eventloop as OSC
from osc4py3 import oscbuildparse


# open a new OSC connexion to the specific remote host and port
# by default connect locally on port 54321
def osc_open(remote_host="127.0.0.1", remote_port=54321):
	# start the system.
	OSC.osc_startup()
	# make client channels to send packets.
	OSC.osc_udp_client(remote_host, remote_port, "osc_client")
	print("[INFO] open OSC connection to host `{}` on port {}".format(remote_host,remote_port))


# close OSC connexion
def osc_close():
	# properly close the system.
	OSC.osc_terminate()
	print("[INFO] close OSC connection")

# send OSC message
def osc_send(address, arguments=None, types=None):

	if arguments is None:
		arguments = []
	elif not isinstance(arguments, list):
			arguments = [arguments]

	# build message and send it (auto )
	msg = oscbuildparse.OSCMessage(address, types, arguments)
	OSC.osc_send(msg, "osc_client")
	OSC.osc_process()
	if VERBOSE:
		print("send OSC message: {} {}".format(address,arguments))



# ----------------------------------------------------------------------------------------------------------------------
# ARDUINO SUPPORT
# you need to install the pySerial @ pyserial.sourceforge.net : pip install --user pySerial
import serial
import serial.tools.list_ports
import threading



# open board on port '/dev/cu.usbmodem14201'
# YOUR SERIAL PORT SHOULD BE DIFFERENT!
port = '/dev/cu.usbmodem14101'
baud = 9600


arduino = None
connected = False

# opens serial port and sets the data rate (baud) for serial transmissions
def arduino_open():
	global arduino, connected

	try:
		arduino = serial.Serial(port, baud)
		connected = True
		print("[INFO] open Arduino on port `{}` with baudrate set to {} ".format(port,baud))
	except serial.SerialException as e:
		# Cannot connect to Arduino: display message, list connected boards
		print("\n[ERROR] FAIL TO CONNECT TO ARDUINO ON PORT:\n"+str(e)+"\n")
		list_boards()


# close serial port if connected
def arduino_close():
	global arduino
	if arduino is not None:
		arduino.close()


# send value to serial port
def send_to_arduino(value):
	global arduino
	if arduino is not None:
		arduino.write(value.encode('ascii'))
		if VERBOSE:
			print("Send '{}' to Arduino".format(value))


# read and returns data from serial port
# or None
def read_from_arduino():
	global arduino
	if arduino is not None:
		return arduino.readline().decode()
	else:
		return None


# list avaoilable serial port
def list_boards():
	print("[INFO] ------------------------")
	print("[INFO] ARDUINO AVAILABLE BOARDS")
	print("[INFO] ------------------------")
	print("\n")

	ports = serial.tools.list_ports.comports()
	for port, desc, hwid in sorted(ports):
		print("{}: {} [{}]".format(port, desc, hwid))

	print("\n")
# ----------------------------------------------------------------------------------------------------------------------


def eye_aspect_ratio(eye):
	# compute the euclidean distances between the two sets of
	# vertical eye landmarks (x, y)-coordinates
	A = dist.euclidean(eye[1], eye[5])
	B = dist.euclidean(eye[2], eye[4])

	# compute the euclidean distance between the horizontal
	# eye landmark (x, y)-coordinates
	C = dist.euclidean(eye[0], eye[3])

	# compute the eye aspect ratio
	ear = (A + B) / (2.0 * C)

	# return the eye aspect ratio
	return ear


# current time in milliseconds
def time_in_sec():
	return int(time.time())

# current time in milliseconds
def time_in_ms():
	return int(round(time.time() * 1000))


 
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-s", "--shape-predictor", type=str, default="shape_predictor_68_face_landmarks.dat", help="Path to facial landmark predictor")
ap.add_argument("-v", "--video", type=str, default="", help="Path to input video file")
ap.add_argument("-u", "--user", type=str, default="user", help="OSC user name")
ap.add_argument("-c", "--camera", type=int, default=0, help="Camera number")
ap.add_argument("-o", "--output", type=str, default=os.environ['PWD'], help="Path to save the camera image")
ap.add_argument("-t", "--freq", type=int, default=0, help="Camera image output frequency in seconds, 0 means disable output (decrease performance)")
ap.add_argument("-a", "--host", type=str, default="127.0.0.1", help="OSC remote address IP or host name")
ap.add_argument("-p", "--port", type=int, default=5000, help="OSC remote port")
ap.add_argument("-d", "--verbose", action='store_true', help="Printout all events (decrease performance)")
ap.add_argument("--nogui", action='store_true', help="Disable UI window")
args = vars(ap.parse_args())


VERBOSE = False
if args["verbose"]:
	print("[INFO] debug mode enable")
	VERBOSE = True

WINDOW_OUTPUT = True
if args["nogui"]:
	print("[INFO] disable output window (no GUI mode)")
	WINDOW_OUTPUT = False



 
# define two constants, one for the eye aspect ratio to indicate
# blink and then a second constant for the number of consecutive
# frames the eye must be below the threshold
EYE_AR_THRESH = 0.23
EYE_AR_CONSEC_FRAMES = 3

# initialize the frame counters and the total number of blinks
COUNTER = 0
TOTAL = 0

# initialize dlib's face detector (HOG-based) and then create
# the facial landmark predictor
print("[INFO] loading facial landmark predictor...")
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(args["shape_predictor"])

# grab the indexes of the facial landmarks for the left and
# right eye, respectively
(lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]

# start the video stream thread
print("[INFO] starting video stream thread...")
if args["video"]:
	vs = FileVideoStream(args["video"]).start()
	fileStream = True
else:
	if RPI:
		size = (320, 240)
		fps = 15
		if usePiCameraModule:
			vs = PiCamera()
			vs.resolution = size
			vs.framerate = fps
			vsbuf = PiRGBArray(vs, size=size)
		else:
			vs = VideoStream(usePiCamera=True,resolution=size,framerate=fps).start()
	else:
		vs = VideoStream(src=args["camera"]).start()
	fileStream = False


CAMERA=args["camera"]
USERNAME=args["user"]
print("[INFO] run as user '{}'...".format(USERNAME))



# initialize external communications
osc_open(args["host"],args["port"])
if not RPI:
	arduino_open()
	# listen to arduino async
	thread = threading.Thread(target=read_from_arduino)
	thread.start()



# define and start (if needed) image saver thread
INTERVAL=args["freq"]
FILENAME=args["output"]+"/camera_"+USERNAME+".jpg"
dumpimage=INTERVAL>0
image=None

def save():
	global INTERVAL,FILENAME,VERBOSE
	global timer, image

	if image is not None:
		cv2.imwrite(FILENAME, image)
		if VERBOSE:
			print("save frame to {}".format(FILENAME))

	# execute periodically
	timer = threading.Timer(INTERVAL, save)
	timer.start()


if dumpimage:
	print("[INFO] starting image saver timer...")
	timer = threading.Timer(INTERVAL, save)
	timer.start()



if WINDOW_OUTPUT:
	print("\nTYPE 'q' TO QUIT ...\n")


# send welcome message
send_to_arduino('W')
osc_send("/connect", USERNAME)



# define eyes status
# -1 = NO DETECTED
#  0 = CLOSED
#  1 = OPENED AND ACCEPTED
#  2 = OPENED BUT REJECTED (SEE EYE_AR_CONSEC_FRAMES ABOVE)
EYE_STATUS = -1

# store current time
CURRENT_TIME = time_in_ms()
TIME_ELAPSED = 0



# wait fo initialisation
time.sleep(1.0)
forcequit = False


WINDOW_NAME = "Blinks detect | User: {} | Camera: {}".format(USERNAME,CAMERA)
cv2.namedWindow(WINDOW_NAME, cv2.WINDOW_NORMAL);
cv2.moveWindow(WINDOW_NAME, 10+(840*CAMERA), 50);

# make window active (OSX only)
if platform.system() == 'Darwin':
	os.system('''/usr/bin/osascript -e 'tell app "Finder" to set frontmost of process "Python" to true' ''') 



# loop over frames from the video stream
while RUNNING:

	# if this is a file video stream, then we need to check if
	# there any more frames left in the buffer to process
	if fileStream and not vs.more():
		break

	# grab the frame from the threaded video file stream, resize
	# it, and convert it to grayscale
	# channels)
	if usePiCameraModule:
		vs.capture(vsbuf, format="rgb", use_video_port=True)
		frame = vsbuf.array
		# clear the stream buffer in preparation for the next frame
		vsbuf.truncate(0)
	else:
		frame = vs.read()

	if frame is None:
		continue

	# copy image to save
	if dumpimage:
		image = frame


	# # resize frame (if require)
	if not RPI:
		frame = imutils.resize(frame, width=640)

	# convert it to grayscale
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)


	# detect faces in the grayscale frame
	rects = detector(gray, 0)


	if len(rects) == 0:
		status = -1 # <------ NOT DETECTED


	# loop over the face detections
	for rect in rects:
		# determine the facial landmarks for the face region, then
		# convert the facial landmark (x, y)-coordinates to a NumPy
		# array
		shape = predictor(gray, rect)
		shape = face_utils.shape_to_np(shape)

		# extract the left and right eye coordinates, then use the
		# coordinates to compute the eye aspect ratio for both eyes
		leftEye = shape[lStart:lEnd]
		rightEye = shape[rStart:rEnd]
		leftEAR = eye_aspect_ratio(leftEye)
		rightEAR = eye_aspect_ratio(rightEye)

		# average the eye aspect ratio together for both eyes
		ear = (leftEAR + rightEAR) / 2.5

		# compute the convex hull for the left and right eye, then
		# visualize each of the eyes
		leftEyeHull = cv2.convexHull(leftEye)
		rightEyeHull = cv2.convexHull(rightEye)
		cv2.drawContours(frame, [leftEyeHull], -1, (0, 255, 0), 1)
		cv2.drawContours(frame, [rightEyeHull], -1, (0, 255, 0), 1)

		# check to see if the eye aspect ratio is below the blink
		# threshold, and if so, increment the blink frame counter
		if ear < EYE_AR_THRESH:
			COUNTER += 1
			status = 0 # <------ CLOSED

		

		# otherwise, the eye aspect ratio is not below the blink
		# threshold
		else:
			# if the eyes were closed for a sufficient number of
			# then increment the total number of blinks
			if COUNTER >= EYE_AR_CONSEC_FRAMES:
				TOTAL += 1
				status = 1 # <------ OPENED VALID
			else:
				status = 2 # <------ OPENED REJECTED

			# reset the eye frame counter
			COUNTER = 0


		cv2.putText(frame, "EAR: {:.2f}".format(ear), (10, 150), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)




	# ----------------------------------------------------------------------------------------------------------
	# SEND TO ARDUINO IF STATUS CHANGE
	if status != EYE_STATUS:
		EYE_STATUS = status
		TIME_ELAPSED = time_in_ms() - CURRENT_TIME
		if VERBOSE:
			print("send eyes status: {}".format(EYE_STATUS))
		send_to_arduino(EYE_STATUS)
		osc_send("/"+USERNAME,EYE_STATUS)
		CURRENT_TIME = time_in_ms()
	# ----------------------------------------------------------------------------------------------------------



	sstatus = "not detected" if EYE_STATUS == -1 else ("closed" if EYE_STATUS == 0 else "opened")
	stime = time.strftime("%M:%S.%f", time.gmtime(TIME_ELAPSED/1000.0))

	# draw the total number of blinks on the frame along with
	# the computed eye aspect ratio for the frame
	cv2.putText(frame, "Frame counter: {}".format(COUNTER), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
	cv2.putText(frame, "Blink total: {}".format(TOTAL), (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
	cv2.putText(frame, "Eyes status: {}".format(sstatus), (10, 90), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
	cv2.putText(frame, "Time elapsed: {} ms".format(TIME_ELAPSED), (10, 120), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

 
	# show the frame
	if WINDOW_OUTPUT:
		cv2.imshow(WINDOW_NAME, frame)
		key = cv2.waitKey(1) & 0xFF

	 
		# if the `q` key was pressed, break from the loop
		if key == ord("q"):
			forcequit = True
			break


# close connexions
osc_close()
if connected:
	# ------------------------------------------------------------------------------------------------------------------
	# ARDUINO: close serial port
	connected = False
	arduino_close()
	# ------------------------------------------------------------------------------------------------------------------
	thread.join()

# wait for saver thread completion
if dumpimage:
	timer.cancel()


# wait a bit
if not forcequit:
	time.sleep(3.0)

# do a bit of cleanup
cv2.destroyAllWindows()


if usePiCameraModule:
	vs.close()
else:
	vs.stop()
