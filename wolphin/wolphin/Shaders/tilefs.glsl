#version 330

in vec3 color;
in vec2 UV;

uniform sampler2D tex;

out vec4 outputColor;

void main(){
   outputColor = texture(tex, UV);
}