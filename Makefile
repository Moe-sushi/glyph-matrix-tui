all :
	cc *.c -o gmt -lm
format :
	clang-format -i *.c include/*.h
test:all
	clear;termux-sensor -c;termux-sensor -d 100 -s "GeoMag Rotation Vector Sensor","LSM6DSO Gyroscope"|stdbuf -o0 jq -r '."GeoMag Rotation Vector Sensor".values[0:4],."LSM6DSO Gyroscope".values[0:3] | @sh '|./gmt
compass:all
	clear;termux-sensor -c;termux-sensor -d 100 -s "GeoMag Rotation Vector Sensor"|stdbuf -o0 jq -r '."GeoMag Rotation Vector Sensor".values[0:4] | @sh '|./gmt compass
level:all
	clear;termux-sensor -c;termux-sensor -d 100 -s "GeoMag Rotation Vector Sensor"|stdbuf -o0 jq -r '."GeoMag Rotation Vector Sensor".values[0:4] | @sh '|./gmt level