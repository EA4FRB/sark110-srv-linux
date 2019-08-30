# sark110-srv-linux
Linux Server for connecting to the SARK-110 Antenna Analyzer series via a network
   
About
======
The purpose of this project is connecting the SARK-110 antenna analyzer to a Raspberry PI and access to it from another computer via the network interface; e.g. from SARK Plots running on a Windows computer.

It has been tested on a Raspberry PI running Raspbian, but it should work in other computers running distinct Linux distros.

Build Instructions
===================
Prerequisites:
--------------
You will need to install development packages for libudev and libusb.

    sudo apt-get install libudev-dev libusb-1.0-0-dev

Build
------
make

Run
----
First time, copy sark110.rules file to /etc/udev/rules.d
Otherwise, you will need to run as root.

./sark110-srv.a

