#version 330

in vec3 color;
in vec2 UV1;
in vec2 UV2;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D textureMap;

out vec4 outputColor;

void main(){
	float mixerValue = texture(textureMap, UV2).r * 3.0f;

	vec4 tex1Color = texture(tex1, UV1) * (1 - abs(clamp(mixerValue - 0, -1, 1)));
	vec4 tex2Color = texture(tex2, UV1) * (1 - abs(clamp(mixerValue - 1, -1, 1)));
	vec4 tex3Color = texture(tex3, UV1) * (1 - abs(clamp(mixerValue - 2, -1, 1)));
	vec4 tex4Color = texture(tex4, UV1) * (1 - abs(clamp(mixerValue - 3, -1, 1)));

	outputColor = tex1Color + tex2Color + tex3Color + tex4Color;
}