#version 330 core

in vec4 fColor;
in vec3 fNormal;
in vec3 fPosition;
in vec3 fnewEyePosition;

uniform vec3 camPos;


out vec4 outColor;


vec4 computeColor(in vec3 position, in vec3 normal, in vec4 color, vec3 eyePosition, vec3 lightPosition);

void main()
{
	vec3 eyePosition = camPos;
	vec3 lightPosition = vec3(0,10,0); 

	outColor = computeColor(fPosition, normalize(fNormal), fColor, eyePosition, lightPosition);
	//outColor = fColor;
}

vec4 computeColor(in vec3 position, in vec3 normal, in vec4 color, vec3 eyePosition, vec3 lightPosition){

	vec4 lightIntensity = vec4(0.8,0.8,0.8,1.0);
	vec4 ambientIntensity = vec4(0.2,0.2,0.2,1.0);
	float shininess = 64;

	vec3 dirToLight = normalize(lightPosition - position);
	vec3 dirToEye = normalize(eyePosition - position);
	vec3 halfvector = normalize(dirToEye+dirToLight);
	float cosSigma = dot(halfvector, normal);
	float cosTheta = clamp(dot(normal, dirToLight),0,1);

	
	
	return color*(lightIntensity*cosTheta + ambientIntensity) + lightIntensity*pow(cosSigma,shininess);

	//float sigma = acos(cosSigma);
	//return color*(lightIntensity*cosTheta + ambientIntensity) + lightIntensity*exp(-pow(sigma*shininess,2));
}
