#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"


class Renderer
{
private:
	// Program object - to host shaders
	GLuint m_program{ 0 };
	GLuint m_programCube{ 0 };
	GLuint m_programJeep{ 0 };
	GLuint m_programSkybox{ 0 };

	GLuint SkyboxTexture;
	
	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };
	GLuint m_VAOJeep{ 0 };
	GLuint m_VAOCube{ 0 };
	GLuint m_VAOSkybox{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };
	GLuint m_numElementsJeep{ 0 };
	GLuint m_numElementsCube{ 0 };

	bool m_wireframe{ false };

	bool CreateProgram();

	bool CreateCubeProgram();

	bool CreateJeepProgram();

	bool CreateSkyboxProgram();

	GLuint tex;
	GLuint jeepTex;
	GLuint cubeColour;
public:
	Renderer();
	~Renderer();

	// Draw GUI
	void DefineGUI();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

