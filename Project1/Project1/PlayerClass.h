//#pragma once
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//class Player {
//public:
//	glm::vec3 position;
//	glm::vec3 color;
//	float speed;
//	float jump_speed;
//	bool jump_active;
//
//	Player()
//	{
//		jump_active = false;
//		position = glm::vec3(0.0f, 0.0f, 0.0f);
//		color = glm::vec3(0.0f, 0.0f, 1.0f);
//		speed = 1.0f;
//		jump_speed = 1.5f;
//	}
//
//	/*void FarAway()
//	{
//		if (glm::abs(position.x) > 17.0f || glm::abs(position.z) > 45.0f) color = glm::vec3(1.0f, 0.0f, 0.0f);
//		else
//		{
//			color = glm::vec3(0.0f, 0.0f, 1.0f);
//		}
//	}*/
//
//	void Move(glm::vec3 change)
//	{
//		position = position + speed * change;
//
//		float screenBoundaryX = 5.0f;
//
//		if (position.x > screenBoundaryX)
//			position.x = -screenBoundaryX;
//		else if (position.x < -screenBoundaryX)
//			position.x = screenBoundaryX;
//	}
//
//	void CheckJump(float t, float g, float yPos)
//	{
//		if (jump_active == true)
//		{
//			position.y += jump_speed * t + 0.3f * g * t * t;
//			if (position.y < yPos)
//			{
//				jump_active = false;
//				position.y = yPos;
//			}
//		}
//	}
//};