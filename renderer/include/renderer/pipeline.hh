#pragma once

#include <string>
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>

class ShaderModule {
public:
    enum class Type {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        COMPUTE = GL_COMPUTE_SHADER
    };
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
    void init(const std::string& path, Type type);
};

class Program {
public:
    Program(std::shared_ptr<const ShaderModule> pVertShader,
            std::shared_ptr<const ShaderModule> pFragShader);
    Program(const std::string& vertShaderPath,
            const std::string& fragShaderPath);
    Program(const Program& other);
    Program& operator=(const Program& other);
    ~Program();
    const std::shared_ptr<const ShaderModule>& getPVertShader() const {
        return _pVertShader;
    }
    const std::shared_ptr<const ShaderModule>& getPFragShader() const {
        return _pFragShader;
    }
    GLuint getId() const {
        return _id;
    }
    void bind() const;
    void unbind();

private:
    std::shared_ptr<const ShaderModule> _pVertShader;
    std::shared_ptr<const ShaderModule> _pFragShader;
    GLuint _id;
    void init(std::shared_ptr<const ShaderModule> pVertShader,
              std::shared_ptr<const ShaderModule> pFragShader);
};

class Pipeline {
public:
    Pipeline();
    Pipeline(const Pipeline& other);
    Pipeline& operator=(const Pipeline& other);
    ~Pipeline();
    GLuint getVaoId() const {
        return _vaoId;
    }
    void bind() const;
    static void unbind();

private:
    void init();
    GLuint _vaoId;
};