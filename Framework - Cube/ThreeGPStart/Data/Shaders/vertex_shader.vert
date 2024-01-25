#version 330

uniform mat4 combined_xform;
uniform mat4 model_xform;
uniform vec3 CameraPosition;

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec2 varying_texCoords;
layout (location=2) in vec3 varying_normals;

out vec2 texCoords;
out vec3 normal;
out vec3 FragPos;
out vec3 camera;

void main(void)
{	
	//this works out the fragment positions and the terrain positions as well as passing the normals,
	//camera position and the tex coords into the fragment shader
	texCoords = varying_texCoords;

	gl_Position = combined_xform * model_xform * vec4(vertex_position, 1.0);

	FragPos = vec3(model_xform * vec4(vertex_position,1.0));

	normal = varying_normals;
	camera = CameraPosition;
}