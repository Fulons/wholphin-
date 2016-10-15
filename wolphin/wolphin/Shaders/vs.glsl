#version 330

in layout(location = 1) vec2 position;
in layout(location = 2) vec3 incolor;
uniform mat4 m, v, p;

out vec3 color;
void main(){
    gl_Position = p * v * m * vec4(position, 0.0f, 1.0f);
	color = incolor;
}