//#pragma once
//
//#include <glm/glm.hpp>
//
//class Enemy {
//public:
//    glm::vec3 position;
//    float speed;
//    float width;
//    float height;
//
//    bool jumpPassed;
//
//    Enemy() {
//        speed = 0.03f;
//        width = 1.0f;
//        height = 1.0f;
//        jumpPassed = false;
//    }
//
//    void Respawn(const glm::vec3& playerPos) {
//        position = glm::vec3(playerPos.x + 6.0f, 0.0f, 0.0f);
//        jumpPassed = false;
//    }
//
//    void Move() {
//        position.x -= speed;
//    }
//
//    bool IsOutOfView() const {
//        return position.x < -6.0f;
//    }
//
//    bool CheckCollision(const glm::vec3& playerPos, float playerWidth, float playerHeight) const {
//        return glm::abs(playerPos.x - position.x) < 0.5f * (playerWidth + width) &&
//            glm::abs(playerPos.y - position.y) < 0.5f * (playerHeight + height);
//    }
//};