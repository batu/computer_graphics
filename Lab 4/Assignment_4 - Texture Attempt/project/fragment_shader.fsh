#version 330 core

in vec2 s_fTexcoord;
out vec4 fColor;


uniform sampler2D sampler;

void main()
{
	fColor = texture(sampler, vec2(0.5,0.5) );
}