import pyowm
import time
from ISStreamer.Streamer import Streamer

streamer = Streamer(bucket_name="SmartBlindsTracker", bucket_key="SmartBlindsTracker", access_key="qfMo5ionZT6MTt6BQnobVfRuwEkhYqrt")
owm = pyowm.OWM('6cca640ffb4b8dbd89464c3e07795f7e')

try:
    while True:
        observation = owm.weather_at_place('Devon,us')
        w = observation.get_weather()

        w.get_wind()
        w.get_humidity()
        w.get_temperature('fahrenheit')
        print("wind", w.get_wind()['speed'])
        print("humidity", w.get_humidity())
        print("temperature", w.get_temperature('fahrenheit')['temp'])
        print("temperature Object", w.get_temperature('fahrenheit'))

        streamer.log("Wind Speed", w.get_wind()['speed'])
        streamer.log("External Humidity", w.get_humidity())
        streamer.log("External Temperature", w.get_temperature('fahrenheit')['temp'])
        time.sleep(60)

except KeyboardInterrupt:
    streamer.close()
