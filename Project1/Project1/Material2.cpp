#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 800;

glm::vec3 move(0.0f, 0.0f, 3.0f);
glm::vec3 move2(0.0f, 0.0f, 3.0f);
glm::vec3 aLightPos = glm::vec3(0.0f, 1.0f, -3.0f);

//shaders
//vertex shader for objects
const char* vertexShaderSource = R"glsl(
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

layout (location = 2) uniform mat4 modelMat;
layout (location = 3) uniform mat4 viewMat;
layout (location = 4) uniform mat4 projMat;

out vec3 Normal;
out vec3 ObjPos;

void main()
{
    ObjPos = vec3(modelMat * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(modelMat))) * aNormal;  
    
    gl_Position = projMat * viewMat * vec4(ObjPos, 1.0);
}
)glsl";

//fragment shader for objects
const char* fragmentShaderSource = R"glsl(
#version 430 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;  
in vec3 ObjPos;  

out vec4 FragColor;

uniform Light light;
uniform Material object_mat;
layout (location = 5) uniform vec3 ViewPos; 

void main()
{
vec3 ambient = light.ambient * object_mat.ambient;
 
vec3 LightDir = normalize(light.position - ObjPos);
float diffuseStrength = max(dot(LightDir, Normal), 0.0);
vec3 diffuse = diffuseStrength * light.diffuse * object_mat.diffuse;
    
vec3 ViewDir = normalize(ViewPos - ObjPos);
vec3 ReflectDir = reflect(-LightDir, Normal);  
float specularStrength = pow(max(dot(ViewDir, ReflectDir),0.0), object_mat.shininess);
vec3 specular = light.specular * object_mat.specular;  
        
float distance = length(light.position - ObjPos);
vec3 FinalLightResult = 1/distance * (ambient + diffuse + specular);

FragColor = vec4(FinalLightResult, 1.0); 
}
)glsl";

//vertex shader for the light source
const char* vertexShaderLightSource = R"glsl(
#version 430 core

layout (location = 0) in vec3 aPos;

layout (location = 2) uniform mat4 modelMat;
layout (location = 3) uniform mat4 viewMat;
layout (location = 4) uniform mat4 projMat;

void main()
{
    
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
}
)glsl";

//fragment shader for the light source
const char* fragmentShaderLightSource = R"glsl(
#version 430 core

out vec4 FragColor;

void main()
{
FragColor = vec4(1.0, 1.0, 1.0, 1.0); 
}
)glsl";

//zderzenia
bool checkCollision(glm::vec3 posA, glm::vec3 scaleA, glm::vec3 posB, glm::vec3 scaleB) {
    bool collisionX = abs(posA.x - posB.x) <= ((scaleA.x + scaleB.x) / 2.0f);
    bool collisionY = abs(posA.y - posB.y) <= ((scaleA.y + scaleB.y) / 2.0f);
    bool collisionZ = abs(posA.z - posB.z) <= ((scaleA.z + scaleB.z) / 2.0f);
    return collisionX && collisionY && collisionZ;
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Przeksztalcenia", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    // build and compile our shader programs
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    //shader program for the light source
    // vertex shader
    unsigned int vertexShaderLight = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderLight, 1, &vertexShaderLightSource, NULL);
    glCompileShader(vertexShaderLight);

    glGetShaderiv(vertexShaderLight, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderLight, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShaderLight = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderLight, 1, &fragmentShaderLightSource, NULL);
    glCompileShader(fragmentShaderLight);

    glGetShaderiv(fragmentShaderLight, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderLight, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgramLight = glCreateProgram();
    glAttachShader(shaderProgramLight, vertexShaderLight);
    glAttachShader(shaderProgramLight, fragmentShaderLight);
    glLinkProgram(shaderProgramLight);

    glGetProgramiv(shaderProgramLight, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramLight, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShaderLight);
    glDeleteShader(fragmentShaderLight);


    static const float Qube[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VertexBufferId, VertexArrayId;
    glGenVertexArrays(1, &VertexArrayId);
    glGenBuffers(1, &VertexBufferId);
    glBindVertexArray(VertexArrayId);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Qube), Qube, GL_STATIC_DRAW);

    //dla wierzcholkow
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //dla wektorow normalnych
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glfwSetTime(0);
    glfwSwapInterval(1);

    //game loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VertexArrayId);

        //macierz rzutowania

        glm::mat4 projMat = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);
        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(projMat));

        //macierz widoku
		glm::vec3 camera_position = glm::vec3(0.0f, 20.0f, 5.0f); 
		glm::vec3 camera_destination = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 camera_up = glm::vec3(0.0f, 0.0f, -1.0f); 

        glm::mat4 viewMat = glm::lookAt(camera_position, camera_destination, camera_up);
        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(viewMat));

        //
        move.z = 10 + 10 * glm::sin(glfwGetTime());
        glm::vec3 pos1 = glm::vec3(-6.0 + move.x, 0.0f + move.y, 0.0 - move.z);

        move.z = 10 - 15 * glm::sin(glfwGetTime());
        glm::vec3 pos2 = glm::vec3(2.0 + move.x, 0.0f + move.y, 0.0 - move.z);

        glm::vec3 pos3 =glm::vec3(8.0 + move2.x, 0.0f + move2.y, 0.0 - move2.z);
        
        
        glm::vec3 scale1(1.0f, 7.0f, 25.0f);
        glm::vec3 scale2(3.0f, 3.0f, 3.0f);

        if (checkCollision(pos3, scale2, pos1, scale1)) {
            std::cout << "Collision with wall 2" << std::endl;
            exit(0);
        }
        if (checkCollision(pos3, scale2, pos2, scale1)) {
            std::cout << "Collision with wall 1" << std::endl;
            exit(0);
        }


       //œciana

		glm::vec3 positions[2] = {
			glm::vec3(-5.0f, 0.0f, 0.0f),
			glm::vec3(5.0f, 0.0f, 0.0f)
		};


        for(int i = 0; i < 2;i++){
			glm::mat4 modelMat2 = glm::translate(glm::mat4(1.0f), positions[i]);
			modelMat2 = glm::scale(modelMat2, glm::vec3(1.0f, 7.0f, 25.0f));
			glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(modelMat2));

			glUniform3f(5, camera_position.x, camera_position.y, camera_position.z);  // view position

			glUniform3f(glGetUniformLocation(shaderProgram, "object_mat.ambient"), 0.3f, 0.3f, 0.3f);  // object ambient
			glUniform3f(glGetUniformLocation(shaderProgram, "object_mat.diffuse"), 1.0f, 0.0f, 0.0f);  // object diffuse
			glUniform3f(glGetUniformLocation(shaderProgram, "object_mat.specular"), 1.0f, 1.0f, 1.0f);  // object specular
			glUniform1f(glGetUniformLocation(shaderProgram, "object_mat.shininess"), 50);  // object shininess

			glUseProgram(shaderProgram);
			glBindVertexArray(VertexArrayId);
			glDrawArrays(GL_TRIANGLES, 0, 36);
        }
  
		      
        //kostka

        glm::mat4 modelMat3 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0 + move2.x, 0.0f + move2.y, 0.0 - move2.z));
        modelMat3 = glm::scale(modelMat3, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(modelMat3));

        glUniform3f(5, camera_position.x, camera_position.y, camera_position.z);  // view position

        glUniform3f(glGetUniformLocation(shaderProgram, "object_mat.ambient"), 0.3f, 0.3f, 0.3f);  // object ambient
        glUniform3f(glGetUniformLocation(shaderProgram, "object_mat.diffuse"), 1.0f, 0.0f, 0.0f);  // object diffuse
        glUniform3f(glGetUniformLocation(shaderProgram, "object_mat.specular"), 0.0f, 1.0f, 1.0f);  // object specular
        glUniform1f(glGetUniformLocation(shaderProgram, "object_mat.shininess"), 50);  // object shininess

        glUseProgram(shaderProgram);
        glBindVertexArray(VertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Ÿród³o œwiat³a

        glUniform3f(glGetUniformLocation(shaderProgram, "light.position"), aLightPos.x, aLightPos.y, aLightPos.z); //light position
        glUniform3f(glGetUniformLocation(shaderProgram, "light.ambient"), 0.3f, 0.3f, 0.3f);  // light ambient
        glUniform3f(glGetUniformLocation(shaderProgram, "light.diffuse"), 0.7f, 0.7f, 0.7f);  // light diffuse
        glUniform3f(glGetUniformLocation(shaderProgram, "light.specular"), 0.6f, 0.3f, 0.3f);  // light specular

        glUseProgram(shaderProgramLight);
        glBindVertexArray(VertexArrayId);

        modelMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
        viewMat = glm::translate(glm::mat4(1.0f), aLightPos);

        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(modelMat));
        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(projMat));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VertexArrayId);
    glDeleteBuffers(1, &VertexBufferId);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

//ruch 
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        move2.x -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        move2.x += 0.1f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}