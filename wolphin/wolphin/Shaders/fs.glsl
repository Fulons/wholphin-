#version 330

in vec3 color;
in vec2 UV;

uniform sampler2D tex;

out vec4 outputColor;

void main(){
   //outputColor = vec4(color, 1.0f);
   outputColor = texture(tex, UV);
}