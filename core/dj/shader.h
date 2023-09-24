#ifndef shader_h
#define shader_h

#include "../ew/external/glad.h"
#include <string>

namespace dj {

	struct Vertex {
		float x, y, z;
		float u, v;
	};
	// Shader Class
	class Shader
	{
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);

		void use();
		void setInt(const std::string& name, int v) const;
		void setFloat(const std::string& name, float v) const;
		void setFloatArray(const std::string& name, float* v, int size) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
	private:
		unsigned int m_id;
	};

	std::string loadShaderSourceFromFile(const std::string& filePath);
	unsigned int createShader(GLenum shaderType, const char* sourceCode);
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
	unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
}

#endif