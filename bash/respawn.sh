#!/usr/bin/env bash

xdotool key super+e
source $HOME/skps/newlook -r
xdotool key super+space
xdotool key super+h
xdotool key X
sleep .5

xdotool key super+w
for i in {1..2}; do
  xdotool key alt+z
done
