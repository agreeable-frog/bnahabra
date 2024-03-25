#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

class Window {
public:
    static int _glfwInit;
    Window(const std::string& name, size_t width, size_t height);
    Window(const Window& other);
    Window& operator=(const Window& other);
    ~Window();
    GLFWwindow* getHandle() const {
        return _handle;
    }
    std::string getName() const {
        return _name;
    }
    size_t getWidth() const {
        return _width;
    }
    size_t getHeight() const {
        return _height;
    }
    float getRatio() const {
        return (float)_width/(float)_height;
    }
    const std::map<int, bool>& getKeyStates() const {
        return _keyStates;
    }

    void addKeyState(int key, bool state) {
        _keyStates[key] = state;
    }

private:
    GLFWwindow* _handle;
    std::string _name;
    size_t _width;
    size_t _height;
    std::map<int, bool> _keyStates;
    void init();
    static void debugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar* message, const void* userparam);
    static void glfwErrorCallback(int i, const char* errStr);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};