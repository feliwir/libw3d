#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
class Arcball
{
public:
	Arcball(float distance, glm::vec3 dir, glm::vec3 center=glm::vec3()) 
		: m_distance(distance),m_center(center), m_dir(dir), m_down(false)
	{
		Recalculate();
	}

	inline void Recalculate()
	{
		m_view = glm::lookAt(m_center + m_dir*m_distance, m_center, glm::vec3(0, 1, 0));
	}

	inline void Move(double x, double y)
	{
		glm::dvec2 new_mouse = glm::dvec2(x, y);
		
		if (m_down)
		{
			glm::vec2 dir = new_mouse - m_mouse;

			m_dir = glm::rotateY(m_dir, -0.01f*dir.x);
			m_dir = glm::rotateX(m_dir, -0.01f*dir.y);
			Recalculate();
		}

		m_mouse = new_mouse;
	}

	inline void Distance(float value)
	{
		m_distance += value;
		Recalculate();
	}

	inline void SetDown(bool down)
	{
		m_down = down;
	}

	inline const glm::mat4 GetViewMatrix()
	{
		return m_view;
	}
private:
	bool m_down;
	glm::dvec2 m_mouse;
	glm::mat4 m_view;
	float m_distance;
	glm::vec3 m_dir;
	glm::vec3 m_center;
};