#include "pipeline.hh"

#include <fstream>
#include <sstream>
#include <iostream>

const std::map<ShaderModule::Type, GLenum> ShaderModule::shaderTypeToGLenum = {
    {ShaderModule::Type::VERTEX, GL_VERTEX_SHADER},
    {ShaderModule::Type::FRAGMENT, GL_FRAGMENT_SHADER},
    {ShaderModule::Type::COMPUTE, GL_COMPUTE_SHADER},
    {ShaderModule::Type::NONE, -1}};

static std::string loadText(const std::string& filename) {
    std::ifstream inputSrcFile(filename, std::ios::in);
    std::stringstream textStream;
    textStream << inputSrcFile.rdbuf();
    return textStream.str();
}

void ShaderModule::init() {
    std::string src = loadText(_path);
    _id = glCreateShader(shaderTypeToGLenum.at(_type));
    char* data = new char[src.size() + 1];
    src.copy(data, src.size() + 1);
    glShaderSource(this->_id, 1, (const GLchar**)&(data), 0);

    GLint compileStatus = GL_TRUE;
    glCompileShader(_id);
    glGetShaderiv(_id, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint logSize;
        glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &logSize);
        char* logData = new char(logSize);
        glGetShaderInfoLog(_id, logSize, &logSize, logData);
        std::cerr << logData << '\n';
        delete logData;
        delete data;
        throw std::runtime_error("Failed to compile : " + _path);
    }

    delete data;
}

ShaderModule::ShaderModule(const std::string& path, Type type) : _path(path), _type(type) {
    init();
}

ShaderModule::ShaderModule(const ShaderModule& other)
    : ShaderModule(other.getPath(), other.getType()) {
}

ShaderModule& ShaderModule::operator=(const ShaderModule& other) {
    if (this == &other) {
        return *this;
    }
    glDeleteShader(_id);
    _path = other.getPath();
    _type = other.getType();
    init();
    return *this;
}

ShaderModule::~ShaderModule() {
    glDeleteShader(_id);
}

void Pipeline::init() {
    _id = glCreateProgram();
    if (_id == 0) throw std::runtime_error("Pipeline creation failed");
    glAttachShader(_id, _pVertShader->getId());
    glAttachShader(_id, _pFragShader->getId());
    glLinkProgram(_id);
    GLint linkStatus = GL_TRUE;
    glGetProgramiv(_id, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLint logSize;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logSize);
        char* logData = new char(logSize);
        if (logData != 0) {
            glGetProgramInfoLog(_id, logSize, &logSize, logData);
            std::cerr << logData << '\n';
            delete logData;
        }
        throw std::runtime_error("Pipeline link failed");
    }
}

Pipeline::Pipeline(std::shared_ptr<const ShaderModule> pVertShader,
                   std::shared_ptr<const ShaderModule> pFragShader)
    : _pVertShader(pVertShader), _pFragShader(pFragShader) {
    init();
}

Pipeline::Pipeline(const Pipeline& other)
    : Pipeline(other.getPVertShader(), other.getPFragShader()) {
}

Pipeline& Pipeline::operator=(const Pipeline& other) {
    if (this == &other) {
        return *this;
    }
    glDeleteProgram(_id);
    _pVertShader = other.getPVertShader();
    _pFragShader = other.getPFragShader();
    init();
    return *this;
}

Pipeline::~Pipeline() {
    glDeleteProgram(_id);
}