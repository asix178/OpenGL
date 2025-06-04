//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <iostream>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//void jump();
//
//const unsigned int SCREEN_WIDTH = 1200;
//const unsigned int SCREEN_HEIGHT = 800;
//
//glm::vec3 move(0.0f);
//float rot_angle = glm::radians(90.0f);
//bool jump_active = false;   
//float yPos = 0.0f;
//float start_z = 0.0f;
//double time_now;
//float jump_time = 0;
//double v0 = 0;
//float g = -9.98f;
//
//
////shaders
//const char* vertexShaderSource = R"glsl(
//#version 430 core
//
//layout (location = 0) in vec3 Pos;
//layout (location = 2) uniform mat4 u_ProjMatrix;
//layout (location = 3) uniform mat4 u_ViewMatrix;
//layout (location = 4) uniform mat4 u_ModelMatrix;
//
//void main()
//{
//   gl_Position = u_ProjMatrix * u_ViewMatrix * u_ModelMatrix * vec4(Pos, 1.0);
//}
//)glsl";
//
//const char* fragmentShaderSource = R"glsl(
//#version 430 core
//
//layout (location = 1) uniform vec4 TriangleColor;
//
//out vec4 FragColor;
//void main()
//{
//   FragColor = TriangleColor;
//}
//)glsl";
//
//int main()
//{
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Przeksztalcenia", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    glEnable(GL_DEPTH_TEST);
//
//
//    // build and compile our shader program
//    // vertex shader
//    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    int success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    // fragment shader
//    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    // link shaders
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//
//    static const float Qube[] = {
//      -0.5f, -0.5f, -0.5f, //bottom
//       0.5f, -0.5f, -0.5f,
//       0.5f,  0.5f, -0.5f,
//      -0.5f,  0.5f, -0.5f,
//
//      -0.5f, -0.5f,  0.5f, //top
//       0.5f, -0.5f,  0.5f,
//       0.5f,  0.5f,  0.5f,
//      -0.5f,  0.5f,  0.5f,
//
//      -20.0f, -0.5f, 50.0f, //ground
//      -20.0f, -0.5f, -50.0f,
//       20.0f, -0.5f, -50.0f,
//       20.0f, -0.5f, 50.0f
//    };
//
//    unsigned int Indices[] = {
//      0, 1, 2, //bottom
//      2, 3, 0,
//      4, 5, 6, //top
//      6, 7, 4,
//      0, 1, 5, //first side
//      5, 4, 0,
//      1, 2, 6, //second side
//      6, 5, 1,
//      2, 3, 7, //third side
//      7, 6, 2,
//      3, 0, 4, //fourth side
//      4, 7, 3,
//
//      8, 9, 10, //ground
//      10, 11, 8
//    };
//
//    unsigned int VertexBufferId, VertexArrayId, ElementBufferId;
//    glGenVertexArrays(1, &VertexArrayId);
//    glGenBuffers(1, &VertexBufferId);
//    glGenBuffers(1, &ElementBufferId);
//    glBindVertexArray(VertexArrayId);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Qube), Qube, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferId);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //glBindBuffer(GL_ARRAY_BUFFER, 0);
//    //glBindVertexArray(0);
//
//    glfwSetTime(0);
//    glfwSwapInterval(1);
//
//    //game loop
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//
//        glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glUseProgram(shaderProgram);
//        glBindVertexArray(VertexArrayId);
//
//        //macierz rzutowania
//
//        glm::mat4 ProjMatrix = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);
//        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(ProjMatrix));
//
//        //macierz widoku
//        //jest w niej kamera
//
//       //View matrix
//        glm::vec3 center = glm::vec3(move.x, move.z, -move.y);
//        glm::vec3 eye = glm::vec3(move.x - 10.0f, 5.0f, - move.y + 10.0f);
//        glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);;
//
//        glm::mat4 ViewMatrix = glm::lookAt(eye, center, camera_up);
//
//        glm::mat4 ModelMatrix = glm::mat4(1.0f);
//        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
//        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
//
//        //pod�oga
//
//        glUniform4f(1, 0.3f, 0.6f, 0.3f, 1.0f);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(unsigned int)));
//
//        //sze�cian
//
//        ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(move.x, move.z, -move.y));
//        ModelMatrix = glm::rotate(ModelMatrix, rot_angle, glm::vec3(0.0f, 1.0f, 0.0f));
//        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
//
//        glUniform4f(1, 0.0f, 0.0f, 1.0f, 1.0f);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//        if (jump_active){
//            jump();
//        }
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &VertexArrayId);
//    glDeleteBuffers(1, &VertexBufferId);
//    glDeleteBuffers(1, &ElementBufferId);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}
//
//void processInput(GLFWwindow* window)
//{
//    float s = 0.1;
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//        rot_angle -= glm::radians(1.0f);
//    }
//    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//        rot_angle += glm::radians(1.0f);
//    }
//    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//
//        move.x -= s * glm::cos(rot_angle);
//        move.y -= s * glm::sin(rot_angle);
//    }
//    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//        move.x += s * glm::cos(rot_angle);
//        move.y += s * glm::sin(rot_angle);
//    }
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//        rot_angle += glm::radians(1.0f);
//
//    }
//    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
//        if (!jump_active) {
//            jump_active = true;
//            jump_time = glfwGetTime();
//            v0 = 5.0;
//        }
//    }
//}
//
//void jump() {
//    float t = 0.0f;
//    t = glfwGetTime() - jump_time;
//    move.z = start_z + v0 * t + 0.5f * g * t * t;
//
//    if (move.z <= start_z) {
//        jump_active = false;
//        move.z = 0.0;
//    }
//
//    return;
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}