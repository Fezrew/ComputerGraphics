// a simple flat colour shader
#version 410

out vec4 FragColour;
uniform vec3 kd;

void main() 
{
	FragColour = vec4(1, 0, 1, 1);
}