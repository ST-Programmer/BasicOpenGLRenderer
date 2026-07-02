#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef DEBUG

#define GL_CALL(x)                                                                                 \
    do                                                                                             \
    {                                                                                              \
        x;                                                                                         \
        GLenum err;                                                                                \
        while ((err = glGetError()) != GL_NO_ERROR)                                                \
        {                                                                                          \
            const char* errStr;                                                                    \
            switch (err)                                                                           \
            {                                                                                      \
                case GL_INVALID_ENUM:                                                              \
                    errStr = "INVALID_ENUM";                                                       \
                    break;                                                                         \
                case GL_INVALID_VALUE:                                                             \
                    errStr = "INVALID_VALUE";                                                      \
                    break;                                                                         \
                case GL_INVALID_OPERATION:                                                         \
                    errStr = "INVALID_OPERATION";                                                  \
                    break;                                                                         \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                                             \
                    errStr = "INVALID_FRAMEBUFFER_OPERATION";                                      \
                    break;                                                                         \
                case GL_OUT_OF_MEMORY:                                                             \
                    errStr = "OUT_OF_MEMORY";                                                      \
                    break;                                                                         \
                default:                                                                           \
                    errStr = "UNKNOWN";                                                            \
                    break;                                                                         \
            }                                                                                      \
            std::cerr << "[GL Error] " << errStr << " (" << err << ") in " #x " at " << __FILE__   \
                      << ":" << __LINE__ << std::endl;                                             \
        }                                                                                          \
    } while (0)

#else
#define GL_CALL(x) x
#endif

inline std::string ReadFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << ("Failed to Open File: " + path) << std::endl;
        return "";
    }

    std::stringstream buffer;

    buffer << file.rdbuf();
    std::string contents = buffer.str();

    return contents;
}

#define ERR(x) (std::cerr << x << std::endl)
#define NORMALIZERGBA(r, g, b, a) r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f
#define NORMALIZERGB(r, g, b) r / 255.0f, g / 255.0f, b / 255.0f
#define GLMNORMALIZERGBA(r, g, b, a) glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f)
#define GLMNORMALIZERGB(r, g, b) glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f)
