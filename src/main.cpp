#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glad/gl.h"

#include <iostream>


void error_callback(int error, const char* description) {
	fprintf(stderr, "err: %s\n", description);
}

void close_callback(GLFWwindow* window) {
	std::cout << "wrap it up!" << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {

}

void cursor_enter_callback(GLFWwindow* window, int entered) {

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {

}

int main(int argc, char* argv[]) {
	if (!glfwInit()) {
		std::cerr << "no glfw!" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "ibis", nullptr, nullptr);
	if (!window) {
		std::cerr << "oups" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// window and context OK

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetWindowCloseCallback(window, close_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	std::cout << "amazing!" << std::endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window)) {
		double time = glfwGetTime();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	std::cout << "finish!" << std::endl;

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
