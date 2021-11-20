import requests
from datetime import datetime

from main.config import Config


def check_car_presence(park_type = "entrance"):
	state = False
	try:
		r = requests.get(f"{Config.IOT_DEVICE_URL}/check-car-presence/?device_key={Config.IOT_DEVICE_KEY}&type={park_type}")
		state = False if int(r.text()) == 0 else True

	except Exception as ex:
		print(f"--clearparking--: {datetime.now()} | check_car_presence exception: {ex}")

	return state


def open_gates(park_type = "entrance", direction = "up"):
	try:
		r = requests.get(f"{Config.IOT_DEVICE_URL}/control/?device_key={Config.IOT_DEVICE_KEY}&type={park_type}&direction={direction}")
		r.text()

	except Exception as ex:
		print(f"--clearparking--: {datetime.now()} | open_gates exception: {ex}")
		return False

	return True


def manage_iot_device(park_type = "entrance"):
	if check_car_presence(park_type):
		if open_gates(park_type):
			print(f"++clearparking++: {datetime.now()} | Opening Gates of {park_type}")
		else:
			print(f"++clearparking++: {datetime.now()} | Unable to open gates of {park_type}")

	else:
		print(f"--clearparking--: {datetime.now()} | Car is not on the road..")