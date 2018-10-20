#!/usr/bin/env python3

from envirophat import light, motion, weather, leds
import time
import atexit


def shutdown():
    print("shutting down")
    leds.off()

atexit.register(shutdown)


if __name__ == "__main__":
    while True:
        t = time.time()
        r, g, b = light.rgb()
        x, y, z = motion.accelerometer()
        # heading = (motion.heading() - north) % 360
        temp_in_celsius = weather.temperature()
        pressure_in_hpa = weather.pressure(unit='hPa')

        print([
            t,
            r, g, b,
            x, y, z, # heading,
            temp_in_celsius, pressure_in_hpa
        ])

        time.sleep(1)
