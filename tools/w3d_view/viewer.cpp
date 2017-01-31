#include "flextGL.h"
#include "viewer.hpp"
#include <iostream>
using namespace libw3d;

std::string Viewer::s_vertSrc =
"#version 330\n"
"layout (location=0)in vec3 pos;\n"
"layout (location=1)in vec3 normal;\n"
"layout (location=2)in vec2 txcoord;\n"
"layout (location=3)in uint boneId1;\n"
"layout (location=4)in uint boneId2;\n"
"uniform mat4 vp;\n"
"uniform mat4 m;\n"
"out vec4 fnormal;\n"
"out vec2 ftxcoord;\n"
"void main()\n"
"{\n"
"	gl_Position = vp*m*vec4(pos,1);\n"
"	ftxcoord = txcoord;\n"
"   fnormal = vec4(normal,0);\n"
"}";
std::string Viewer::s_fragSrc =
"#version 330\n"
"in vec4 fnormal;\n"
"in vec2 ftxcoord;\n"
"uniform bool has_diffuse;\n"
"uniform sampler2D diffuse;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"	if(has_diffuse)\n"
"		color = texture(diffuse,ftxcoord);\n"
"	else\n"
"		color = vec4(1.0,0,1.0,1.0);\n"
"}";


void APIENTRY Viewer::Callback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	std::cout << message << std::endl;
}

Viewer::Viewer() : m_width(800),m_height(600), m_vao(0), m_arcball(100,glm::vec3(1,0,0))
{
	glfwSetErrorCallback(Error);
	if (glfwInit() == GLFW_FALSE)
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
	#else
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif	

	m_window = glfwCreateWindow(m_width, m_height, "W3d viewer", NULL, NULL);
	if (m_window == NULL)
		exit(EXIT_FAILURE);

	glfwSetWindowSizeCallback(m_window, Resize);
	glfwSetCursorPosCallback(m_window, MouseMove);
	glfwSetScrollCallback(m_window, MouseScroll);
	glfwSetMouseButtonCallback(m_window, MouseButton);
	glfwSetWindowUserPointer(m_window, this);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(0);

	flextInit(m_window);
	if (FLEXT_KHR_debug)
	{
		#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Viewer::Callback, nullptr);
		#endif
	}
	glViewport(0, 0, m_width, m_height);
	m_shader.Load(s_vertSrc, s_fragSrc);
	m_shader.addUniform("m");
	m_shader.addUniform("vp");
	m_shader.addUniform("has_diffuse");
	m_shader.addUniform("diffuse");
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	float ratio = m_width / (float)m_height;
	m_projection = glm::perspective(glm::radians(45.0f), 
		ratio, 1.0f, 1000.0f);
}

Viewer::~Viewer()
{
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
		
	glfwTerminate();
}

void Viewer::Run()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glfwShowWindow(m_window);

	while (!glfwWindowShouldClose(m_window))
	{
		m_vp = m_projection*m_arcball.GetViewMatrix();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		m_shader.Use();
		glUniformMatrix4fv(m_shader.uniform("vp"), 1, false, glm::value_ptr(m_vp));
		
		m_model.Render(m_shader);
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

bool Viewer::SetInput(const std::string& name)
{
	auto model = libw3d::Loader::FromFile(name, true);
	m_model.Create(model);
	return false;
}

void Viewer::SetWidth(const unsigned int width)
{
	m_width = width;
	glfwSetWindowSize(m_window, m_width, m_height);
}

void Viewer::SetHeight(const unsigned int height)
{
	m_height = height;
	glfwSetWindowSize(m_window,m_width, m_height);
}

void Viewer::Error(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

void Viewer::Resize(GLFWwindow* win, int width, int height)
{

}

void Viewer::MouseMove(GLFWwindow * window, double xpos, double ypos)
{
	Viewer* v = reinterpret_cast<Viewer*>(glfwGetWindowUserPointer(window));
	v->GetArcball().Move(xpos, ypos);
}

void Viewer::MouseScroll(GLFWwindow * window, double xoffset, double yoffset)
{
	Viewer* v = reinterpret_cast<Viewer*>(glfwGetWindowUserPointer(window));
	v->GetArcball().Distance(-5*yoffset);
}

void Viewer::MouseButton(GLFWwindow* window, int button, int action, int mods)
{
	Viewer* v = reinterpret_cast<Viewer*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			v->GetArcball().SetDown(true);
		else if(action == GLFW_RELEASE)
			v->GetArcball().SetDown(false);
	}
}