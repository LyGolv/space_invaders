#!/bin/bash

pid=`ps aux | grep "play -q ../audios/overlord.mp3 repeat -$" | awk '{print $2}'`
kill -9 $pid
rm rep