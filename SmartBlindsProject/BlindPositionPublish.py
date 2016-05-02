import paho.mqtt.client as mqtt

import time
from ISStreamer.Streamer import Streamer

streamer = Streamer(bucket_name="SmartBlindsTracker", bucket_key="SmartBlindsTracker", access_key="qfMo5ionZT6MTt6BQnobVfRuwEkhYqrt")

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("blindsPosition")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    streamer.log("BlindsPosition", str(msg.payload))

client = mqtt.Client()

client.connect("192.168.137.215", 1883, 60)
# client.publish("outTopic", '32')
client.on_connect = on_connect
client.on_message = on_message
client.loop_forever()
