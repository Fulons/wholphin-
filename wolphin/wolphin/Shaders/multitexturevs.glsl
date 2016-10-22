#version 330

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

in layout(location = 1) vec3 position;
in layout(location = 2) vec3 incolor;
in layout(location = 3) vec2 inUV1;
in layout(location = 4) vec2 inUV2;

out vec3 color;
out vec2 UV1;
out vec2 UV2;

void main(){
    gl_Position = p * v * m * vec4(position, 1.0f);
	color = incolor;
	UV1 = inUV1;
	UV2 = inUV2;
}