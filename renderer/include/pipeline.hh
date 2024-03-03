#pragma once

#include <map>
#include <string>
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>

class ShaderModule {
public:
    enum class Type { NONE = 0, VERTEX = 1, FRAGMENT = 2, COMPUTE = 3 };
    static const std::map<Type, GLenum> shaderTypeToGLenum;
    ShaderModule(const std::string& path, Type type);
    ShaderModule(const ShaderModule& other);
    ShaderModule& operator=(const ShaderModule& other);
    ~ShaderModule();
    const std::string& getPath() const {
        return _path;
    }
    Type getType() const {
        return _type;
    }
    GLuint getId() const {
        return _id;
    }

private:
    std::string _path;
    Type _type;
    GLuint _id;
    void init();
};

class Pipeline {
public:
    Pipeline(std::shared_ptr<const ShaderModule> pVertShader,
             std::shared_ptr<const ShaderModule> pFragShader);
    Pipeline(const Pipeline& other);
    Pipeline& operator=(const Pipeline& other);
    ~Pipeline();
    const std::shared_ptr<const ShaderModule> getPVertShader() const {
        return _pVertShader;
    }
    const std::shared_ptr<const ShaderModule> getPFragShader() const {
        return _pFragShader;
    }
    GLuint getId() const {
        return _id;
    }

private:
    std::shared_ptr<const ShaderModule> _pVertShader;
    std::shared_ptr<const ShaderModule> _pFragShader;
    GLuint _id;
    void init();
};