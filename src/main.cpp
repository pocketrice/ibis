#define GLFW_INCLUDE_NONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <AL/al.h>
#include "glad/gl.h"
#include "SOIL.h"

#include "util.hpp"

#define HSL_STEP 0.0001
#define FPS_DELTA 3.0


void error_callback(int error, const char* description) {
	fprintf(stderr, "err: %s\n", description);
}

void close_callback(GLFWwindow* window) {
	log_info("wrap it up!");
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
		log_err("no glfw!");
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
		log_err("oups");
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

	log_info("amazing!");

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//					ok to shade!
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	constexpr float verts[] = {
	//     Position         Color          Texcoords
		-0.5f,  0.5f, 0.75f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, 0.50f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.25f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
	};


	constexpr GLuint elems[] = {
		0, 1, 3,
		1, 2, 3
	};

	// -----------------------------------------------------
	//				VAOs, VBOs, EBOs, texs
	// -----------------------------------------------------

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(verts), verts, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(elems), elems, GL_STATIC_DRAW);

	GLuint textures[2];
	glGenTextures(2, textures);

	// -----------------------------------------------------
	//					compile shaders
	// -----------------------------------------------------

	GLuint vertShader = compile_shader("../shaders/basic.vert", GL_VERTEX_SHADER);
	GLuint fragShader = compile_shader("../shaders/basic.frag", GL_FRAGMENT_SHADER);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// -----------------------------------------------------
	//				photographics!
	// -----------------------------------------------------

	load_image("../assets/vistapolis.jpg", "texCity", shaderProgram, textures, GL_TEXTURE0, 0);
	load_image("../assets/verbatiny.jpg", "texCD", shaderProgram, textures, GL_TEXTURE2, 2);

	// -----------------------------------------------------
	//				 transforms!
	// -----------------------------------------------------

	auto trans = glm::mat4(1.0f);
	const GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");

	// -----------------------------------------------------
	//			        link attrs!
	// -----------------------------------------------------

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
		7 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		7 * sizeof(float), (void*)(2 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
		7 * sizeof(float), (void*)(5 * sizeof(float)));


	// -----------------------------------------------------
	//			   uniforms n stuffs
	// -----------------------------------------------------

	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
	float h = 0.0;
	const float s = 1.0;
	const float l = 0.75;


	// -----------------------------------------------------
	//					draw it!
	// -----------------------------------------------------

	const GLint uniTime = glGetUniformLocation(shaderProgram, "time");
	glClearColor(1.0, 1.0, 1.0, 1.0);


	uint32_t fps = 0;
	float fps_stamp = glfwGetTime() + FPS_DELTA;

	trans = glm::scale(trans, glm::vec3(1.3, 1.7, 1.3));

	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();

		trans = glm::rotate(trans, time * glm::radians(0.2f), glm::vec3(time, time, time));
		glUniform1f(uniTime, time);
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		++fps;
		if (time >= fps_stamp) {
			log_warn("fps: %.0f", fps / FPS_DELTA);
			fps = 0.0;
			fps_stamp = time + FPS_DELTA;
		}
	}

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//					done shading!
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



	glfwDestroyWindow(window);

	log_info("finish'd!");

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
