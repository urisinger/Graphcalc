#include "shader.h"


Shader::Shader(const std::string& vertex_file_path, const std::string& fragment_file_path, vec2 res)
{
    // Create the shader program
    Renderer_ID = glCreateProgram();

    // Compile the vertex and fragment shaders
    unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, OpenShader(vertex_file_path));
    unsigned int fragment_shader = CompileShader(GL_FRAGMENT_SHADER, OpenShader(fragment_file_path));

    // Attach the shaders to the program and link it
    glAttachShader(Renderer_ID, vertex_shader);
    glAttachShader(Renderer_ID, fragment_shader);
    glLinkProgram(Renderer_ID);
    glValidateProgram(Renderer_ID);



    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(Renderer_ID);

    location = glGetUniformLocation(Renderer_ID, "Screen_Size");

    glUniform2f(location,res.x,res.y);
    location = glGetUniformLocation(Renderer_ID, "zoom");


    glUniform2f(location, 5.0, 5.0*res.ratio);
    location = glGetUniformLocation(Renderer_ID, "offset");

    glUniform2f(location, 0.0, 0.0);
}

void Shader::bind() {
    glUseProgram(Renderer_ID);
}

void Shader::unbind() {
    glUseProgram(0);
}


void Shader::setcamerauniform(vec2 res,vec2 zoom, vec2 offset) {
    location = glGetUniformLocation(Renderer_ID, "Screen_Size");

    glUniform2f(location, res.x, res.y);

    location = glGetUniformLocation(Renderer_ID, "zoom");

    glUniform2f(location, zoom.x, zoom.y*res.ratio);
    location = glGetUniformLocation(Renderer_ID, "offset");

    glUniform2f(location, offset.x, offset.y);
}

void Shader::setevaluniform(std::queue<Token> postfix){
    float values[256];
    int opers[256];
    int location;

    int size = postfix.size();
    for(int cur = 0; cur < 256 && !postfix.empty(); cur++){

        values[cur] = postfix.front().val;
        opers[cur] = postfix.front().oper;
        std::cout << opers[cur]<< ": " << postfix.front().val <<std::endl;
        postfix.pop();
    }

    location = glGetUniformLocation(Renderer_ID, "size");
    glUniform1i(location, size);

    location = glGetUniformLocation(Renderer_ID, "values");
    glUniform1fv(location, 256,values);

    location = glGetUniformLocation(Renderer_ID, "opers");

    glUniform1iv(location, 256,opers);

}
Shader::~Shader() {
    glDeleteProgram(Renderer_ID);
}



std::string Shader::OpenShader(std::string filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* messge = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, messge);
        std::cout << "failt to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << messge << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}



