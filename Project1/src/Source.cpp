#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
#include<sstream>
#include<unordered_map>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Tests/Test.h"
#include "Tests/TestClearColor.h"
#include "Tests/TestTexture2D.h"


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	/* Create a windowed mode window and its OpenGL context */
	const char* glsl_version = "#version 130";
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		std::cout << "Something's wrong" << std::endl;
	}
	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

	{
		float positions[] = {
			  -75.0f,  -75.0f, 0.0f, 0.0f,
			   75.0f,  -75.0f, 1.0f, 0.0f,
			   75.0f,   75.0f, 1.0f, 1.0f,
			  -75.0f,   75.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		/*
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);


		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;

		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Texture texture("res/textures/doggo.png");
		texture.Bind();
		*/


		Renderer renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;


		ImGui::StyleColorsDark();
		
		test::Test* CurrentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(CurrentTest);
		CurrentTest = testMenu;
		
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture Test");

		
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
		bool sliderA = false, sliderB = false;
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			{

				if (CurrentTest) {
					CurrentTest->OnUpdate(0.0f);
					CurrentTest->OnRender();
					ImGui::Begin("Test");
					if (CurrentTest != testMenu && ImGui::Button("<-")) {
						GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
						delete CurrentTest;
						CurrentTest = testMenu;
					}
					CurrentTest->OnImguiRender();
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					ImGui::End();
				}

			}
			ImGui::Render();
	
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */

		}
		delete CurrentTest;
		if (CurrentTest != testMenu) {
			delete testMenu;
		}

	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}