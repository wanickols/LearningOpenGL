#include "pch.h"
#include "shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(const string& filepath)
	: m_FilePath(filepath)
{
    //gets source shaders from file and parses them
    ShaderProgramSource source = parseShader(filepath);

    //Creates shader
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);


}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
 
}

unsigned int Shader::CompileShader(unsigned int type, const string& source)
{
    unsigned int id = glCreateShader(type);
    //source must exist at this point, no temp strings
    const char* src = source.c_str();
    //specify shader source code
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id); //compile

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //returns length of shader log
        //allows to dynamically allocate length to char on stack because char[length] isn't allowed (because c++)
        char* message = (char*)alloca(length * sizeof(char));
        //uses maxLength to set length of string, by using length we can make it as long as it needs to be. Not sure why you wouldn't do this. THen it returns how long it was back to length, shouldn't change for us. 
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!" << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


unsigned int Shader::CreateShader(const string& vertexShader, const string& fragmentShader)
{
    //getting indexes for new 
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    //TODO add an assert to check if compilation completed. 


    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderProgramSource Shader::parseShader(const string& filepath)
{

    //types of shaders
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    fstream fs;
    fs.open(filepath, ios::in);

    if (fs.is_open()) {
        string line;
        stringstream ss[2];

        while (getline(fs, line))
        {


            //looking for #shader tag at top of each shader
            if (line.find("#shader") != string::npos)
            {
                //changes mode based on type of shader
                if (line.find("vertex") != string::npos)
                {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != string::npos)
                {
                    type = ShaderType::FRAGMENT;
                }
            }
            else
            {
                //prints to a ss based on current mode
                ss[int(type)] << line << "\n";
            }
        }
        fs.close();
        return { ss[0].str(), ss[1].str() };

    }
    else
    {
        cout << "ERROR::parseShader::File Unable to Open." << "\n";
    }

    return ShaderProgramSource();


}

void Shader::SetUniform4f(const string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1f(const string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform1i(const string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

unsigned int Shader::GetUniformLocation(const string& name)
{
    if (m_LocationCache.find(name) != m_LocationCache.end())
        return m_LocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        cout << "Warning uniform '" << name << "' doesn't exist!" << endl;
    else
        m_LocationCache[name] = location;

    return location;
}

//2356 2480 2364 2394 --> 2398.5

//1205 1216 1204 1189
