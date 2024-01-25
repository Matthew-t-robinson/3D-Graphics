#include "Renderer.h"
#include "Camera.h"
#include "ImageLoader.h"

Renderer::Renderer() 
{

}

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	// TODO: clean up any memory used including OpenGL objects via glDelete* calls
	glDeleteProgram(m_program);
	glDeleteBuffers(1, &m_VAO);
}

// Use IMGUI for a simple on screen GUI
void Renderer::DefineGUI()
{
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("3GP");						// Create a window called "3GP" and append into it.

	ImGui::Text("Visibility.");					// Display some text (you can use a format strings too)	

	ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
	ImGui::End();
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return false;

	return true;
}

bool Renderer::CreateCubeProgram()
{
	// Create a new program (returns a unqiue id)
	m_programCube = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/Cubevertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/Cubefragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_programCube, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_programCube, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_programCube))
		return false;

	return true;
}

bool Renderer::CreateJeepProgram()
{

	// Create a new program (returns a unqiue id)
	m_programJeep = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/Jeepvertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/Jeepfragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_programJeep, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_programJeep, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_programJeep))
		return false;

	return true;
}

bool Renderer::CreateSkyboxProgram()
{

	// Create a new program (returns a unqiue id)
	m_programSkybox = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/Skyboxvertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/Skyboxfragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_programSkybox, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_programSkybox, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_programSkybox))
		return false;

	return true;
}

//noise Calculations
float Noise(int x, int y)
{
	int n = x + y * 57;
	n = (n >> 33) ^ n;
	int nn = (n * (n * n * 60493 + 1990303) + 1376312589) & 0x7fffffff;
	return 1.0f - ((float)nn / 1073741824.0f);
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	
	if (!CreateJeepProgram())
		return false;
	if (!CreateCubeProgram())
		return false;
	if (!CreateProgram())
		return false;
	if (!CreateSkyboxProgram())
		return false;
	Helpers::ModelLoader loader;

	if (!loader.LoadFromFile("Data\\Models\\Jeep\\jeep.obj"))
		return false;
	//loads and binds the jeep texture
	Helpers::ImageLoader JeepTexLoader;
	if (JeepTexLoader.Load("Data\\Models\\Jeep\\jeep_rood.jpg"))
	{
		glGenTextures(1, &jeepTex);
		glBindTexture(GL_TEXTURE_2D, jeepTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, JeepTexLoader.Width(), JeepTexLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, JeepTexLoader.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		MessageBox(NULL, L"Jeep texture not loading", L"Error", MB_OK | MB_ICONEXCLAMATION);

		return false;
	}
	
	//loads the jeep model
	for (const Helpers::Mesh& mesh : loader.GetMeshVector())
	{
		//calculates jeep normals
		std::vector<glm::vec3> normalsJeep;
		for (size_t i = 0; i < mesh.vertices.size(); i++)
		{
			normalsJeep.push_back(glm::vec3(0, 0, 0));
		}
		for (size_t i = 0; i < mesh.elements.size(); i += 3)
		{
			glm::vec3 p1, p2, p3;
			p1 = mesh.vertices[mesh.elements[i]];
			p2 = mesh.vertices[mesh.elements[i + 1]];
			p3 = mesh.vertices[mesh.elements[i + 2]];
			glm::vec3 edge1 = p2 - p1;
			glm::vec3 edge2 = p3 - p1;
			glm::vec3 TriNorm = glm::normalize(glm::cross(edge1, edge2));

			normalsJeep[mesh.elements[i]] += TriNorm;
			normalsJeep[mesh.elements[i + 1]] += TriNorm;
			normalsJeep[mesh.elements[i + 2]] += TriNorm;
		}
		for (size_t i = 0; i < normalsJeep.size(); i++)
		{
			glm::normalize(normalsJeep[i]);
		}

		
		//creates buffers for the positions, texture positions, the normals and all the elements
		GLuint JeepPositionsVBO;
		glGenBuffers(1, &JeepPositionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, JeepPositionsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint JeepElementEBO;
		glGenBuffers(1, &JeepElementEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, JeepElementEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.elements.size(), mesh.elements.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		GLuint JeepCoordsVBO;
		glGenBuffers(1, &JeepCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, JeepCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint JeepnormalsVBO;
		glGenBuffers(1, &JeepnormalsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, JeepnormalsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normalsJeep.size(), normalsJeep.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_numElementsJeep = mesh.elements.size();

		//wraps the buffers

		glGenVertexArrays(1, &m_VAOJeep);

		glBindVertexArray(m_VAOJeep);

		glBindBuffer(GL_ARRAY_BUFFER, JeepPositionsVBO);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, JeepElementEBO);

		glBindVertexArray(0);

		glBindVertexArray(m_VAOJeep);

		glBindBuffer(GL_ARRAY_BUFFER, JeepCoordsVBO);

		glEnableVertexAttribArray(1);

		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindVertexArray(0);

		glBindVertexArray(m_VAOJeep);

		glBindBuffer(GL_ARRAY_BUFFER, JeepnormalsVBO);

		glEnableVertexAttribArray(2);

		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindVertexArray(0);
	}
	
	/*
		TODO 1: The first step is to create the vertices, colours and indices
		You could use std::vectors to hold these

		The cube is 20 by 20 centred on 0,0,0
		so the minimum values for x, y, z are -10,-10,-10
		and the maximum values are 10,10,10
	*/

	//terrain
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvCoords;
	std::vector<GLuint> elements;
	std::vector<glm::vec3> normals;
	float numCellX = 255;
	float numCellZ = 255;
	int numVertsX = numCellX + 1;
	int numVertsZ = numCellZ + 1;
	
	size_t index{ 0 };
	float NoiseVal;
	

	bool toggleForDiamondPattern = true;
	bool toggleForNoise = true;
	bool MoreNoise = false;
	//loads and applies the heightmap
	Helpers::ImageLoader HeightMap;
	if (HeightMap.Load("Data\\Heightmaps\\curvy.gif"))
	{
		float VertXToHM = HeightMap.Width() / numVertsX;
		float VertZToHM = HeightMap.Height() / numVertsZ;
		BYTE* data = HeightMap.GetData();
		for (int i = 0; i < numVertsX; i++)
		{

			for (int j = 0; j < numVertsZ; j++)
			{
				float HeightMapX = VertXToHM * i;
				float HeightMapZ = VertZToHM * j;
				size_t Offset = ((size_t)HeightMapX + (size_t)HeightMapZ *(HeightMap.Width())) * 4;
				BYTE Height = data[Offset];
				vertices.push_back(glm::vec3(i * 8, Height , j * 8));
				uvCoords.push_back(glm::vec2(j / (float)numCellX, i / (float)numCellZ));
				normals.push_back(glm::vec3(0, 1, 0));
			}
		}
	}
	else
	{
		for (int i = 0; i < numVertsZ; i++)
		{
			for (int j = 0; j < numVertsX; j++)
			{
				vertices.push_back(glm::vec3(i * 8, 0, j * 8));
				uvCoords.push_back(glm::vec2(j / (float)numCellX, i / (float)numCellZ));
			}
		}
	}
	//sets the terrain grid to be in a diamond pattern
	for ( int cellZ = 0; cellZ < numCellZ; cellZ++)
	{
		for  (int cellX = 0; cellX < numCellX; cellX++)
		{
			int startVertsIndex = (cellZ * numVertsX) + cellX;
			if (toggleForDiamondPattern)
			{
				elements.push_back(startVertsIndex);
				elements.push_back(startVertsIndex + 1);
				elements.push_back(startVertsIndex + numVertsX);

				elements.push_back(startVertsIndex + 1);
				elements.push_back(startVertsIndex + numVertsX + 1);
				elements.push_back(startVertsIndex + numVertsX);
			}
			else
			{
				elements.push_back(startVertsIndex + 1);
				elements.push_back(startVertsIndex + numVertsX + 1);
				elements.push_back(startVertsIndex);
				
				
				elements.push_back(startVertsIndex);
				elements.push_back(startVertsIndex + numVertsX + 1);
				elements.push_back(startVertsIndex + numVertsX);
			}
			toggleForDiamondPattern = !toggleForDiamondPattern;
		}
		toggleForDiamondPattern = !toggleForDiamondPattern;

	}
	//adds noise to the terrain
	if (toggleForNoise)
	{
		for (int i = 0; i < numVertsX; i++)
		{
			for (int j = 0; j < numVertsZ; j++)
			{
				NoiseVal = Noise(i, j);
				NoiseVal = NoiseVal + 1.25 / 2;
				glm::vec3 NoiseVec = vertices[index];

				if (!MoreNoise)
				{
					NoiseVal = NoiseVal * 5;
				}
				
				NoiseVec.y = NoiseVec.y + NoiseVal;
				vertices[index] = NoiseVec;
				index++;
			}
		}
	}

	
	
	
	m_numElements = elements.size();
	//loads and binds the terrain texture
	Helpers::ImageLoader TerrainTexture;
	if (TerrainTexture.Load("Data\\textures\\dirt_earth-n-moss_df_.dds"))
	{
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TerrainTexture.Width(), TerrainTexture.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, TerrainTexture.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	//calculates normals for the terrain
	for (size_t i = 0; i < vertices.size(); i++)
	{
		normals.push_back(glm::vec3(0, 0, 0));
	}
	for (size_t i = 0; i < elements.size(); i += 3)
	{
		glm::vec3 p1, p2, p3;
		p1 = vertices[elements[i]];
		p2 = vertices[elements[i + 1]];
		p3 = vertices[elements[i + 2]];
		glm::vec3 edge1 = p2 - p1;
		glm::vec3 edge2 = p3 - p1;
		glm::vec3 TriNorm = glm::normalize(glm::cross(edge1, edge2));

		normals[elements[i]] += TriNorm;
		normals[elements[i + 1]] += TriNorm;
		normals[elements[i + 2]] += TriNorm;
	}
	for (size_t i = 0; i < normals.size(); i++)
	{
		glm::normalize(normals[i]);
	}
	
	///*
	//	TODO 2: Next you need to create VBOs for the vertices and the colours
	//	You can look back to last week for examples
	//*/
	//creates buffers for the positions, texture positions, the normals and all the elements
	GLuint positionsVBO;
	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint texCoordsVBO;
	glGenBuffers(1, &texCoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvCoords.size(), uvCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint elementEBO;
	glGenBuffers(1, &elementEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	///*
	//	TODO 3: You also need to create an element buffer
	//	Store the number of elements in the member variable m_numElements
	//*/
	

	///*
	//	TODO 4: Finally create a VAO to wrap the buffers. You need to specify the streams for the positions
	//	(attribute 0) and colours (attribute 1). You also need to bind the element buffer.
	//	Use the member variable m_VAO
	//*/
	//wraps the buffers
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindVertexArray(0);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindVertexArray(0);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);

	glEnableVertexAttribArray(2);

	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementEBO);
	glBindVertexArray(0);

	///*
	//	TODO 5: Run it and see if you can see the cube.
	//	You should not have to edit the render function or shaders to see the cube.
	//*/
	//sets the cube vertices positions and colours
	glm::vec3 frontCubeCorners[4] =
	{
		{-10,-10,10},//0
		{10, -10, 10},//1
		{-10, 10 , 10},//2
		{10, 10, 10}//3
	};
	glm::vec3 rightCubeCorners[4] =
	{
		{10, -10, 10},//1
		{10, 10, 10},//3
		{10, -10, -10},//5
		{10, 10, -10}//7
	};
	glm::vec3 backCubeCorners[4] =
	{
		{-10, -10, -10},//4
		{10, -10, -10},//5
		{-10 ,10 ,-10},//6
		{10, 10, -10}//7
	};
	glm::vec3 leftCubeCorners[4] =
	{
		{-10,-10,10},//0
		{-10, 10 , 10},//2
		{-10, -10, -10},//4
		{-10 ,10 ,-10}//6
	};
	glm::vec3 topCubeCorners[4] =
	{
		{-10, 10 , 10},//2
		{10, 10, 10},//3
		{-10 ,10 ,-10},//6
		{10, 10, -10}//7
	};
	glm::vec3 bottomCubeCorners[4] =
	{
		{-10,-10,10},//0
		{10, -10, 10},//1
		{-10, -10, -10},//4
		{10, -10, -10}//5
	};
	glm::vec3 colours[6] =
	{
		{1, 0 , 0},//red
		{0, 0, 1},//blue
		{0,1,0},//green
		{1,0.5,0},//orange
		{1,1,0},//yellow
		{1,1,1}//white
	};
	std::vector<glm::vec3> cubeColours;
	std::vector<glm::vec3> cubeVerts;
	cubeVerts.push_back(frontCubeCorners[0]);//0
	cubeVerts.push_back(frontCubeCorners[1]);//1
	cubeVerts.push_back(frontCubeCorners[2]);//2
	cubeVerts.push_back(frontCubeCorners[3]);//3
	cubeColours.push_back(colours[0]);
	cubeColours.push_back(colours[0]);
	cubeColours.push_back(colours[0]);
	cubeColours.push_back(colours[0]);
	cubeVerts.push_back(rightCubeCorners[0]);//1
	cubeVerts.push_back(rightCubeCorners[1]);//3
	cubeVerts.push_back(rightCubeCorners[2]);//5
	cubeVerts.push_back(rightCubeCorners[3]);//7
	cubeColours.push_back(colours[1]);
	cubeColours.push_back(colours[1]);
	cubeColours.push_back(colours[1]);
	cubeColours.push_back(colours[1]);
	cubeVerts.push_back(backCubeCorners[0]);//4
	cubeVerts.push_back(backCubeCorners[1]);//5
	cubeVerts.push_back(backCubeCorners[2]);//6
	cubeVerts.push_back(backCubeCorners[3]);//7
	cubeColours.push_back(colours[2]);
	cubeColours.push_back(colours[2]);
	cubeColours.push_back(colours[2]);
	cubeColours.push_back(colours[2]);
	cubeVerts.push_back(leftCubeCorners[0]);//0
	cubeVerts.push_back(leftCubeCorners[1]);//2
	cubeVerts.push_back(leftCubeCorners[2]);//4
	cubeVerts.push_back(leftCubeCorners[3]);//6
	cubeColours.push_back(colours[3]);
	cubeColours.push_back(colours[3]);
	cubeColours.push_back(colours[3]);
	cubeColours.push_back(colours[3]);
	cubeVerts.push_back(topCubeCorners[0]);//2
	cubeVerts.push_back(topCubeCorners[1]);//3
	cubeVerts.push_back(topCubeCorners[2]);//6
	cubeVerts.push_back(topCubeCorners[3]);//7
	cubeColours.push_back(colours[4]);
	cubeColours.push_back(colours[4]);
	cubeColours.push_back(colours[4]);
	cubeColours.push_back(colours[4]);
	cubeVerts.push_back(bottomCubeCorners[0]);//0
	cubeVerts.push_back(bottomCubeCorners[1]);//1
	cubeVerts.push_back(bottomCubeCorners[2]);//4
	cubeVerts.push_back(bottomCubeCorners[3]);//5
	cubeColours.push_back(colours[5]);
	cubeColours.push_back(colours[5]);
	cubeColours.push_back(colours[5]);
	cubeColours.push_back(colours[5]);


	//adds the cube vertices positions to the elements vector
	std::vector<GLuint> elementsCube;
	//front
	elementsCube.push_back(0);
	elementsCube.push_back(1);
	elementsCube.push_back(2);

	elementsCube.push_back(3);
	elementsCube.push_back(2);
	elementsCube.push_back(1);

	//right
	elementsCube.push_back(6);
	elementsCube.push_back(5);
	elementsCube.push_back(4);

	elementsCube.push_back(5);
	elementsCube.push_back(6);
	elementsCube.push_back(7);
	//back
	elementsCube.push_back(10);
	elementsCube.push_back(9);
	elementsCube.push_back(8);

	elementsCube.push_back(9);
	elementsCube.push_back(10);
	elementsCube.push_back(11);
	//left
	elementsCube.push_back(12);
	elementsCube.push_back(13);
	elementsCube.push_back(14);

	elementsCube.push_back(15);
	elementsCube.push_back(14);
	elementsCube.push_back(13);
	//top
	elementsCube.push_back(16);
	elementsCube.push_back(17);
	elementsCube.push_back(18);

	elementsCube.push_back(19);
	elementsCube.push_back(18);
	elementsCube.push_back(17);
	//bottom
	elementsCube.push_back(22);
	elementsCube.push_back(21);
	elementsCube.push_back(20);

	elementsCube.push_back(21);
	elementsCube.push_back(22);
	elementsCube.push_back(23);

	m_numElementsCube = elementsCube.size();

	//creates buffers for the positions, colours and all the elements

	GLuint positionsVBOCube;
	glGenBuffers(1, &positionsVBOCube);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBOCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* cubeVerts.size(), cubeVerts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint coloursVBOCube;
	glGenBuffers(1, &coloursVBOCube);
	glBindBuffer(GL_ARRAY_BUFFER, coloursVBOCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* cubeColours.size(), cubeColours.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint elementEBOCube;
	glGenBuffers(1, &elementEBOCube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementEBOCube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* elementsCube.size(), elementsCube.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//wraps the buffers

	glGenVertexArrays(1, &m_VAOCube);

	glBindVertexArray(m_VAOCube);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVBOCube);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindVertexArray(0);

	glBindVertexArray(m_VAOCube);

	glBindBuffer(GL_ARRAY_BUFFER, coloursVBOCube);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementEBOCube);
	glBindVertexArray(0);


	std::string back = "Data\\Models\\Sky\\Hills\\skybox_back.jpg";
	std::string front = "Data\\Models\\Sky\\Hills\\skybox_front.jpg";
	std::string left = "Data\\Models\\Sky\\Hills\\skybox_left.jpg";
	std::string right = "Data\\Models\\Sky\\Hills\\skybox_right.jpg";
	std::string top = "Data\\Models\\Sky\\Hills\\skybox_top.jpg";
	std::string bottom = "Data\\Models\\Sky\\Hills\\skybox_bottom.jpg";

	
	Helpers::ImageLoader Right;
	Helpers::ImageLoader Left;
	Helpers::ImageLoader Top;
	Helpers::ImageLoader Bottom;
	Helpers::ImageLoader Back;
	Helpers::ImageLoader Front;
	glGenTextures(1, &SkyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture);
	//loads and applies the textures to the correct cubemap positions
	if (Right.Load(right))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, Right.Width(), Right.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Right.GetData());
	}
	if (Left.Load(left))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, Left.Width(), Left.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Left.GetData());
	}
	if (Top.Load(top))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, Top.Width(), Top.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Top.GetData());
	}
	if (Bottom.Load(bottom))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, Bottom.Width(), Bottom.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Bottom.GetData());
	}
	if (Back.Load(back))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, Back.Width(), Back.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Back.GetData());
	}
	if (Front.Load(front))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, Front.Width(), Front.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Front.GetData());
	}
	//clamps the cube map to the edge
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//sets the skybox positions
	std::vector<GLfloat> SkyboxPositions =
	{
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f
	};

	//creates buffer for the skybox positiosn
	GLuint positionsVBOSkybox{};

	glGenBuffers(1, &positionsVBOSkybox);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBOSkybox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * SkyboxPositions.size(), SkyboxPositions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//wraps the buffer

	glGenVertexArrays(1, &m_VAOSkybox);

	glBindVertexArray(m_VAOSkybox);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVBOSkybox);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindVertexArray(0);

	return true;
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{			
	// Configure pipeline settings
	glEnable(GL_CULL_FACE);

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clear buffers from previous frame
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 1.0f, 5000.0f);

	// Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;

	//disables the depth for rendering the skybox
	glUseProgram(m_programSkybox);

	glm::mat4 model_xform = glm::mat4(1);

	GLuint Skybox_uniform_id = glGetUniformLocation(m_programSkybox, "skybox");
	// Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform2 = glm::mat4(glm::mat3(view_xform));
	glm::mat4 combined_xform2 = projection_xform * view_xform2;

	//send the model matrix 
	GLuint model_xform_id2 = glGetUniformLocation(m_programSkybox, "model_xform");
	glUniformMatrix4fv(model_xform_id2, 1, GL_FALSE, glm::value_ptr(model_xform));

	// Send the model matrix to the shader in a uniform
	GLuint combined_xform_id2 = glGetUniformLocation(m_programSkybox, "combined_xform");
	glUniformMatrix4fv(combined_xform_id2, 1, GL_FALSE, glm::value_ptr(combined_xform2));

	//while the depth mask is disabled this binds the cubemap texture and then draws it, then it renables the depth mask
	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture);
	glBindVertexArray(m_VAOSkybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);
	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);
	GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	

	

   // Send the model matrix to the shader in a uniform
	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");
	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

	//send the view matrix to the shader in a uniform
	GLuint view_xform_id = glGetUniformLocation(m_program, "view_xform");
	glUniformMatrix4fv(view_xform_id, 1, GL_FALSE, glm::value_ptr(view_xform));

	//send the camera position to the shader in a uniform
	glm::vec3 Camera_position = camera.GetPosition();
	GLuint Camera_position_id = glGetUniformLocation(m_program, "CameraPosition");
	glUniform3fv(Camera_position_id, 1, glm::value_ptr(Camera_position));

	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);
	// Bind our VAO and render
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);


	glUseProgram(m_programJeep);
	
	// Send the combined matrix to the shader in a uniform
	GLuint Jeepcombined_xform_id = glGetUniformLocation(m_programJeep, "Jeepcombined_xform");
	glUniformMatrix4fv(Jeepcombined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	glm::mat4 Jeepmodel_xform = glm::mat4(1);

	 

	// Send the model matrix to the shader in a uniform
	GLuint Jeepmodel_xform_id = glGetUniformLocation(m_programJeep, "Jeepmodel_xform");
	glUniformMatrix4fv(Jeepmodel_xform_id, 1, GL_FALSE, glm::value_ptr(Jeepmodel_xform));

	// send the camera position to the shader in a uniform
	glm::vec3 JeepCamera_position = camera.GetPosition();
	GLuint JeepCamera_position_id = glGetUniformLocation(m_programJeep, "JeepCameraPosition");
	glUniform3fv(Camera_position_id, 1, glm::value_ptr(JeepCamera_position));
	// binds the jeep texture and sends it to the shader in a uniform
	glBindTexture(GL_TEXTURE_2D, jeepTex);
	glUniform1i(glGetUniformLocation(m_programJeep, "sampler_tex"), 0);

	// Bind our VAO and render
	glBindVertexArray(m_VAOJeep);
	glDrawElements(GL_TRIANGLES, m_numElementsJeep, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);


	//changes the program being used
	glUseProgram(m_programCube);
	//sets the cube position
	glm::mat4 Cubemodel_xform = glm::mat4(1);
	Cubemodel_xform = glm::translate(Cubemodel_xform, glm::vec3{ 1000.f, 500.f, 500.f });

	static float angle = 0;
	static bool rotatey = true;
	//rotates the cube
	if (rotatey) // rotate around y axis		
		Cubemodel_xform = glm::rotate(Cubemodel_xform, angle, glm::vec3{ 0 ,1,0 });
	else // rotate around x axis		
		Cubemodel_xform = glm::rotate(Cubemodel_xform, angle, glm::vec3{ 1 ,0,0 });

	angle += 0.001f;
	if (angle > glm::two_pi<float>())
	{
		angle = 0;
		rotatey = !rotatey;
	}
	// Send the combined matrix to the shader in a uniform
	GLuint Cubecombined_xform_id = glGetUniformLocation(m_programCube, "Cubecombined_xform");
	glUniformMatrix4fv(Cubecombined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	// Send the model matrix to the shader in a uniform
	GLuint Cubemodel_xform_id = glGetUniformLocation(m_programCube, "Cubemodel_xform");
	glUniformMatrix4fv(Cubemodel_xform_id, 1, GL_FALSE, glm::value_ptr(Cubemodel_xform));

	
	// Bind our VAO and render
	glBindVertexArray(m_VAOCube);
	glDrawElements(GL_TRIANGLES, m_numElementsCube, GL_UNSIGNED_INT, (void*)0);

	
}

