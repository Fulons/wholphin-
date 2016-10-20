#version 330

in vec3 color;
in vec2 UV;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

out vec4 outputColor;

void main(){
   outputColor = texture(tex, UV);
}