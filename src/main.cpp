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

	constexpr float verts_cube[] = {
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

	constexpr float verts_quad[] = {
	//      X     Y     U     V
		 -1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f, 1.0f, 1.0f,
	      1.0f, -1.0f, 1.0f, 0.0f,

		  1.0f, -1.0f, 1.0f, 0.0f,
		 -1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0f,  1.0f, 0.0f, 1.0f
	};


	constexpr GLuint elems[] = {
		0, 1, 3,
		1, 2, 3
	};


	// -----------------------------------------------------
	//					compile shaders
	// -----------------------------------------------------

	const GLuint sceneVertShader = compile_shader("../shaders/scene.vert", GL_VERTEX_SHADER);
	const GLuint sceneFragShader = compile_shader("../shaders/scene.frag", GL_FRAGMENT_SHADER);
	const GLuint screenVertShader = compile_shader("../shaders/screen.vert", GL_VERTEX_SHADER);
	const GLuint screenFragShader = compile_shader("../shaders/screen.frag", GL_FRAGMENT_SHADER);

	const GLuint sceneShaderProgram = generate_shader_program({sceneVertShader, sceneFragShader});
	const GLuint screenShaderProgram = generate_shader_program({screenVertShader, screenFragShader});


	// -----------------------------------------------------
	//				buffers and attrs
	// -----------------------------------------------------

	const GLuint ebo = generate_ebo(elems, sizeof(elems));

	// =============
	//     scene
	// =============
	const GLuint vao_cube = generate_vao();
	const GLuint vbo_cube = generate_vbo(verts_cube, sizeof(verts_cube));

	glBindVertexArray(vao_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);

	const GLint posSceneAttrib = glGetAttribLocation(sceneShaderProgram, "position");
	glEnableVertexAttribArray(posSceneAttrib);
	glVertexAttribPointer(posSceneAttrib, 3, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), 0);

	const GLint colorSceneAttrib = glGetAttribLocation(sceneShaderProgram, "color");
	glEnableVertexAttribArray(colorSceneAttrib);
	glVertexAttribPointer(colorSceneAttrib, 4, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (void*)(3 * sizeof(float)));

	const GLint texSceneAttrib = glGetAttribLocation(sceneShaderProgram, "texcoord");
	glEnableVertexAttribArray(texSceneAttrib);
	glVertexAttribPointer(texSceneAttrib, 2, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (void*)(7 * sizeof(float)));

	// =============
	//     screen
	// =============
	const GLuint vao_quad = generate_vao();
	const GLuint vbo_quad = generate_vbo(verts_quad, sizeof(verts_quad));

	glBindVertexArray(vao_quad);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);

	const GLint posScreenAttrib = glGetAttribLocation(screenShaderProgram, "position");
	glEnableVertexAttribArray(posScreenAttrib);
	glVertexAttribPointer(posScreenAttrib, 2, GL_FLOAT, GL_FALSE,
		4 * sizeof(float), 0);

	const GLint texScreenAttrib = glGetAttribLocation(screenShaderProgram, "texcoord");
	glEnableVertexAttribArray(texScreenAttrib);
	glVertexAttribPointer(texScreenAttrib, 2, GL_FLOAT, GL_FALSE,
		4 * sizeof(float), (void*)(2 * sizeof(float)));


	// -----------------------------------------------------
	//				photographics!
	// -----------------------------------------------------

	const GLuint tex_photo = load_image(argv[1]);
	log_info("loading a texture (%s)...", std::filesystem::path{ argv[1] }.filename().c_str());

	glUseProgram(sceneShaderProgram);
	glUniform1i(glGetUniformLocation(sceneShaderProgram, "texPhoto"), 0);

	glUseProgram(screenShaderProgram);
	glUniform1i(glGetUniformLocation(screenShaderProgram, "texBuffer"), 0);


	// -----------------------------------------------------
	//				 transforms!
	// -----------------------------------------------------

	glm::mat4 view = glm::lookAt(
		glm::vec3(1.0f, 3.0, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	glm::mat4 proj = glm::perspective(glm::radians(40.0f), static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 10.0f);

	glm::mat4 model_box = {};
	glm::mat4 model_floor = {};

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

	glUseProgram(sceneShaderProgram);

	const GLint uniTime = glGetUniformLocation(sceneShaderProgram, "time");
	const GLint uniTint = glGetUniformLocation(sceneShaderProgram, "tint");

	const GLint uniView = glGetUniformLocation(sceneShaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	const GLint uniProj = glGetUniformLocation(sceneShaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	const GLint uniModel = glGetUniformLocation(sceneShaderProgram, "model");

	// -----------------------------------------------------
	//			      misc vars
	// -----------------------------------------------------

	uint32_t fps = 0;
	float fps_stamp = glfwGetTime() + FPS_TIME;
	float x_flip_timer = 0.0;

	// -----------------------------------------------------
	//			     moar framebuffers
	// -----------------------------------------------------

	GLuint frame_buffer;
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

	GLuint tex_buffer;
	glGenTextures(1, &tex_buffer);
	glBindTexture(GL_TEXTURE_2D, tex_buffer);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_buffer, 0
	);

	GLuint rbo_depth_stencil;
	glGenRenderbuffers(1, &rbo_depth_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);

	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_stencil
	);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		log_err("bad framebuffer");
		exit(-1);
	}

	// -----------------------------------------------------
	//					draw it!
	// -----------------------------------------------------

	glClearColor(1.0, 1.0, 1.0, 1.0);

	while (!glfwWindowShouldClose(window)) {
		const float time = glfwGetTime();

		x_flip_timer = std::max((x_flip && x_flip_timer == 0.0f) + x_flip_timer - X_FLIP_DELTA, 0.0f);
		x_flip &= (x_flip_timer > 0.0f);

		// =========================
		//      draw scene
		// =========================
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		glBindVertexArray(vao_cube);
		glEnable(GL_DEPTH_TEST);
		glUseProgram(sceneShaderProgram);
		swap_texture(tex_photo, GL_TEXTURE0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		// =========================
		//      draw screen
		// =========================

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(vao_quad);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(screenShaderProgram);

		swap_texture(tex_buffer, GL_TEXTURE0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

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
