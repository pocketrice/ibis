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
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS || action == GLFW_REPEAT) {
		x_flip = true;
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
	//    X      Y     Z      R     G     B      A     U     V
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // cube
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // floor
		 1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
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

	GLuint textures[1];
	glGenTextures(1, textures);

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

	const std::filesystem::path texPath = argv[1];

	log_info("loading a texture (%s)...", texPath.filename().c_str());
	load_image(texPath, "tex", shaderProgram, textures, GL_TEXTURE0, 0);


	// -----------------------------------------------------
	//				 transforms!
	// -----------------------------------------------------

	glm::mat4 view = glm::lookAt(
		glm::vec3(1.0f, 3.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	glm::mat4 proj = glm::perspective(glm::radians(40.0f), 800.0f / 600.0f, 1.0f, 10.0f);

	glm::mat4 model_box = {};
	glm::mat4 model_floor = {};

	// -----------------------------------------------------
	//			        link attrs!
	// -----------------------------------------------------

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), 0);

	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (void*)(3 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (void*)(7 * sizeof(float)));

	// -----------------------------------------------------
	//			   depth and stencil buffers
	// -----------------------------------------------------

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);

	// -----------------------------------------------------
	//			   transparency
	// -----------------------------------------------------

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// -----------------------------------------------------
	//			      uniforms
	// -----------------------------------------------------

	const GLint uniTime = glGetUniformLocation(shaderProgram, "time");
	const GLint uniTint = glGetUniformLocation(shaderProgram, "tint");

	const GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	const GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	const GLint uniModel = glGetUniformLocation(shaderProgram, "model");

	// -----------------------------------------------------
	//			      misc vars
	// -----------------------------------------------------

	uint32_t fps = 0;
	float fps_stamp = glfwGetTime() + FPS_TIME;
	float x_flip_timer = 0.0;

	// -----------------------------------------------------
	//					draw it!
	// -----------------------------------------------------

	glClearColor(1.0, 1.0, 1.0, 1.0);

	while (!glfwWindowShouldClose(window)) {
		const float time = glfwGetTime();

		x_flip_timer = std::max((x_flip && x_flip_timer == 0.0f) + x_flip_timer - X_FLIP_DELTA, 0.0f);
		x_flip &= (x_flip_timer > 0.0f);

		model_floor = glm::mat4(1.0);
		model_floor = glm::rotate(model_floor, time * glm::radians(80.0f), glm::vec3(0.0, 0.0, 1.0));
		model_floor = glm::rotate(model_floor, (1.0f - x_flip_timer) * glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));

		model_box = model_floor;
		model_box = glm::translate(model_box, glm::vec3(0.0, 0.0, 0.3 + (sin(3.0 * time) + 0.5) / 5.0));
		glUniform1f(uniTime, time);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model_box));

		/*
		 * draw regular box
		 */
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*
		 * draw floor (do not occlude reflection)
		 */
		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);					    // ┓
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	 	    // ┃     use only depth buffer of first box (so reflected box isn't blocked).
		glStencilMask(0xFF);										 	    // ┣━━   flag all stencils that pass depth test (not blocked by first box, i.e. floor) with 1
		glDepthMask(GL_FALSE); // don't write to depth buffer			    // ┃     use this stencil mask (i.e. bounds of floor) to render the reflected box, turning on depth buffer for that box itself
		glClear(GL_STENCIL_BUFFER_BIT); // clear stencil buffer to 0    // ┛

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model_floor)); // null transforms for floor
		glDrawArrays(GL_TRIANGLES, 36, 6);

		/*
		 * draw reflection (draw within floor stencil)
		 */
		glStencilFunc(GL_EQUAL, 1, 0xFF); // pass test if stencil == 1
		glStencilMask(0x00); // don't write to stencil buffer
		glDepthMask(GL_TRUE); // write to depth buffer

		model_box = glm::translate(model_floor, glm::vec3(0.0, 0.0, -1.0));
		model_box = glm::rotate(model_box, time, glm::vec3(time, time, time));
		model_box = glm::scale(model_box, glm::vec3(1.1));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model_box));

		glUniform3f(uniTint, 0.35f, 0.35f, 0.35f);  // override with darker mix color
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(uniTint, 1.0f, 1.0f, 1.0f);

		glDisable(GL_STENCIL_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
