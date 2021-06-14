#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
#include<sstream> 
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
			-0.5f, -0.5f,
			 0.5f,  -0.5f,
			 0.5f, 0.5f,
			 -0.5f, 0.5f,
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.5f, 0.0f, 0.5f, 1.0f);

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		VertexBufferLayout layout;

		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		shader.UnBind();
		vb.UnBind();
		va.UnBind();
		ib.UnBind();

		Renderer renderer;

		float r = 0.0f, increment = 0.05f;


		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);
			renderer.Draw(va, ib, shader);
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
				increment = -0.01f;
			else if (r < 0.0f)
				increment = 0.01f;
			r += increment;
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}
	glfwTerminate();
	return 0;
}