#pragma once

#include <map>
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
    void init();
    std::string _path;
    Type _type;
    GLuint _id;
};

class Program {
public:
    Program(std::shared_ptr<const ShaderModule> pVertShader,
            std::shared_ptr<const ShaderModule> pFragShader);
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

private:
    void init();
    std::shared_ptr<const ShaderModule> _pVertShader;
    std::shared_ptr<const ShaderModule> _pFragShader;
    GLuint _id;
};

class Pipeline {
public:
    Pipeline(std::shared_ptr<const Program> pProgram);
    Pipeline(const Pipeline& other);
    Pipeline& operator=(const Pipeline& other);
    ~Pipeline();
    const std::shared_ptr<const Program>& getPProgram() const {
        return _pProgram;
    }
    GLuint getVaoId() const {
        return _vaoId;
    }
    void bind() const;
    bool unbind() const;

private:
    void init();
    std::shared_ptr<const Program> _pProgram;
    static uint _uidGenerator;
    static uint _boundPipelineUid;
    uint _uid;
    GLuint _vaoId;
};