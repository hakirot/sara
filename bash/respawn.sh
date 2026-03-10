#!/usr/bin/env bash

xdotool key super+equal
for i in {1..20}; do
  xdotool key alt+x
done

# handle cmdline arg
source $HOME/skps/newlook -r

#xdotool key X
#sleep .25
xdotool key super+e
xdotool key super+space
xdotool key super+h
xdotool key X
xdotool key super+w
sleep .25
#sleep .5
for i in {1..35}; do
  xdotool key super+plus
done
for i in {1..10}; do
  xdotool key alt+z
done
