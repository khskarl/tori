#ifndef TYPES_HPP
#define TYPES_HPP

#include <glm/glm.hpp>
#include <string>
#include <cstdint>

// OpenGL shader defines
#define GL_VERTEX_SHADER          0x8B31
#define GL_FRAGMENT_SHADER        0x8B30
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER    0x8E88
#define GL_GEOMETRY_SHADER        0x8DD9
#define GL_COMPUTE_SHADER         0x91B9

// OpenGL Texture defines
#define GL_TEXTURE_1D       0x0DE0
#define GL_TEXTURE_2D       0x0DE1
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_NEAREST 0x2600
#define GL_LINEAR  0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST  0x2701
#define GL_NEAREST_MIPMAP_LINEAR  0x2702
#define GL_LINEAR_MIPMAP_LINEAR   0x2703
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_REPEAT          0x2901
#define GL_MIRRORED_REPEAT 0x8370
#define GL_CLAMP_TO_EDGE   0x812F
#define GL_RED  0x1903
#define GL_RG   0x8227
#define GL_RGB  0x1907
#define GL_RGBA 0x1908
#define GL_DEPTH_COMPONENT 0x1902
#define GL_STENCIL_INDEX  0x1901

#define GL_BYTE           0x1400
#define GL_UNSIGNED_BYTE  0x1401
#define GL_SHORT          0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT            0x1404
#define GL_UNSIGNED_INT   0x1405
#define GL_FLOAT          0x1406

#endif // TYPES_HPP
