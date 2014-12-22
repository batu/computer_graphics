#version 330 core

in vec4 s_vPosition;
in vec2 s_vTexcoord;
out vec2 s_fTexcoord;
void main () {
	s_fTexcoord = s_vTexcoord;  
	gl_Position = s_vPosition;
}
