#pragma once

struct ShaderProgramSource
{
	string VertexSource;
	string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	string m_FilePath; //for debug
	unordered_map<string, int> m_LocationCache;
public:
	Shader(const string& filepath);
	~Shader();
	void Bind() const;
	void Unbind() const;

	unsigned int CompileShader(unsigned int type, const string& source);
	unsigned int CreateShader(const string& vertexShader, const string& fragmentShader);
	ShaderProgramSource parseShader(const string& filepath);
	//Set Uniforms
	void SetUniform4f(const string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const string& name, float value);
	void SetUniform1i(const string& name, int value);
private:
	unsigned int GetUniformLocation(const string& name);
};

