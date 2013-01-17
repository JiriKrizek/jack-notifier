#!/bin/bash
#
#echo "Setting volume to $1"
amixer set 'Master',0 "$1"% "$1"% >/dev/null 2>&1
NOTIFY_UID=1000
DISPLAY=:0 sudo -u \#$NOTIFY_UID notify-send --hint=int:transient:1 --expire-time=1 "Sound set to $1%" --urgency=low
