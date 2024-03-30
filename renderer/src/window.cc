#include "renderer/window.hh"

#include <stdexcept>
#include <iostream>

int Window::_glfwInit = 0;

Window::Window(const std::string& name, size_t width, size_t height)
    : _name(name), _width(width), _height(height) {
    if (!_glfwInit) {
        glfwInit();
    }
    _glfwInit++;
    init();
}

Window::Window(const Window& other)
    : Window(other.getName(), other.getWidth(), other.getHeight()) {
}

Window& Window::operator=(const Window& other) {
    if (this == &other) {
        return *this;
    }
    glfwDestroyWindow(_handle);
    _name = other.getName();
    _width = other.getWidth();
    _height = other.getHeight();
    init();
    return *this;
}

Window::~Window() {
    if (_handle) glfwDestroyWindow(_handle);
    _glfwInit--;
    if (!_glfwInit) {
        glfwTerminate();
    }
}

void Window::init() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _handle =
        glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetWindowUserPointer(_handle, this);
    glfwMakeContextCurrent(_handle);
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW init failed.");
    }
    glfwSwapInterval(0);
    glfwSetErrorCallback(&Window::glfwErrorCallback);
    glfwSetKeyCallback(_handle, &Window::keyCallback);
    glfwSetMouseButtonCallback(_handle, &Window::mouseButtonCallback);
    glfwSetCursorPosCallback(_handle, &Window::cursorPosCallback);
#ifndef NDEBUG
    glDebugMessageCallback(&Window::debugCallback, 0);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
}

void Window::debugCallback(GLenum source, GLenum type, GLuint id,
                           GLenum severity, GLsizei length,
                           const GLchar* message, const void* userparam) {
    std::cout << "DEBUG : source " << source << ", type " << type << ", id "
              << id << ", severity " << severity << '\n';
    std::cout << std::string(message) << '\n';
}

void Window::glfwErrorCallback(int i, const char* errStr) {
    std::cerr << "ERROR : " << errStr << '\n';
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    void* userPtr = glfwGetWindowUserPointer(window);
    Window* pWindow = (Window*)userPtr;
    if (action == GLFW_PRESS) {
        pWindow->_keyStates[key] = true;
    } else if (action == GLFW_RELEASE) {
        pWindow->_keyStates[key] = false;
    }
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action,
                                 int mods) {
    void* userPtr = glfwGetWindowUserPointer(window);
    Window* pWindow = (Window*)userPtr;
    if (action == GLFW_PRESS) {
        pWindow->_mouseButtonStates[button] = true;
    } else if (action == GLFW_RELEASE) {
        pWindow->_mouseButtonStates[button] = false;
    }
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    void* userPtr = glfwGetWindowUserPointer(window);
    Window* pWindow = (Window*)userPtr;
    static double lastxpos = xpos;
    static double lastypos = ypos;
    pWindow->_cursorMove[0] += lastxpos - xpos;
    pWindow->_cursorMove[1] += lastypos - ypos;
    lastxpos = xpos;
    lastypos = ypos;
}