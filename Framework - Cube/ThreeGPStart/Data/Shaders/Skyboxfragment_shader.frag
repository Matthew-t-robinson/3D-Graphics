#version 330

//uniform sampler2D sampler_tex;

uniform samplerCube skybox;

in vec3 texCoords;

out vec4 fragment_colour;

void main(void)
{
	//this sets the fragment colour of the skybox to be the cubemap
	fragment_colour = texture(skybox,texCoords);
}