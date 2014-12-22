#version 330 core

in vec2 s_fTexcoord;

out vec4 fColor;

uniform sampler2D diffuseMap;

void main () {

	vec3 eyePosition = vec3(0,0,0);

   fColor = texture(diffuseMap, s_fTexcoord);

}
