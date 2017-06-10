#include "flextGL.h"
#include "viewer.hpp"
#include <iostream>
using namespace w3dview;

static inline const std::string getBasePath(const std::string& file)
{
	auto found=file.find_last_of("/\\");
	return file.substr(0,found);
}

void APIENTRY Viewer::Callback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if(type!=GL_DEBUG_TYPE_ERROR)
		return;

	std::cout << "[GL]"<< message << std::endl;
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

	m_window = glfwCreateWindow(m_width, m_height, "W3D Viewer", NULL, NULL);
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
	//load shaders
	glViewport(0, 0, m_width, m_height);
	const std::string DefaultW3D_vs =
#include "shaders/DefaultW3D.vs"
		;
	const std::string DefaultW3D_fs =
#include "shaders/DefaultW3D.fs"
		;
	m_default.Load(DefaultW3D_vs, DefaultW3D_fs);
	m_default.addUniform("m");
	m_default.addUniform("vp");
	m_default.addUniform("has_skinning");
	m_default.addUniform("bones");
	m_default.addUniform("has_diffuse");
	m_default.addUniform("diffuse");
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
		m_vp = m_projection * m_arcball.GetViewMatrix();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		m_default.Use();
		glUniformMatrix4fv(m_default.uniform("vp"), 1, false, glm::value_ptr(m_vp));
		
		m_cmodel.Render(m_default);
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

bool Viewer::SetInput(const std::string& name)
{
	m_model = libw3d::Loader::FromFile(name, true);
	if(!m_model.IsValid())
		return false;

	std::string title = "W3D Viewer - " + name;
	glfwSetWindowTitle(m_window,title.c_str());
	m_base = getBasePath(name);
	m_cmodel.Create(m_model,m_base);
	return true;
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

bool Viewer::SetAnimation(const std::string& ani)
{
	auto anim = libw3d::Loader::FromFile(ani, false);

	m_model.AddAnimation(anim);
	m_cmodel.Create(m_model,m_base);
	return true;
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