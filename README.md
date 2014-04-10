# 2k48  
Terminal version of 2048 game - http://git.io/2048

You can play online: **` telnet 2048.dynu.com `**

 - **author:** Mike 'sc0ty' Szymaniak
 - **email:** sc0typl[at]gmail.com
 - **webpage:** http://sc0ty.pl

## HOW TO PLAY

Use your arrow keys to move the tiles. When two tiles with the same number touch, they merge into one.

## CONTROLS

 - move: **arrows** or **W/S/A/D**
 - quit: **Q**
 - change rendering mode: **R**  
**R** will force your screen to be cleared and repainted every frame.  
It is useful if your terminal renders animation incorrectly.

## COMPILATION
- At this moment, only Linux OS is supported, sorry.
- You need *libncurses* library. You can get it (Ubuntu/Debian) by ` apt-get install libncurses5 libncurses5-dev `.
- To build it, run ` make `.
- To build debug version, run ` make DEBUG=1 `. It will be compiled with debug symbols and there is also testing cheat key **T**.
- To run by as telnet/ssh/whatever server, run `make DEAMON=1` and configure it to be run as login program for telnet service. E.g. if you are using (x)inetd, add to your /etc/inetd.conf:  
` telnet tcpnowait telnetd /usr/sbin/tcpd /usr/sbin/in.telnetd -h -L /path/to/2k48 `

## LICENSE
GNU GENERAL PUBLIC LICENSE Version 2, June 1991  
For more information see LICENSE file.

