import sys
import paho.mqtt.client as mqtt
import Adafruit_DHT
import pyowm
import time
from ISStreamer.Streamer import Streamer

streamer = Streamer(bucket_name="SmartBlindsTracker", bucket_key="SmartBlindsTracker", access_key="qfMo5ionZT6MTt6BQnobVfRuwEkhYqrt")

# Parse command line parameters.
sensor_args = { '11': Adafruit_DHT.DHT11,
				'22': Adafruit_DHT.DHT22,
				'2302': Adafruit_DHT.AM2302 }
if len(sys.argv) == 3 and sys.argv[1] in sensor_args:
	sensor = sensor_args[sys.argv[1]]
	pin = sys.argv[2]
else:
	print 'usage: sudo ./Adafruit_DHT.py [11|22|2302] GPIOpin#'
	print 'example: sudo ./Adafruit_DHT.py 2302 4 - Read from an AM2302 connected to GPIO #4'
	sys.exit(1)
try:
	while True:
		humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)
		temperature = temperature * 9 / 5.0 + 32

		if humidity is not None and temperature is not None:
			print 'Temp={0:0.1f}*  Humidity={1:0.1f}%'.format(temperature, humidity)
			streamer.log("Room Temperature", temperature)
			streamer.log("Room Humidity", humidity)
		else:
			print 'Failed to get reading. Try again!'
			sys.exit(1)
        time.sleep(60)

except KeyboardInterrupt:
    streamer.close()
