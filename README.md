# Description

Desktop software for DI-Lambda (in QT).

DI-Lambda is an open hardware spetrophotometer ([see video](https://www.youtube.com/watch?v=tzXC3dagmgE)).

[See website](https://koi.science/)

# Installation instructions

For Windows: get the executable file [from here](https://koi.science/downloads/).

For Mac OS X: get the executable file [from here](https://koi.science/downloads/).

For Ubuntu systems, see the PPA:
https://launchpad.net/~koi-science/+archive/ubuntu/software

For Archlinux:

~~~bash
sudo pacman -Sy qt5-serialport
qmake
make
~~~

For any GNU/Linux, install the qt5-serialport dependency, then `qmake` and `make`.

