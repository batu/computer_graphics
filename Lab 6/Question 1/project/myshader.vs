#version 330 core

in vec4 s_vPosition;
in vec2 s_vTexcoord;
in float vertexSharpness;

out float fragmentSharpness;
out vec2 s_fTexcoord;
out vec3 s_fPosition;
out vec3 s_fNormal;

uniform mat4 Mmodel; 

void main () {
	
	s_fTexcoord = s_vTexcoord;  
	vec3 s_vNormal = vec3(0,0,-1);
	s_fNormal = (Mmodel * vec4(s_vNormal,1.0)).xyz;
	gl_Position = Mmodel * s_vPosition;
	s_fPosition = gl_Position.xyz;
}
