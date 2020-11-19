#!/bin/bash
STR=$(date -d "yesterday" +"%y-%m-%d")
echo $STR
mv /home/pi/air_quality.log /home/pi/$STR-air_quality.log
