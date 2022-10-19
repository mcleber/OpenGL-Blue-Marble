#include <iostream>
#include <cassert>
#include <array>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


// Window 4:3
int Width = 800;
int Height = 600;

std::string ReadFile(const char* FilePath)
{
	std::string FileContents;

	if (std::ifstream FileStream{ FilePath, std::ios::in })
	{
		FileContents.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
	}

	return FileContents;

}

void CheckShader(GLuint ShaderId)
{
	GLint Result = GL_TRUE;
	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		GLint InfoLogLength = 0;
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::string ShaderInfoLog(InfoLogLength, '\0');
			glGetShaderInfoLog(ShaderId, InfoLogLength, nullptr, &ShaderInfoLog[0]);

			std::cout << "Shader error." << std::endl;
			std::cout << ShaderInfoLog << std::endl;

			assert(false);

		}
						
	}
}


GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile)
{
	std::string VertexShaderSource = ReadFile(VertexShaderFile);
	std::string FragmentShaderSource = ReadFile(FragmentShaderFile);

	 assert(!VertexShaderSource.empty());
	 assert(!FragmentShaderSource.empty());

	// Id Vertex e Fragment Shader
	GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	std::cout << "Compiling..." << VertexShaderFile << std::endl;
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	glShaderSource(VertexShaderId, 1, &VertexShaderSourcePtr, nullptr);
	glCompileShader(VertexShaderId);

	CheckShader(VertexShaderId);

	std::cout << "Compiling..." << FragmentShaderFile << std::endl;
	const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
	glShaderSource(FragmentShaderId, 1, &FragmentShaderSourcePtr, nullptr);
	glCompileShader(FragmentShaderId);

	CheckShader(FragmentShaderId);

	std::cout << "Linking the program." << std::endl;
	GLuint ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);

	GLint Result = GL_TRUE;
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		// Log
		GLint InfoLogLength = 0;
		glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::string ProgramInfoLog(InfoLogLength, '\0');
			glGetProgramInfoLog(ProgramId, InfoLogLength, nullptr, &ProgramInfoLog[0]);

			std::cout << "Error linking program." << std::endl;
			std::cout << ProgramInfoLog << std::endl;

			 assert(false);
			}
		
	}

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(VertexShaderId);
	glDeleteShader(FragmentShaderId);

	return ProgramId;
}

GLuint LoadTexture(const char* TextureFile)
{
	std::cout << "Loading texture... " << TextureFile << std::endl;

	stbi_set_flip_vertically_on_load(true);
	
	int TextureWidth = 0, TextureHeight = 0;
	int NumberOfComponents = 0;
	unsigned char* TextureData = stbi_load(TextureFile, &TextureWidth, &TextureHeight, &NumberOfComponents, 3); // 3 -> RGB

	assert(TextureData);

	// Texture
	GLuint TextureId;
	glGenTextures(1, &TextureId); // 1 texture

	// Enable texture
	glBindTexture(GL_TEXTURE_2D, TextureId);

	// Copy texture to GPU
	GLint Level = 0;
	GLint Border = 0;
	glTexImage2D(GL_TEXTURE_2D, Level, GL_RGB, TextureWidth, TextureHeight, Border, GL_RGB, GL_UNSIGNED_BYTE, TextureData);

	// Filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S é a coordenada U do mapa UV
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T é a coordenada V do mapa UV

	// MIPMAP
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(TextureData);

	return TextureId;

}

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Color;
	glm::vec2 UV;
};

struct DirectionalLight
{
	glm::vec3 Direction;
	GLfloat Intensity;
};


GLuint LoadGeometry()
{

	std::array<Vertex, 6> Quad = {
		Vertex { glm::vec3 { -1.0f, -1.0f, 0.0f }, 
				 glm::vec3{ 0.0f, 0.0f, 1.0f }, 
				 glm::vec3{ 1.0f, 0.0f, 0.0f }, 
				 glm::vec2{ 0.0f, 0.0f } },

		Vertex { glm::vec3 { 1.0f, -1.0f, 0.0f },
				 glm::vec3{ 0.0f, 0.0f, 1.0f }, 
				 glm::vec3{ 0.0f, 1.0f, 0.0f }, 
				 glm::vec2{ 1.0f, 0.0f } },

		Vertex { glm::vec3 { 1.0f,  1.0f, 0.0f }, 
				 glm::vec3{ 0.0f, 0.0f, 1.0f }, 
				 glm::vec3{ 1.0f, 0.0f, 0.0f }, 
				 glm::vec2{ 0.0f, 1.0f } },

		Vertex { glm::vec3 { -1.0f,  1.0f, 0.0f },
				 glm::vec3{ 0.0f, 0.0f, 1.0f }, 
				 glm::vec3{ 0.0f, 0.0f, 1.0f }, 
				 glm::vec2{ 0.0f, 1.0f } },
	};

	std::array<glm::ivec3, 2> Indices = { glm::ivec3{ 0, 1, 3 }, glm::ivec3{ 3, 1, 2 } };

	GLuint VertexBuffer;

	// VertexBuffer (VBO).
	glGenBuffers(1, &VertexBuffer);

	// EBO
	GLuint ElementBuffer = 0;
	glGenBuffers(1, &ElementBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad.data(), GL_STATIC_DRAW);

	// Element Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices.data(), GL_STATIC_DRAW);

	// Vertex Array Object (VAO)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// VAO
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1); 
	glEnableVertexAttribArray(2); 
	glEnableVertexAttribArray(3); 

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr); // sizeof(Vertex) é o STRIDE . nullptr indica o offset (começa em 0 Byte).
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));  // GL_TRUE indica que é normalizado.
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

	glBindVertexArray(0);

	return VAO;

} 

void GenerateSphereMesh(GLuint Resolution, std::vector<Vertex>& Vertices, std::vector<glm::ivec3>& Indices)
{
	Vertices.clear();
	Indices.clear();

	constexpr float Pi = glm::pi<float>();
	constexpr float TwoPi = glm::two_pi<float>();
	const float InvResolution = 2.0f / static_cast<float>(Resolution - 1);

	// Discretizar o domínio
	for (GLuint UIndex = 0; UIndex < Resolution; UIndex++)
	{
		const float U = UIndex * InvResolution;
		// Interpolação linear entre 0 e TwoPi
		const float	Theta = glm::mix(0.0f, Pi, static_cast<float>(U));

		for (GLuint VIndex = 0; VIndex < Resolution; VIndex++)
		{
			const float	 V = VIndex * InvResolution;
			const float Phi = glm::mix(0.0f, TwoPi, static_cast<float>(V));

			glm::vec3 VertexPosition =
			{
				glm::sin(Theta) * glm::cos(Phi),
				glm::sin(Theta) * glm::sin(Phi),
				glm::cos(Theta)  // Considera o eixo Z para cima

			};

			Vertex Vertex
			{
				VertexPosition,
				glm::normalize(VertexPosition),
				glm::vec3{ 1.0f, 1.0f, 1.0f },
				glm::vec2{ 1.0f - U, V }

			};

			Vertices.push_back(Vertex);

		}
	}

	for (GLuint U = 0; U < Resolution - 1; U++)
	{
		for (GLuint V = 0; V < Resolution - 1; V++)
		{
			GLuint P0 = U + V * Resolution;
			GLuint P1 = (U + 1) + V * Resolution;
			GLuint P2 = (U + 1) + (V + 1) * Resolution;
			GLuint P3 = U + (V + 1) * Resolution;

			Indices.push_back(glm::ivec3{ P0, P1, P3 });
			Indices.push_back(glm::ivec3{ P3, P1, P2 });

		}
	}

}

GLuint LoadSphere(GLuint& NumVertices, GLuint& NumIndices)
{
	std::vector<Vertex> Vertices;
	std::vector<glm::ivec3> Triangles;
	GenerateSphereMesh(100, Vertices, Triangles); // Resolução da Esfera

	NumVertices = Vertices.size();
	NumIndices = Triangles.size() * 3;

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

	// (EBO - Elemento Buffer Object).
	GLuint ElementBuffer;
	glGenBuffers(1, &ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices * sizeof(GLuint), Triangles.data(), GL_STATIC_DRAW); // Número em Bytes dentro do array de triângulos.

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

	glBindVertexArray(0);

	return VAO;
		
}

class FlyCamera
{
public:

	float Speed = 4.0f;
	float Sensitivity = 0.1f;

	// Matriz View
	glm::vec3 Location{ 0.0f, 0.0f, 5.0f };
	glm::vec3 Direction{ 0.0f, 0.0f, -1.0f };
	glm::vec3 Up{ 0.0f, 1.0f, 0.0f };

	// Matriz Projection
	float FieldOfView = glm::radians(45.0f);
	float AspectRatio = Width / Height;
	float Near = 0.01f;
	float Far = 1000.0f;

	// Retorna só a matriz view
	glm::mat4 GetView() const
	{
		return glm::lookAt(Location, Location + Direction, Up);
	}

	glm::mat4 GetViewProjection() const
	{
		glm::mat4 Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
		return Projection * GetView();
	}
	
	void MoveForward(float Amount)
	{
		Location += glm::normalize(Direction) * Amount * Speed;
	}

	void MoveRight(float Amount)
	{
		glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));
		Location += Right * Amount * Speed;
	}

	void Look(float Yaw, float Pitch)
	{
		Yaw *= Sensitivity;
		Pitch *= Sensitivity;

		const glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));
		const glm::mat4 I = glm::identity<glm::mat4>();

		glm::mat4 YawRotation = glm::rotate(I, glm::radians(Yaw), Up);
		glm::mat4 PitchRotation = glm::rotate(I, glm::radians(Pitch), Right);

		Direction = YawRotation * PitchRotation * glm::vec4{ Direction, 0.0f };

	}

};

FlyCamera Camera;
bool bEnableMouseMovement = false;
glm::vec2 PreviousCursor{ 0.0f, 0.0f };

void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Modifiers)
{
	std::cout << "Button: " << Button << "Action: "
		<< "Modifiers: " << Modifiers << std::endl;

	if (Button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (Action == GLFW_PRESS)
		{
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			double X, Y;
			glfwGetCursorPos(Window, &X, &Y);

			PreviousCursor = glm::vec2{ X, Y };

			bEnableMouseMovement = true;

		}

		if (Action == GLFW_RELEASE)
		{
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			bEnableMouseMovement = false;

		}

	}

}

void MouseMotionCallback(GLFWwindow* Window, double X, double Y)
{
	if (bEnableMouseMovement)
	{
		// Delta Cursor
		glm::vec2 CurrentCursor{ X, Y };
		glm::vec2 DeltaCursor = CurrentCursor - PreviousCursor;

		Camera.Look(DeltaCursor.x, DeltaCursor.y);

		PreviousCursor = CurrentCursor;

	}
	
}

void Resize(GLFWwindow* Window, int NewWidth, int NewHeight)
{
	Width = NewWidth;
	Height = NewHeight;

	Camera.AspectRatio = static_cast<float>(Width) / Height;
	glViewport(0, 0, Width, Height);
}

int main()
{
	// GLFW
	if (!glfwInit())
	{
		std::cout << "Error initializing GLFW" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_DEPTH_BITS, 32);

	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);

	// GLFW
	if (!Window)
	{
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(Window);

	// Habilita ou desabilita o V-Sync
	glfwSwapInterval(0);

	// Callbacks no GLFW
	glfwSetMouseButtonCallback(Window, MouseButtonCallback);
	glfwSetCursorPosCallback(Window, MouseMotionCallback);
	glfwSetFramebufferSizeCallback(Window, Resize);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing GLEW" << std::endl;
		glfwTerminate();
		return 1;
	}

	GLint GLMajorVersion{ 0 };
	GLint GLMinorVersion{ 0 };
	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);
	std::cout << "OpenGL Version: " << GLMajorVersion << "." << GLMinorVersion << std::endl;

	std::cout << "OpenGL Vendor   : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version  : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version    : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "--------------------------------------------------------" << std::endl;

	Resize(Window, Width, Height);

	GLuint ProgramId = LoadShaders("shaders/triangle_vertex.glsl", "shaders/triangle_fragment.glsl");

	GLuint TextureId = LoadTexture("textures/earth_2k.jpg"); 
	GLuint CloudTextureId = LoadTexture("textures/earth_clouds_2K.jpg");

	// Retorna o VAO
	GLuint QuadVAO = LoadGeometry();

	GLuint SphereNumVertices = 0;
	GLuint SphereNumIndices = 0;
	GLuint SphereVAO = LoadSphere(SphereNumVertices, SphereNumIndices);

	std::cout << "Number of vertices of the sphere: " << SphereNumVertices << std::endl;
	std::cout << "Number of sphere indices: " << SphereNumIndices << std::endl;

	// Model Matrix
	glm::mat4 I = glm::identity<glm::mat4>();
	glm::mat4 ModelMatrix = glm::rotate(I, glm::radians(90.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });

	// Window background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	
	// Guarda o tempo do frame anterior
	double PreviousTime = glfwGetTime();

	// Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); 

	// Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Light
	DirectionalLight Light;
	Light.Direction = glm::vec3{ 0.0f, 0.0f, -1.0f };
	Light.Intensity = 0.8f;

	while (!glfwWindowShouldClose(Window))
	{
		
		double CurrentTime = glfwGetTime();
		double DeltaTime = CurrentTime - PreviousTime;
		if (DeltaTime > 0.0)
		{
			PreviousTime = CurrentTime;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shader
		glUseProgram(ProgramId);

		glm::mat4 NormalMatrix = glm::inverse(glm::transpose(Camera.GetView() * ModelMatrix));
		glm::mat4 ViewProjectionMatrix = Camera.GetViewProjection();
		glm::mat4 ModelViewProjection = ViewProjectionMatrix * ModelMatrix;

		GLint TimeLoc = glGetUniformLocation(ProgramId, "Time");
		glUniform1f(TimeLoc, CurrentTime);

		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramId, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		GLint NormalMatrixLoc = glGetUniformLocation(ProgramId, "NormalMatrix");
		glUniformMatrix4fv(NormalMatrixLoc, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureId);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, CloudTextureId);

		GLint TextureSamplerLoc = glGetUniformLocation(ProgramId, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);

		GLint CloudTextureLoc = glGetUniformLocation(ProgramId, "CloudTexture");
		glUniform1i(CloudTextureLoc, 1);

		GLint LightDirectionLoc = glGetUniformLocation(ProgramId, "LightDirection");
		glUniform3fv(LightDirectionLoc, 1, glm::value_ptr(Camera.GetView() * glm::vec4{ Light.Direction, 0.0f }));

		GLint LightIntensityLoc = glGetUniformLocation(ProgramId, "LightIntensity");
		glUniform1f(LightIntensityLoc, Light.Intensity);

		glBindVertexArray(SphereVAO);
		
		glPointSize(10.0f);
		glLineWidth(10.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, SphereNumIndices, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwPollEvents();

		glfwSwapBuffers(Window);

		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Camera.MoveForward(1.0f * DeltaTime);
		}

		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Camera.MoveForward(-1.0f * DeltaTime);
		}

		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Camera.MoveRight(-1.0f * DeltaTime);
		}

		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Camera.MoveRight(1.0f * DeltaTime);
		}

	}

	glDeleteVertexArrays(1, &QuadVAO);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	std::cout << std::endl;

	// system("PAUSE");
	return 0;

}
