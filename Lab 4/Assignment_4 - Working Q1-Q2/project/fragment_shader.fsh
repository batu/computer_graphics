#version 330 core

in vec4 color;
out vec4 fColor;

in vec4 position;

vec3 center;
vec3 newPosition;

vec4 computeColor();

vec3 normal;

void main()
{
	fColor = computeColor();
}



vec4 computeColor(){

	newPosition[0] = position[0];
	newPosition[1] = position[1];
	newPosition[2] = position[2];

	center = vec3(0,0,0);
	normal = (newPosition - center);

	vec4 lightIntensity = vec4(0.8,0.8,0.8,1.0);
	vec4 ambientIntensity = vec4(0.3,0.3,0.3,1.0);
	vec3 eyePosition = vec3(0.0, 0.0, -100.0);
	vec3 lightPosition = vec3(-1,0,0); 
	float shininess = 64;

	vec3 dirToLight = normalize(lightPosition);
	vec3 dirToEye = normalize(eyePosition);
	vec3 halfvector = normalize(dirToEye+dirToLight);
	float cosSigma = dot(halfvector, normal);
	float cosTheta = clamp(dot(normal, dirToLight),0,1);

	float sigma = acos(cosSigma);
	
	return color*(lightIntensity*cosTheta + ambientIntensity) + lightIntensity*pow(cosSigma,shininess);
	//return color*(lightIntensity*cosTheta + ambientIntensity) + lightIntensity*exp(-pow(sigma*shininess,2));
}