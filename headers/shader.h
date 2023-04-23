#include <string>
#include <iostream>
#include <fstream>
#include <stack>

#include <glad.h>

struct vec2 {
	float x;
	float y;
	float ratio;
	vec2(float _x, float _y) {
		x = _x;
		y = _y;
		if (_x != 0.0) {
			ratio = _y / _x;
		}
		else {
			ratio = 0.0;
		}
	}
};


class Shader {
public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath ,vec2 res);
	~Shader();


	void setcamerauniform(vec2 res, vec2 zoom, vec2 offset);
	void bind();
	void unbind();

private:
	unsigned int Renderer_ID;
	unsigned int location;
	unsigned int CompileShader(unsigned int type, const std::string& source);
	std::string OpenShader(std::string filepath);
};