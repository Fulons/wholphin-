#version 330

in layout(location = 1) vec3 position;
in layout(location = 3) vec2 inUV;
uniform mat4 m, v, p;

out vec2 UV;

void main(){
    gl_Position = p * v * m * vec4(position, 1.0f);
	gl_Position.z = 0.0f;
	UV = inUV;
}