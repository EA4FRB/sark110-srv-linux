# sark110-srv-linux
     Linux Server for connecting to the sark-110 via a network
     =========================================================
About
======
The purpose of this project is connecting the SARK-110 antenna analyzer to a Raspberry PI and access to it from another computer via the network interface; e.g. from SARK Plots running on a Windows computer.

It has been tested on a Raspberry PI running Raspbian, but it should work in other computers running distinct Linux distros.

Build Instructions
===================
Prerequisites:
--------------
You will need to install development packages for libudev and	libusb.
    sudo apt-get install libudev-dev libusb-1.0-0-dev

Build
------
make

Install
-------
Add sark110.rules file to /etc/udev/rules.d
    
