// This shader is a changed version of a shader
// from this source: https://github.com/Nuke928/mandelbrot-opengl/blob/master/shader.glsl
// Copyright (C) 1989, 1991 Free Software Foundation, Inc., <http://fsf.org/>
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
// Changes must be documented due to GNU GPL license, so:
// - output color variable name changed
// - opengl used version changed
// - all double typed variables changed on float
// - gen color function name changed

#version 330 core
 
out vec4 frag_color;

uniform float screen_ratio;
uniform float zoom;

uniform vec2 screen_size;
uniform vec2 center;

uniform int iterations;
 
vec4 gen_color(const float t);
 
void main()
{
    vec2 z, c;
    c.x = screen_ratio * (gl_FragCoord.x / screen_size.x - 0.5) * 2;
    c.y = (gl_FragCoord.y / screen_size.y - 0.5) * 2;

    c.x = c.x / zoom + center.x;
    c.y = c.y / zoom + center.y;

    int i;
    
    for (i = 0; i < iterations; ++i) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;

		if (x * x + y * y > 4.0) 
            break;

		z.x = x;
		z.y = y;
    }
 
    float t = float(i) / iterations;

    frag_color = gen_color(t);
}

vec4 gen_color(const float t)
{
    float r = 9.0 * (1.0 - t) * t * t * t,
          g = 15 * (1.0 - t) * (1.0 - t) * t * t,
          b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

    return vec4(r, g, b, 1);
}