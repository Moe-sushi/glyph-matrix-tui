# POC stage only
# Warning:
- Some code are written by ChatGPT
- This way is not a general way for reading sensor data on every devices, you may need to adapt it for your device
- Use at your own risk

# Steps to reproduce:
- I used Samsung Galaxy watch 7, ONEUI 8.0
- I used a fork of termux-monet [(achyuki/termux-monet)](https://github.com/achyuki/termux-monet)
- Remember to download armeabi instead of aarch64 version
- Grant full permissions to termux-monet app
- Use `adb shell appops set com.termux MANGE_EXTERNAL_STORAGE allow` to allow termux-monet to read external storage, or it will crash on launch
- Enable built-in termux-api
- Disable monet background
- Change termux font, I used MiSans
- Open drawer, if you cannot, edit termux keys and add a `DRAWER` key
- Long press `keyboard` to disable it
- To do these steps, I used scrcpy

# How to run:
```
# level
termux-sensor -c;clear;termux-sensor -d 100 -s "Your sensor name"|stdbuf -o0 jq -r '."Your sensor name".values[0:4] | @sh '|./gmt level
# compass
termux-sensor -c;clear;termux-sensor -d 100 -s "Your sensor name"|stdbuf -o0 jq -r '."Your sensor name".values[0:4] | @sh '|./gmt compass
```
```
The sensor name can be found by running `termux-sensor -l`.
For Samsung Galaxy Watch 7, the sensor name is `"GeoMag Rotation Vector Sensor"`, and for xiaomi 15, it's `"rotation vector  Non-wakeup"`.