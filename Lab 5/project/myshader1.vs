#version 330 core



in vec4 s_vPosition;
in vec2 s_vTexcoord;
in vec3 vnewEyePosition;

out vec2 s_fTexcoord;
//need to send out the vertex position for fragment shader to do lighting
out vec3 s_fPosition;
out vec3 fnewEyePosition;

uniform mat4 Mcam;
uniform mat4 Mproj;

void main () {
	s_fTexcoord = s_vTexcoord;  
//	s_fPosition = s_vPosition.xyz;//send the vertex position
	gl_Position = Mproj*Mcam*s_vPosition;
	fnewEyePosition = vnewEyePosition;
}
