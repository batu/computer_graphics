#version 330 core

in vec4 vPosition;

in vec2 s_vTexcoord;
out vec2 s_fTexcoord;

uniform float rot_x1;
uniform float rot_x2;
uniform float rot_x3;

uniform float rot_y1;
uniform float rot_y2;
uniform float rot_y3;

uniform float rot_z1;
uniform float rot_z2;
uniform float rot_z3;

void main()
{

	s_fTexcoord = s_vTexcoord;
    gl_Position = vPosition;
	

	gl_Position[0] = (gl_Position[0] * rot_x1) + (gl_Position[1] * rot_x2) + (gl_Position[2] * rot_x3);
	gl_Position[1] = (gl_Position[0] * rot_y1) + (gl_Position[1] * rot_y2) + (gl_Position[2] * rot_y3);
	gl_Position[2] = (gl_Position[0] * rot_z1) + (gl_Position[1] * rot_z2) + (gl_Position[2] * rot_z3);

 
}