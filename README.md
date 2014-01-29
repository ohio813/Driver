Driver
======

A Mandatory Access Control(MAC) Security System on Windows.

WARNING
-----------

Since it is a Kernal-Mode Driver & not finished, it may break your NT Kernal.

If you do not want to make your NT Kernal BROKEN, DO NOT clone & build it or even load the driver into your kernal & start the controller.

If you are sure of your knowledge of NT Kernal, SSDT & other things, GO AHEAD.

Build
------------

Use Windows Driver Kits(WDK) to compile & build the files in the root folder.
Driver Development Kits(DDK) are not suggested since they are deprecated, and i have not tested.

Just start the WDK environment & use the following simple command to build.

	build
	
The [makefile](https://github.com/gyf1214/Driver/blob/master/makefile) & [sources](https://github.com/gyf1214/Driver/blob/master/sources) have been configured,
you don't need to worry.

Then build the controller if you want. G++ Compiler is required, other C++ Compiler is not suggested since i don't know what error may occur.

Get into the [/controller](https://github.com/gyf1214/Driver/tree/master/controller) & use GNU Make to build automatically:

	make
	
If you do not have the GNU Make use the following commands:

	if NOT EXIST bin mkdir bin
	g++ -o bin/controller.exe main.cpp
	
The engine has not finished yet, but you can also make it using GNU make.

Execution
-------------

Well, if you want, these steps may REALLY BREAK YOUR NT KERNAL.

Register .sys file you just compiled into a driver service. Then start ithe service to load the driver into your kernal.

Then run the controller.exe you just compiled.

To stop the controller type 'q' & ENTER in the console.

Problem
------------

Please send an [issue](https://github.com/gyf1214/Driver/issues)

Contribution
-------------

Please send a [PR](https://github.com/gyf1214/Driver/pulls)

Author
-------------

[gyf1214](https://github.com/gyf1214) & [Tony Yang](https://github.com/tonyyanga)
