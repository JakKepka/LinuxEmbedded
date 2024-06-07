#!/usr/bin/python
import gpiod
import time
import sys
chip = gpiod.Chip(sys.argv[1])
button = chip.get_line(int(sys.argv[2]))
button.request(consumer="its_me", type=gpiod.LINE_REQ_EV_BOTH_EDGES)
while True:
   ev_line = button.event_wait(sec=2)
   if ev_line:
     event = button.event_read()
     print(event)
   else:
     print(".")
     

