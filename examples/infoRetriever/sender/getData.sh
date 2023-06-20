#!/usr/bin/bash
while true
  do
  cpu=$(echo "$(grep 'cpu ' /proc/stat | awk '{cpu_usage=($2+$4)*100/($2+$4+$5)} END {printf "%0.2f%", cpu_usage}')")
  gpu=$(sudo lshw -C display -short | awk 'FNR==3 {print $4}')
  memory=$(free -h --si | awk '/^Mem:/ {print $3}')
  storage=$(df -h /dev/mapper/luks_root | awk 'FNR==2 {print $4}')

  ./sendArduino.sh ","$cpu $gpu $memory $storage
  sleep 1s
done
