#version 330

in layout(location = 1) vec2 position;
in layout(location = 2) vec3 incolor;
in layout(location = 3) vec2 inUV1;
in layout(location = 4) vec2 inUV2;
in layout(location = 4) vec2 inUV3;
uniform mat4 m, v, p;

out vec3 color;
out vec2 UV;

void main(){
    gl_Position = p * v * m * vec4(position, 0.0f, 1.0f);
	color = incolor;
	UV = inUV;
}