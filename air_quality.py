#! /usr/bin/python3

# Designed to be run via crontab

import adafruit_pm25
import scrollphathd
from scrollphathd.fonts import font5x7
import busio
import board
import time

t0 = time.time()

# pm sensor using i2c bus, physical pins 3 and 5

reset_pin = None
i2c = busio.I2C(board.SCL, board.SDA, frequency=100000)
pm25 = adafruit_pm25.PM25_I2C(i2c, reset_pin)

# now read the air quality data, many more are possible to log if you choose

aqdata = pm25.read()
pm25_std = aqdata["pm25 standard"]
pm10_std = aqdata["pm10 standard"]

#   Log result to a file

fp = open("/home/pi/air_quality.log", "a")
outs = ("{0:s}, pm1.0: {1:0.0f}, pm2.5: {2:0.0f},").format(time.strftime("%Y/%m/%d %H:%M:%S",time.localtime()), pm10_std, pm25_std)
fp.write(outs)
fp.close()

# display the 2.5 micron results

outs = ("{0:0.0f}").format(pm25_std)
scrollphathd.set_brightness(0.2)
scrollphathd.rotate(180)
scrollphathd.clear()
scrollphathd.write_string(outs, x=1, y=0, font=font5x7)
scrollphathd.show()

# keep the display up until the next cron job should run
# with a few seconds of dead display time

dt = 295-(time.time()-t0)
time.sleep(dt)
