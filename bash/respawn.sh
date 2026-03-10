#!/usr/bin/env bash

xdotool key X
sleep .25
xdotool key super+e
xdotool key super+space
sleep .5
xdotool key super+w
xdotool key super+equal
for i in {1..35}; do
  xdotool key super+plus
done
xdotool key alt+z
xdotool key alt+z
