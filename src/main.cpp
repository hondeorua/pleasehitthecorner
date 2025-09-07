#include "glm/fwd.hpp"
#include "shader.hpp" //this header file already has the other #include
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>
#include <stdexcept>

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
const char *VERTEX_SHADER_PATH = "../src/shaders/shader.vert";
const char *FRAGMENT_SHADER_PATH = "../src/shaders/shader.frag";
float visibility_of_face = 0.2;

GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Shader shader);
glm::vec3 normal_vect_wall(glm::vec3 position);
bool hit_wall(glm::vec3 position);

int main() {

  std::cout << "program's running ech ech..." << std::endl;
  stbi_set_flip_vertically_on_load_thread(true);

  if (!glfwInit()) {
    std::cout << "no gud glfw not inited gudly" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "mommiii", NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cout << "no gud window not created" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  unsigned int VAO, VBO, EBO, Texture1, Texture2;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glGenTextures(1, &Texture1);
  glBindTexture(GL_TEXTURE_2D, Texture1);
  int width, height, nrChannels;
  unsigned char *data = stbi_load("../src/resources/bouncing_dvd_logo.jpg", &width,
                                  &height, &nrChannels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else
    throw std::runtime_error("Failed to load image");

  stbi_image_free(data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  // glGenTextures(1, &Texture2);
  // glBindTexture(GL_TEXTURE_2D, Texture2);
  // stbi_set_flip_vertically_on_load(true);
  // data = stbi_load("../src/resources/hahahihi.png", &width, &height,
  //                  &nrChannels, 0);
  //
  // if (data) {
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
  //                GL_UNSIGNED_BYTE, data);
  //   glGenerateMipmap(GL_TEXTURE_2D);
  // } else
  //   throw std::runtime_error("Failed to load image");
  // stbi_image_free(data);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  shader.use();

  // get location of aTrans uniform
  unsigned int transLoc = glGetUniformLocation(shader.shaderID, "aTrans");

  shader.setInt("Texture1", 0);
  shader.setInt("Texture2", 1);

  shader.setFloat("visibility_of_face", 0.2);

  float last_time = glfwGetTime();

  glm::vec3 position = glm::vec3(0, 0, 0);
  glm::vec3 velocity = glm::vec3(0.7, 0.8, 0.0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window, shader);

    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;
    
    if (hit_wall(position)){
      velocity = glm::reflect(velocity, normal_vect_wall(position));
    }

    position += velocity * delta_time;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 trans = glm::mat4(1.0);
    trans = glm::translate(trans, position);
    trans = glm::scale(trans, glm::vec3(0.7, 0.7, 0.7));

    shader.use();
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Texture2);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width * 2, height * 2);
}

void processInput(GLFWwindow *window, Shader shader) {
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    visibility_of_face -= 0.005;
    shader.setFloat("visibility_of_face", visibility_of_face);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    visibility_of_face += 0.005;
    shader.setFloat("visibility_of_face", visibility_of_face);
  }
}

glm::vec3 normal_vect_wall(glm::vec3 position) {
  float x = position.x;
  float y = position.y;
  if (std::abs(x) > std::abs(y)) {
    if (x > 0)
      return glm::vec3(-1, 0, 0);
    if (x < 0)
      return glm::vec3(1, 0, 0);
  }
  if (y > 0)
    return glm::vec3(0, -1, 0);
  if (y < 0)
    return glm::vec3(0, 1, 0);
  return glm::vec3(-x/std::abs(x), -y/std::abs(y), 0);
}

bool hit_wall(glm::vec3 position){
  float x = position.x;
  float y = position.y;
  if (std::abs(x) >= 0.65 || std::abs(y) >= 0.65) return true;
  return false;
}
