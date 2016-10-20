#version 330

in vec2 UV;

uniform sampler2D tex;

out vec4 outputColor;

void main(){
	vec4 color = texture(tex, UV);
	if(color.a == 0.0f) {
		discard;
	}
   outputColor = color;
}