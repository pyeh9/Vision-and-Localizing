ECE3090-Assignment-4-Vision-Localizing
======================================
Author: Peihsun (Ben) Yeh

Given images of a robot on a plane, identify its location and orientation.

Summary 
=======
This problem describes the task of determining where a robot is on a plane, as well as its orientation, from an image. 

The images are titled "TestImage#.pgm"

The outputs are (X,Y) locations of the robot in the image, with (0,0) in the top left of the image, and orientation is relative to 0 degrees when the robot is facing east. 

A more detailed problem description can be found in Project4.pdf and Project4Slides.pdf

Running
=======
Make sure the following are in the same directory:
- Project4.cpp
- image.cpp
- image.h
- Robot.cpp
- Robot.h
- input.dat
- makefile
As well as the following image files:
- TestImage0.pgm
- TestImage1.pgm
- TestImage2.pgm
- TestImage3.pgm
- TestImage4.pgm
Also make sure "input.dat" includes each of these files on its own line, finished to "ENDOFINPUT"

To compile, in terminal, type "make". To run, type "./Project4". When it is done, the output will be sent to "output.dat".

Possible future changes
=======================
- Include implementation for multiple item localizing. 
- Faster method than scanlines. 