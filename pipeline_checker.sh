#!/bin/bash

FAILED="failed"
OUTPUT="$(curl http://127.0.0.1:8154/go/pipelines.json)"
if echo "$OUTPUT" | grep -q "$FAILED"; then
  mosquitto_pub -h {YOUR_MQTT_HOST} -p 1883 -t {YOUR_TOPIC_NAME} -m "1";
else
  mosquitto_pub -h {YOUR_MQTT_HOST} -p 1883 -t {YOUR_TOPIC_NAME} -m "0";
fi

