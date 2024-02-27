Written by Alexander Danho

Compiling and running code on chipKIT Uno32 using MCB32 tools on WSL2,

Using a debian based system follow the instructions.

run the following instructions on a terminal,

echo "deb-src http://archive.ubuntu.com/ubuntu/ xenial main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list
sudo apt update
sudo apt-get install bzip2 make libftdi-dev

Then proceed to and download the linux .run file from here https://github.com/is1200-example-projects/mcb32tools/releases/

an easy way to do that is to 

wget https://github.com/is1200-example-projects/mcb32tools/releases/download/v2.2/mcb32tools-v2.2-x86_64-linux-gnu.run

then run sudo ./mcbtools-vX.X...-linux.gnu.run


Using the toolchain:

Enter the terminal and type,

. /opt/mcb32tools/environment

Connect the chipkit then,

In Windows Device Manager, open the 'Ports (COM & LTP)' section. Find your device and check its assigned COM port number (it will look like 'COM4').

Compile the the code with "make"

To install the code onto the device type "make install TTYDEV=/dev/ttyS_" where "_" is the COM number noted earlier.