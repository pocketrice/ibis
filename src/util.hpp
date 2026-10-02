#ifndef IBIS_UTIL_HPP
#define IBIS_UTIL_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

/*
 * Logging macros
 */
#define log_err(msg, ...) fprintf(stderr, "[!] " msg "\n", ## __VA_ARGS__)   ///< Logs an error to standard error.
#define log_warn(msg, ...) fprintf(stderr, "[?] " msg "\n", ## __VA_ARGS__)  ///< Logs a warning to standard error.
#define log_info(msg, ...) fprintf(stdout, "[*] " msg "\n", ## __VA_ARGS__)  ///< Logs an info to standard output.

/*
 * Promises
 */
#define promise_make(msg, ...) fprintf(stdout, msg " ... ", ## __VA_ARGS__)
#define promise_fulfill(status) fprintf(stdout, (status) ? "OK\n" : "FAIL\n")

/**
 * Tries to load the file at given filepath into a string.
 * @param str string to store file contents in
 * @param filepath path of file to load
 * @return whether loading was successful
 */
inline bool load_file(std::string& str, const std::filesystem::path& filepath) noexcept {
	std::ifstream file { filepath };

	try {
		std::string line;
		while (getline(file, line)) {
			str.append(line);
			str.push_back('\n');
		}

		file.close();
	} catch (const std::exception& _) {
		log_err("Could not open file %s", filepath.c_str());
		return false;
	}

	return true;
}

/**
 * Tries to compile the given typed OpenGL shader, returning the pointer value (invalid if compilation fails).
 * @param filepath path to shader file
 * @param shaderType OpenGL shader type (fragment, vertex, compute)
 * @return OpenGL pointer, valid if compilation passes
 */
inline GLuint compile_shader(const std::filesystem::path& filepath, const GLenum shaderType) noexcept {
	std::string shaderStr;
	try {
		load_file(shaderStr, filepath);
	} catch (const std::exception& _) {
		log_warn("Bad shader filepath");
		return 0;
	}
	const char *shaderSource = shaderStr.c_str();

	const GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);

	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	char buffer[512];
	glGetShaderInfoLog(shader, 512, NULL, buffer);
	std::cout << buffer;

	if (status == GL_FALSE) {
		log_warn("Shader %s failed compiling :c", filepath.filename().c_str());
	} else {
		log_info("Shader %s compiled!!", filepath.filename().c_str());
	}

	return shader;
}

/**
 * Converts HSL to RGB.
 * @param h hue value [0.0, 1.0]
 * @param s saturation value [0.0, 1.0]
 * @param l luminosity value [0.0, 1.0]
 * @return converted RGB value
 */
inline std::tuple<float, float, float> hsl2rgb(const float h, const float s, const float l) {
	const float hd = h * 360.0; // map to degrees (1.0 -> 360.0)

	const float c = (1 - std::fabs(2 * l - 1)) * s;
	const float x = c * (1 - std::fabs(std::fmod(hd / 60.0, 2) - 1));
	const float m = l - c / 2.0;

	// 0 is [120-240], [240-360], [0-120].
	// C is [300-60], [60-180], [180-300].
	// X is [60-120 & 240-300], [0-60 & 180-240], [120-180 & 300-360].

	const float r = x * (hd >= 60 && hd < 120 || hd >= 240 && hd < 300) + c * (hd >= 0 && hd < 60 || hd >= 300 && hd < 360) + m;
	const float g = x * (hd >= 0 && hd < 60 || hd >= 180 && hd < 240) + c * (hd >= 60 && hd < 180) + m;
	const float b = x * (hd >= 120 && hd < 180 || hd >= 300 && hd < 360) + c * (hd >= 180 && hd < 300) + m;

	return { r, g, b };
}

#endif //IBIS_UTIL_HPP