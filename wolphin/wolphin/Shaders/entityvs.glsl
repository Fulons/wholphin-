#version 330

in layout(location = 1) vec3 position;
in layout(location = 3) vec2 inUV;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec2 UV;

void main(){
    gl_Position = p * v * m * vec4(position, 1.0f);
	UV = inUV;
}