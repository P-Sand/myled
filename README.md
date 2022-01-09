# myled
This repository is a Linux device driver. Sends the Morse code corresponding to the entered characters.

# Description
By installing this module, the characters entered on the command prompt are read and sent as Morse code from the LEDs connected to GPIO24 and GPIO25 of Raspberry Pi3.

Explanation of Morse code by wikipedia : https://ja.wikipedia.org/wiki/%E3%83%A2%E3%83%BC%E3%83%AB%E3%82%B9%E7%AC%A6%E5%8F%B7#%E5%A4%96%E9%83%A8%E3%83%AA%E3%83%B3%E3%82%AF

# Demo
Check it out on You Tube : https://youtu.be/thIq2NcFO1Y

# Usage
## compile
```
$ make
```
Makefile is required in the same directory.

## introduce
```
$ sudo insmod myled.ko
```
## change permissions
```
$ sudo shmod 666 /dev/myled0
```

## transmission
```
$ echo a > /dev/myled0
```
Words consisting only of lowercase letters can be used.

## remove
```
$ sudo rmmod myled
```

# Requirement
- Ubuntu 18.04
- Raspberry Pi3

# License
Released under the GNU General Public License version 3.0.

© P-Sand 2022
All rights reserved
