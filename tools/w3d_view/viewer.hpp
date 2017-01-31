#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include <libw3d/w3d.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.hpp"
#include "arcball.hpp"

namespace w3dview
{
	class Viewer
	{
	public:
		Viewer();
		~Viewer();
		//Run
		void Run();
		bool SetInput(const std::string& name);
		void SetWidth(const unsigned int width);
		void SetHeight(const unsigned int height);

		inline Arcball& GetArcball()
		{
			return m_arcball;
		}
	private:
		static void APIENTRY Callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
		static void Error(int error, const char* description);
		static void Resize(GLFWwindow* win, int width, int height);
		static void MouseMove(GLFWwindow* window, double xpos, double ypos);
		static void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButton(GLFWwindow* window, int button, int action, int mods);
	private:
		bool m_debug;
		unsigned int m_width;
		unsigned int m_height;
		GLFWwindow* m_window;
		CompiledModel m_model;
		Shader m_default;
		Arcball m_arcball;
		GLuint m_vao;
		glm::mat4 m_projection;
		glm::mat4 m_vp;
	};
}