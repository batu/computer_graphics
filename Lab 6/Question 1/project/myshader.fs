#version 330 core

uniform float shaderSharpness;

in vec2 s_fTexcoord;
in vec3 s_fPosition;
in vec3 s_fNormal;

out vec4 fColor;

vec3 position;
vec3 normal;
vec4 color;


uniform sampler2D sampler;

vec4 computeColor();

void main () {
	position = s_fPosition;
	normal = s_fNormal * ;
	color = texture(sampler, s_fTexcoord);
	fColor = computeColor();
}

vec4 computeColor(){

	vec3 axis = vec3(0,0,1); 
	float angle = 3.14159/60;
	vec3 lightPosition = vec3(0, 0,-3); 
	vec3 eyePosition = vec3(0.0, 0.0, -100.0);

	
	vec3 dirToLight = normalize(lightPosition - position);
	vec3 dirToEye = normalize(eyePosition - position);
	vec3 halfvector = normalize(dirToEye + dirToLight);
	float cosSigma = dot(halfvector, normal);
	float cosTheta = clamp(dot(normal, dirToLight),0,1);

	
	float x = dot(-dirToLight, axis);
	float y = cos(angle);
	float ambient = 0.2;
	float intensity = ambient + (x>y ? 1: exp(shaderSharpness*(x-y)) );

	return intensity*color*1;
}