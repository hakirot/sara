#!/usr/bin/env bash
set -eou pipefail

if [ "$#" -ne 1 ]; then
    echo "Usage: xdo.sh <option>"
    exit 1
fi

if [ "$1" -eq "1" ]; then
  xdotool key alt+backslash
  xdotool key alt+backslash
  xdotool key control+j
  xdotool key E
  xdotool key Q
  sleep 0.5
  xdotool key v
  sleep 0.25
  xdotool key f
fi

if [ "$1" -eq "2" ]; then
  xdotool key control+j
  xdotool key exclam
  xdotool key alt+backslash
  xdotool key alt+backslash
  xdotool key control+j
  xdotool key E
  xdotool key Q
  sleep 0.5
  xdotool key v
  sleep 0.25
  xdotool key f
fi

if [ "$1" -eq "3" ]; then
  xdotool key m
  xdotool key KP_Enter
  xdotool key alt+minus
  xdotool key alt+minus
  xdotool key q
  xdotool key m
  xdotool key KP_Enter
  xdotool key control+j
  xdotool key E
  xdotool key q
  xdotool key m
  xdotool key KP_Enter
  xdotool key alt+k
  xdotool key q
  xdotool key m
  xdotool key KP_Enter
  xdotool key alt+backslash
  xdotool key alt+backslash
  xdotool key q
  xdotool key m
  xdotool key KP_Enter
  xdotool key control+j
  xdotool key E
  nudges=14
  xdotool key control+j
  while [ $nudges -gt 0 ]; do
    xdotool key alt+Right
    nudges=$((nudges - 1))
  done
  xdotool key alt+h
  xdotool key alt+h
  nudges=14
  xdotool key control+j
  while [ $nudges -gt 0 ]; do
    xdotool key alt+Left
    nudges=$((nudges - 1))
  done
  xdotool key alt+k
  nudges=3
  xdotool key control+j
  while [ $nudges -gt 0 ]; do
    xdotool key alt+Up
    nudges=$((nudges - 1))
  done
  xdotool key control+j
  xdotool key control+Up
  xdotool key alt+k
  nudges=3
  xdotool key control+j
  while [ $nudges -gt 0 ]; do
    xdotool key alt+Down
    nudges=$((nudges - 1))
  done
  xdotool key control+j
  xdotool key control+Down
  xdotool key alt+k
  xdotool key alt+l
  xdotool key v
  sleep 1
  xdotool key f
  xdotool key alt+backslash
  xdotool key alt+h
fi

if [ "$1" -eq "4" ]; then

fi
