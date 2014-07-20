ObjectLoader
============

ObjectLoader (obj-loader) is a simple program designed to render
object files (.obj) using OpenGL 3.2, mainly so that I could learn the
new OpenGL standard (it is very different from versions 1 and 2).  

To compile, simply type 'make'.  

usage:

    $ obj-loader <file.obj>  
    
If no file is given, the screen will just be black. You can actually
give it multiple .obj files, but I never implemented a way to move
them around which means they'll all overlap each other. Loading one
file was really all I wanted to do anyway....  

This program depends on the following:  
OpenGL 3.2+ (GL and GLEW)  
[glm](http://glm.g-truc.net/0.9.4/index.html) (OpenGL Math library)  
[glfw](http://www.glfw.org/) (similar to GLUT, but newer. Creates the
window and such)  

Controls:

* mouse to look around  
* wasd to move around  
* q and e to rotate  
* x and z to raise and lower  


Created by Sean Hickey (Wisellama), 2014  
Released to the Public Domain under the terms of the
[Unlicense](http://unlicense.org/)  

Note: I used the
[Generic Makefile](https://github.com/mbcrawfo/GenericMakefile) for
this project. It's a great way to quickly set up a C/C++ project
quickly and know that the Makefile is correct. I also barely touched
its real functionality. It's released under the MIT license which can
be found in the "other_licenses" directory.
