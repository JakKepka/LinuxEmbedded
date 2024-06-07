#!/usr/bin/python
import gpiod
import time
import sys
chip = gpiod.Chip(sys.argv[1])
led1 = chip.get_line(int(sys.argv[2]))
led1.request(consumer="its_me", type=gpiod.LINE_REQ_DIR_OUT)
while True:
   led1.set_value(1)
   time.sleep(0.5)
   led1.set_value(0)
   time.sleep(0.5)

