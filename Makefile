all:
	g++ -o compiled/mandelbrot.exe main.cpp glad.c -std=c++17 -lglfw3dll -lopengl32 -Wall -O3