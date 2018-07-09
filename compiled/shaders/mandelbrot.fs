#version 410
 
out vec4 colorOut;
uniform float screen_ratio;
uniform vec2 screen_size;
uniform vec2 center;
uniform float zoom;
uniform int itr;
 
vec4 map_to_color(float t) {
    float r = 9.0 * (1.0 - t) * t * t * t;
    float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;
 
    return vec4(r, g, b, 1.0);
}
 
void main()
{
    vec2 z, c;
    c.x = screen_ratio * (gl_FragCoord.x / screen_size.x - 0.5) * 2;
    c.y = (gl_FragCoord.y / screen_size.y - 0.5) * 2;
 
    c.x /= zoom;
    c.y /= zoom;
 
    c.x += center.x;
    c.y += center.y;
 
    int i;
    for(i = 0; i < itr; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;
 
		if((x * x + y * y) > 4.0) break;
		z.x = x;
		z.y = y;
    }
 
    float t = float(i) / float(itr);
 
    colorOut = map_to_color(t);
}