#define GLFW_INCLUDE_NONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <AL/al.h>

#include "globals.h"
#include "glad/gl.h"
#include "SOIL.h"

#include "util.hpp"


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

	if (argc != 2 || !std::filesystem::exists(argv[1]) || !std::filesystem::is_regular_file(argv[1])) {
		std::cerr << "./ibis [texture]" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ibis", nullptr, nullptr);
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

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	log_info("ok to go..!");

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//					ok to shade!
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	constexpr float pts[] = {
		-0.45f,  0.45f,
		 0.45f,  0.45f,
		 0.45f, -0.45f,
		-0.45f, -0.45f,
	};

	// -----------------------------------------------------
	//					compile shaders
	// -----------------------------------------------------

	const GLuint sceneVertShader = compile_shader("../shaders/geo.vert", GL_VERTEX_SHADER);
	const GLuint sceneFragShader = compile_shader("../shaders/geo.frag", GL_FRAGMENT_SHADER);
	const GLuint sceneGeomShader = compile_shader("../shaders/geo.geom", GL_GEOMETRY_SHADER);

	const GLuint sceneShaderProgram = generate_shader_program({sceneVertShader, sceneFragShader, sceneGeomShader});
	glUseProgram(sceneShaderProgram);

	// -----------------------------------------------------
	//				buffers and attrs
	// -----------------------------------------------------

	const GLuint vao = generate_vao();
	const GLuint vbo = generate_vbo(pts, sizeof(pts));

	glBindVertexArray(vao);
	GLint posAttrib = glGetAttribLocation(sceneShaderProgram, "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// -----------------------------------------------------
	//				photographics!
	// -----------------------------------------------------

	const GLuint tex_photo = load_image(argv[1]);
	log_info("loading a texture (%s)...", std::filesystem::path{ argv[1] }.filename().c_str());


	// -----------------------------------------------------
	//				 transforms!
	// -----------------------------------------------------

	glm::mat4 view = glm::lookAt(
		glm::vec3(1.0f, 3.0, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	glm::mat4 proj = glm::perspective(glm::radians(40.0f), static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 10.0f);

	glm::mat4 model = {};


	// -----------------------------------------------------
	//			      uniforms
	// -----------------------------------------------------

	// -----------------------------------------------------
	//			      misc vars
	// -----------------------------------------------------

	uint32_t fps = 0;
	float fps_stamp = glfwGetTime() + FPS_TIME;

	// -----------------------------------------------------
	//					draw it!
	// -----------------------------------------------------

	while (!glfwWindowShouldClose(window)) {
		const float time = glfwGetTime();

		// =========================
		//      draw scene
		// =========================
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_POINTS, 0, 4);

		// =========================
		//     actually render it!
		// =========================

		glfwSwapBuffers(window);
		glfwPollEvents();

		++fps;
		if (time >= fps_stamp) {
			log_warn("fps: %.0f", fps / FPS_TIME);
			fps = 0.0;
			fps_stamp = time + FPS_TIME;
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
