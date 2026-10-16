#ifndef IBIS_GLOBALS_H
#define IBIS_GLOBALS_H

/*
 * Constants
 */
#define HSL_STEP 0.0001f
#define FPS_TIME 3.0f
#define X_FLIP_DELTA 0.015f
#define DEFAULT_TEXTURE "../assets/minc.jpg"

#include <filesystem>
#include <regex>

#include "glad/gl.h"

/*
 * X-flip animation
 */
inline bool x_flip = false;

/*
 * Texture defaults
 */
inline std::vector<std::filesystem::path> catalogue = {};
inline bool is_catalogue = false;
inline size_t ptr_catalogue = 0;
inline auto re_catalogue_exts = std::regex { "\\.(jpg|jpeg|png)" };

/*
 * WIP
 */
inline GLuint shaderProgram;
inline GLuint textures[1];

#endif //IBIS_GLOBALS_H
