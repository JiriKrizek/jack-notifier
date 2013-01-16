#!/bin/bash
#
#echo "Setting volume to $1"
amixer set 'Master',0 "$1"% "$1"% >/dev/null 2>&1
