#version 330

uniform sampler2D sampler_tex;

uniform vec4 diffuse_colour;

in vec2 texCoords;
in vec3 normal;
in vec3 FragPos;
in vec3 camera;

out vec4 fragment_colour;

void main(void)
{
	//calculates directional lighting, ambient lighting, point light, specular light and diffuse
	vec3 colour = vec3(1.0f,1.0f,1.0f);
	float specularStrength = 0.05;
	vec3 norm = normalize(normal);
	vec3 PointlightPos = vec3(1000.f, 500.f, 500.f);
	vec3 PointlightDir = normalize(PointlightPos - FragPos);
	vec3 lightDirection = vec3(0.45, 0.45, 1);
	vec3 viewDir = normalize(camera - FragPos);

	vec3 PointlightReflectDirection = reflect(-PointlightDir, norm);
	vec3 DirectionalLightReflectDirection = reflect(-lightDirection, norm);
	float specularPoint = pow(max(dot(viewDir, PointlightReflectDirection),0.0),32);
	float specularDirectional = pow(max(dot(viewDir, DirectionalLightReflectDirection),0.0),32);
	vec3 pointSpec = specularStrength*specularPoint*colour;
	vec3 dirSpec = specularStrength*specularDirectional*colour;

	float Directionaldiff = max(dot(norm, lightDirection), 0.0);
	float diff = max(dot(norm, lightDirection), 0.0);
	
	vec3 DirectionalDiffuse = Directionaldiff * colour;
	vec3 ambient = 0.5 * colour;
	vec3 diffuse = diff * colour;
	vec3 tex_colour = (texture(sampler_tex, texCoords).rgb) * (ambient + diffuse  + DirectionalDiffuse+dirSpec+pointSpec);
	fragment_colour = vec4(tex_colour,1.0);
}