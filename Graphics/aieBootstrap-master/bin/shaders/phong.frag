// classic Phong fragment shader
#version 410

in vec3 vNormal;
uniform vec3 AmbientColour; // ambient light colour
uniform vec3 LightColour; // light colour
uniform vec3 LightDirection;
out vec4 FragColour;

void main() 
{
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	
	// calculate lambert term (negate light direction)
	float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );
	
	// calculate diffuse
	vec3 diffuse = LightColour * lambertTerm;

	// output final colour
	FragColour = vec4( diffuse, 1 );
	
}