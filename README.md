# Analyzer

A Lilu plugin to display data of the CPU and all GPU(s) of the current machine.
Please don't use this, this is just a fun pet project I had to get myself into Kext development. ~~This hasn't even been tested yet.~~ Tested, and is working on my machine, so far.

Uses kernel-space I/O Kit families to achieve this, as well as probing the GPU device(s) in order to obtain information.
