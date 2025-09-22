#include "config.h"

#include "header/object.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera_t{
    glm::vec3 position;
    glm::vec3 lookat;
    glm::vec3 up;
    float rotationY;
};

struct Model_t{
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    Object *object;
};

const int INITIAL_SCR_WIDTH = 800;
const int INITIAL_SCR_HEIGHT = 600;

int SCR_WIDTH = INITIAL_SCR_WIDTH;
int SCR_HEIGHT = INITIAL_SCR_HEIGHT;


std::string objDir = "..\\src\\asset\\";
std::string shaderDir = "..\\src\\Shader\\";

unsigned int VAO, VBO;
Model_t Dino;
glm::mat4 dinoModel(1.0f);

// std::filesystem::path cwd = std::filesystem::current_path();
unsigned int vertexShader, fragmentShader, shaderProgram;

glm::mat4 model(1.0f);
glm::mat4 cameraModel(1.0f);
Camera_t camera1;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

std::string GetShaderSource(const std::string &Shaderpath);
void update();
void setup();
void render();


int main(){
    
    //init
    if (!glfwInit()){
        std::cout << "GLFW is not working";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);



    GLFWwindow *window = glfwCreateWindow(800,600, "window", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);


    // dinoObject = new Object(objDir + "Dino.obj");
    // glGenBuffers(1, &VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(Testvertices), Testvertices, GL_STATIC_DRAW);

    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*(sizeof(float)), (void*)(0*sizeof(float)));
    // glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*(sizeof(float)), (void*)(3*sizeof(float)));
    // glEnableVertexAttribArray(1);


    //load and send the shader out
    const char* vShaderSource = GetShaderSource(shaderDir + "default.vert").c_str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSource, NULL);
    glCompileShader(vertexShader);

    const char* fShaderSource = GetShaderSource(shaderDir + "default.frag").c_str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    setup();

    while(!glfwWindowShouldClose(window)){

        
        update();
        render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

//check all the input 
//and this one is the default setting for the openGL for getting the key action
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int modes){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        camera1.rotationY -=15;   
        dinoModel = glm::rotate(dinoModel, glm::radians(-15.0f), glm::vec3(0.0f,0.0f,1.0f));
        std::cout << "camera1 ratation :" << camera1.rotationY << "\n";

    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        camera1.rotationY +=15;
        dinoModel = glm::rotate(dinoModel, glm::radians(15.0f), glm::vec3(0.0f,0.0f,1.0f));
        std::cout << "camera1 ratation :" << camera1.rotationY << "\n";
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        camera1.position += glm::vec3(0.0f, 0.0f, -0.5f);
    }
    if (key ==GLFW_KEY_S && action == GLFW_PRESS){
        camera1.position += glm::vec3(0.0f, 0.0f, 0.5f);
    }
    if (action = GLFW_PRESS){
        std::cout << "camera1 positoin:" << camera1.position.x << "\t"<<camera1.position.y << "\t"<<camera1.position.z<< "\t" << "camera1 rotation y: "<<camera1.rotationY <<std::endl;
        std::cout << "cameraModel: "<< cameraModel[3].x <<"\t"<<cameraModel[3].y <<"\t"<<cameraModel[3].z << "\n"<<std::endl;
    }
}

std::string GetShaderSource(const std::string &Shaderpath){
    
    //vertex shader
    std::ifstream fs(Shaderpath);
    std::stringstream ss;
    std::string s;
    while (getline(fs, s)) {
        ss << s << "\n";
    }
    std::string temp = ss.str();
    const char *source = temp.c_str();

    return temp;

}

void update(){
    cameraModel = glm::mat4(1.0f);
    cameraModel = glm::translate(cameraModel, camera1.position);
    cameraModel = glm::rotate(cameraModel, glm::radians(camera1.rotationY), glm::vec3(0.0f,1.0f,0.0f));

    glm::vec4 pos4 = cameraModel[3];
    //glm::vec3 position = glm::vec3(pos4) / pos4.w;  // Divide by w to get normalized 3D position
    glm::mat4 view = glm::lookAt(glm::vec3(pos4), camera1.lookat, camera1.up);
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(dinoModel));
}

void setup(){
    camera1.position = glm::vec3(0.0f, 1.0f, 7.0f);
    camera1.lookat = glm::vec3(0.0f);
    camera1.up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera1.rotationY = 0.0f;

    dinoModel = glm::mat4(1.0f);
    Dino.position = glm::vec3(0.0f);
    Dino.scale = glm::vec3(0.01f);
    Dino.rotation = glm::vec3(0.0f);
    dinoModel = glm::translate(dinoModel, Dino.position);
    dinoModel = glm::scale(dinoModel, Dino.scale);
    dinoModel = glm::rotate(dinoModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Dino.object = nullptr;
    Dino.object = new Object(objDir + "Dino_2.obj");
    Dino.object -> load_to_buffer();



}

void render(){
    glClearColor(.8f, .8f, .8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glDrawArrays(GL_TRIANGLES, 0, 1000);

    Dino.object -> render();


}

