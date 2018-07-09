all:
	g++ -o compiled/mandelbrot.exe main.cpp external/glad.c -std=c++17 -lglfw3dll -lopengl32 -Wall -O3