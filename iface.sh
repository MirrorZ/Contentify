#!/bin/sh

ifconfig wlan0 down
ifconfig wlan1 down

#iwconfig wlan0 mode monitor
#iwconfig wlan1 mode monitor

ip maddr add 01:00:5e:00:00:10 dev wlan0
ip maddr add 01:00:5e:00:00:10 dev wlan1

ifconfig wlan0 up
ifconfig wlan1 up

