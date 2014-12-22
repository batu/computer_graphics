#version 330 core

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec3 vnewEyePosition;

out vec3 fNormal;
out vec4 fColor;
out vec3 fPosition;
out vec3 fnewEyePosition;

uniform mat4 Mcam;
uniform mat4 Mproj;

void main()
{
	gl_Position = Mproj*Mcam*vPosition;
	fPosition = vPosition.xyz;
	fNormal = vNormal;
	fColor = vColor;
	fnewEyePosition = vnewEyePosition;
}
