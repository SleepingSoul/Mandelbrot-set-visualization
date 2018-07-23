![alt text](https://raw.githubusercontent.com/SleepingSoul/Mandelbrot-set-visualization/master/preview.gif)

Hello!
--------------------------------------

In this project i show how to implement Mandelbrot set visualization.

How to use it:
  -  You can move a camera with W, A, S, D buttons.
  -  You can increase / decrease number of iterations with '-', '=' buttons.
  -  You can change a zoom with mouse wheel.

On the current day it has a few minuses:
- i use a float type in fragment shader, so the quality of picture is seriously restricted when it is rendered with big zoom.
It would be great to find some way to make glsl 4 work with glfw3 / glad.
- it gives user too restricted number of options. Should provide some customizations (for example, color).

Important!
--------
If you want to run it in your computer, you should install glwf3 (http://www.glfw.org/)
and glad (https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library, http://glad.dav1d.de/). I do not neither own nor provide
this headers.
glad.c header is generated automatically and i cannot guarantee it will work in your computer. I would recommend to replace it on new
file, generated in http://glad.dav1d.de/.
