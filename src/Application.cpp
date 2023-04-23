#include "Application.h"


void MessageCallback( GLenum source,
                           GLenum type,
                           GLuint id,
                           GLenum severity,
                           GLsizei length,
                           const GLchar* message,
                           const void* userParam ){
fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
type, severity, message );
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));

    application->zoom.x += yoffset * application->zoom.x / 5;
    application->zoom.y += yoffset * application->zoom.y / 5;

    application->_Shaders[0].setcamerauniform(application->res,application->zoom,application->offset);
}

void mouse_callback(GLFWwindow* window,int button, int action, int mods) {

}

Application::Application(int screen_X, int screen_Y)
    :res(screen_X, screen_Y),
    zoom(20.0,20.0),
    offset(0.0,0.0),
     parser("x=5")
{
    unsigned int indecies[6] = { 0,1,2,
                  0,1,3 };
    /* Initialize the library */
    glfwInit();
    /* Create a windowed mode window and its OpenGL context */
    _window = glfwCreateWindow(screen_X, screen_Y, "Hello World", NULL, NULL);
    if(!_window){
        exit(1);
    };

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit (-1);
    }

    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );
    glfwSetWindowUserPointer(_window, this);
    glfwSetScrollCallback(_window, scroll_callback);
    glfwSetMouseButtonCallback(_window, mouse_callback);
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    _VertexBuffers.emplace_back();
    _Shaders.emplace_back("../../res/shaders/default.vert", "../../res/shaders/default.frag",res);
    _IndexBuffers.emplace_back();
    _IndexBuffers[0].adddata(indecies, 6);
}

void Application::GameLoop() {
    double prevtime=0;
    double currenttime=0;
    double timediff = 0;
    int counter = 0;
    std::cout << parser.eval(3,5) << std::endl;
    while (!glfwWindowShouldClose(_window)) {
        float vertcies[8] = { 1.0, 1.0,-1.0,-1.0,-1.0, 1.0, 1.0,-1.0 };

        Draw(vertcies, 0, 8);

        currenttime = glfwGetTime();
        timediff = currenttime - prevtime;
        counter++;
        if (timediff >= 1 / 30.0) {
            glfwSetWindowTitle(_window, (std::to_string((1.0 / timediff) * counter)).c_str());
            prevtime = currenttime;
            counter = 0;
        }
        /* Swap front and back buffers */
        glfwSwapBuffers(_window);

        /* Poll for and process events */

        glfwPollEvents();
        

    }
}

void Application::Draw(void* vertcies, int index, int count) {


    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 0, 0, 1);
    _Shaders[0].bind();
    _VertexBuffers[index].AddData(vertcies,count*2);
    
    _VertexBuffers[index].Bind();
    _IndexBuffers[index].Bind();


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    _VertexBuffers[index].UnBind();
}

