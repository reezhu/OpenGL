#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    std::cout << "File:" << std::endl;
    std::cout << filepath << std::endl;
    std::cout << "Vertex:" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment:" << std::endl;
    std::cout << source.FragmentSource << std::endl;
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
    GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
    GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1,v2,v3));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }


    }
    return { ss[0].str(),ss[1].str() };
}
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;

    }
    return id;
}
unsigned int Shader::CreateShader(const std::string& vertextShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertextShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;

}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) 
        std::cout << "Warning:uniform not exist!"<<name << std::endl;
    m_uniformLocationCache[name] = location;
	return location;
}
