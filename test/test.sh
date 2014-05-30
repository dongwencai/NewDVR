#!/bin/bash

i=1
for ((i=1;i<255;++i))
do
    temp=192.168.100."$i"
    if 
    ping -c 1 -w 1 $temp>/dev/null
    then
        echo $temp
    fi
    
done
