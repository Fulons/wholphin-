#version 330

layout(location = 1) in vec2 position;
layout(location = 2) in vec3 incolor;

out vec3 color;
void main(){
    gl_Position = vec4(position, 0.0f, 1.0f);
	color = incolor;
}