
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Model.h"

const int screen_width = 800;
const int screen_height = 600;

GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,

		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
};

glm::vec3 cube_pos_lit[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(-2.0f, -2.0f, 2.0f),
	glm::vec3(-2.0f, 2.0f, 2.0f),
	glm::vec3(2.0f, 2.0f, 2.0f),
	glm::vec3(2.0f, -2.0f, 2.0f),
	glm::vec3(-2.0f, -2.0f, -2.0f),
	glm::vec3(-2.0f, 2.0f, -2.0f),
	glm::vec3(2.0f, 2.0f, -2.0f),
	glm::vec3(2.0f, -2.0f, -2.0f)
};

glm::vec3 light_pos_list[] = {
	glm::vec3(3.0f, 3.0f, 0.0f),
	glm::vec3(-3.0f, -3.0f, 0.0f),
	glm::vec3(3.0f, -3.0f, 0.0f),
	glm::vec3(-3.0f, 3.0f, 0.0f),

	glm::vec3(0.0f, 5.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f)
};

unsigned int indices[] = {
	0,1,2,
	2,3,0,

	4,5,6,
	6,7,4,

	8,9,10,
	10,11,8,

	12,13,14,
	14,15,12,

	16,17,18,
	18,19,16,

	20,21,22,
	22,23,20
};

Camera * camera_main;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow * window);

void key_callback(GLFWwindow * window, int key, int scancode, int actin, int mods);

void mouse_callback(GLFWwindow * window, double xpos, double ypos);

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

int main() 
{
	// 初始化GLFW窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建GLFW窗口
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	camera_main = new Camera(glm::vec3(0.0f, 10.0f, 20.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.01f, 100.0f);

	stbi_set_flip_vertically_on_load(true);

 	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);



	//// 声明一个顶点缓冲对象
	//GLuint VBO;
	//glGenBuffers(1, &VBO);
	//// 设置顶点缓冲对象缓冲区类型
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//// 向缓冲区中写入数据
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// light
	//GLuint VAO_light;
	//glGenVertexArrays(1, &VAO_light);
	//	glBindVertexArray(VAO_light);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	//	glEnableVertexAttribArray(0);
	//glBindVertexArray(0);

	//Shader shader_light = Shader("../LearnOpenGL/res/shader.vs", "../LearnOpenGL/res/shader_light.fs");
	Shader shader_obj = Shader("../LearnOpenGL/res/shader.vs", "../LearnOpenGL/res/shader.fs");	

	Model model = Model("../LearnOpenGL/res/nanosuit/nanosuit.obj");

	while (!glfwWindowShouldClose(window))
	{
		// 检查并调用事件
		glfwPollEvents();
		


		// 处理输入事件 
		processInput(window);

		camera_main->update();

		// 处理渲染指令
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// 观察矩阵
		glm::mat4 view = camera_main->getView();
		// 投影矩阵
		glm::mat4 projection = camera_main->getProjection();
		
		//// 灯光obj
		glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
		//shader_light.use();
		//shader_light.setMatrix4f("view", view);
		//shader_light.setMatrix4f("projection", projection);
		//glm::mat4 trans_light = glm::mat4(1.0f);
		//trans_light = glm::translate(trans_light, light_pos_list[5]);
		//shader_light.setMatrix4f("transform", trans_light);
		//shader_light.setVec3f("lightColor", light_color);
		//glBindVertexArray(VAO_light);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		glm::vec3 direction_light_direction = glm::vec3(-100.0f, -100.0f, -100.0f);
		glm::vec3 light_ambient_color = light_color * glm::vec3(0.2f);
		glm::vec3 light_diffuse_color = light_color * glm::vec3(0.5f);
		glm::vec3 light_specular_color = light_color;

		shader_obj.use();
		shader_obj.setMatrix4f("view", view);
		shader_obj.setMatrix4f("projection", projection);
		glm::mat4 trans_obj = glm::mat4(1.0f);
		//trans_obj = glm::translate(trans_obj, glm::vec3(0.0f, 0.0f, 0.0f));
		//trans_obj = glm::scale(trans_obj, glm::vec3(1.0f, 1.0f, 1.0f));
		//trans_obj = glm::rotate(trans_obj, (float)glfwGetTime() * 0.5f, glm::vec3(1.0f, -0.5f, -1.0f));
		shader_obj.setMatrix4f("transform", trans_obj);
		shader_obj.setVec3f("viewPos", camera_main->getCameraPosition());

		shader_obj.setVec3f("directionLight.direction", direction_light_direction);
		shader_obj.setVec3f("directionLight.ambient", light_ambient_color);
		shader_obj.setVec3f("directionLight.diffuse", light_diffuse_color);
		shader_obj.setVec3f("directionLight.specular", light_specular_color);
		shader_obj.setFloat("material.shininess", 32.0f);

		model.draw(shader_obj);


		// 灯光
		//glm::vec3 light_ambient_color = light_color * glm::vec3(0.2f);
		//glm::vec3 light_diffuse_color = light_color * glm::vec3(0.5f);
		//glm::vec3 light_specular_color = light_color;

		
		//glm::vec3 spot_light_direction = glm::vec3(0.0f, -100.0f, 0.0f);

		//for (int i = 0; i < 9; i++)
		//{
		//	shader_obj->use();
		//	shader_obj->setMatrix4f("view", view);
		//	shader_obj->setMatrix4f("projection", projection);
		//	glm::mat4 trans_obj = glm::mat4(1.0f);
		//	trans_obj = glm::translate(trans_obj, cube_pos_lit[i]);
		//	trans_obj = glm::scale(trans_obj, glm::vec3(1.0f, 1.0f, 1.0f));
		//	trans_obj = glm::rotate(trans_obj, (float)glfwGetTime() * 0.5f, glm::vec3(1.0f, -0.5f, -1.0f));
		//	
		//	shader_obj->setMatrix4f("transform", trans_obj);
		//	shader_obj->setVec3f("viewPos", camera_main->getCameraPosition());
		//	// 材质颜色
		//	//shader_obj->setVec3f("material.ambient", glm::vec3(1.0f, 0.5f, 0.3f));
		//	//shader_obj->setVec3f("material.diffuse", glm::vec3(1.0f, 0.5f, 0.3f));
		//	//shader_obj->setVec3f("material.specular", glm::vec3(0.5f));
		//	// 材质贴图
		//	shader_obj->setInt("material.diffuse", 0);
		//	shader_obj->setInt("material.specular", 1);
		//	shader_obj->setFloat("material.shininess", 32.0f);
		//	
		//	// 平行光
		//	shader_obj->setVec3f("directionLight.direction", direction_light_direction);
		//	shader_obj->setVec3f("directionLight.ambient", light_ambient_color);
		//	shader_obj->setVec3f("directionLight.diffuse", light_diffuse_color);
		//	shader_obj->setVec3f("directionLight.specular", light_specular_color);

		//	// 点光源
		//	for (int j = 0; j < 4; j ++)
		//	{
		//		shader_obj->setVec3f("pointLights[" + std::to_string(j) + "].position", light_pos_list[j]);
		//		shader_obj->setVec3f("pointLights[" + std::to_string(j) + "].ambient", light_ambient_color);
		//		shader_obj->setVec3f("pointLights[" + std::to_string(j) + "].diffuse", light_diffuse_color);
		//		shader_obj->setVec3f("pointLights[" + std::to_string(j) + "].specular", light_specular_color);
		//		shader_obj->setFloat("pointLights[" + std::to_string(j) + "].constant", 1.0f);
		//		shader_obj->setFloat("pointLights[" + std::to_string(j) + "].linear", 0.09f);
		//		shader_obj->setFloat("pointLights[" + std::to_string(j) + "].quadratic", 0.032f);
		//	}

		//	// 聚光灯
		//	shader_obj->setVec3f("spotLight.position", light_pos_list[4]);
		//	shader_obj->setVec3f("spotLight.direction", spot_light_direction);
		//	shader_obj->setVec3f("spotLight.ambient", light_ambient_color);
		//	shader_obj->setVec3f("spotLight.diffuse", light_diffuse_color);
		//	shader_obj->setVec3f("spotLight.specular", light_specular_color);
		//	shader_obj->setFloat("spotLight.cutOff", glm::cos(glm::radians(15.0f)));
		//	shader_obj->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(20.0f)));

		//	glBindVertexArray(VAO_obj);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	glBindVertexArray(0);
		//}

		// 交换缓冲区
		glfwSwapBuffers(window);
	}

	//glDeleteVertexArrays(1, &VAO_light);
	//glDeleteBuffers(1, &VBO);
	//delete &shader_obj;
	//delete &shader_light;

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (nullptr != camera_main)
	{
		camera_main->input_callback(window);
	}
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (nullptr != camera_main)
	{
		camera_main->mouse_callback(window, xpos, ypos);
	}
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) 
{
	if (nullptr != camera_main)
	{
		camera_main->scroll_callback(window, xoffset, yoffset);
	}
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (key == GLFW_KEY_CAPS_LOCK && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
