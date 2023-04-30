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

int TextInputCallback(ImGuiInputTextCallbackData* data)
{
    // Check if the text has been modified
    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
    {
        // The text has been modified
        // Do something here, e.g. update a flag
        return 1;
    }

    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));

    application->zoom.x += yoffset * application->zoom.x / 5;
    application->zoom.y += yoffset * application->zoom.y / 5;

    application->maskshader->SetCameraUniform(application->res,application->zoom,application->offset);
    application->lineshader->SetCameraUniform(application->res,application->zoom,application->offset);
}

void mouse_callback(GLFWwindow* window,int button, int action, int mods) {

    auto& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }

    auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));


    double xpos;
    double ypos;

    glfwGetCursorPos(window, &xpos, &ypos);
    if(button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS){
        application->lastpress = vec2((2*(xpos/application->res.x)-1)*application->zoom.x+application->offset.x,-(2*(ypos/application->res.y)-1)*application->zoom.y+application->offset.y);
    }
}

Application::Application(int screen_X, int screen_Y)
    :res(screen_X, screen_Y),
    zoom(5.0,5.0),
    offset(0.0,0.0),
     lastpress(0,0)
{

    float vertcies[8] = { 1.0, 1.0,-1.0,-1.0,-1.0, 1.0, 1.0,-1.0 };

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

    glEnable( GL_DEBUG_OUTPUT );
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback( MessageCallback, 0 );
    glfwSetWindowUserPointer(_window, this);
    glfwSetScrollCallback(_window, scroll_callback);
    glfwSetMouseButtonCallback(_window, mouse_callback);


    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);


    Mask = new FrameBuffer(res);
    maskshader = new Shader("../dependencies/shaders/Default.vert", "../dependencies/shaders/First.frag",res);
    lineshader = new Shader("../dependencies/shaders/Default.vert", "../dependencies/shaders/Second.frag",res);
    _VertexBuffers.emplace_back();
    _IndexBuffers.emplace_back();
    lineshader->BindTexture(0);
    _VertexBuffers[0].AddData(vertcies,8*2);
    _IndexBuffers[0].AddData(indecies, 6);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    input[0]='\0';
}

void Application::GameLoop() {
    double prevtime=0;
    double currenttime=0;
    double timediff = 0;
    int counter = 0;
    parser.SetText("0");
    parser.SetUniform(maskshader->GetID());
    double xpos;
    double ypos;
    while (!glfwWindowShouldClose(_window)) {
        auto& io = ImGui::GetIO();
        if(glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !(io.WantCaptureMouse)) {
            glfwGetCursorPos(_window, &xpos, &ypos);
            offset.x += lastpress.x - ((2 * (xpos / res.x) - 1) * zoom.x + offset.x);
            offset.y += lastpress.y - (-(2 * (ypos / res.y) - 1) * zoom.y + offset.y);

            maskshader->SetCameraUniform(res, zoom, offset);
            lineshader->SetCameraUniform(res, zoom, offset);
        }



        Draw();

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

void Application::Draw() {
    
    _VertexBuffers[0].Bind();
    _IndexBuffers[0].Bind();
    Mask->Bind();
    maskshader->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    maskshader->UnBind();
    Mask->UnBInd();
     glClear(GL_COLOR_BUFFER_BIT);

    lineshader->Bind();

    glActiveTexture(GL_TEXTURE0);
    Mask->BindTexture();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    glBindTexture(GL_TEXTURE_2D,0);
   _VertexBuffers[0].UnBind();

   ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Gui");

    if (ImGui::InputText("", input, 256, ImGuiInputTextFlags_CallbackEdit, TextInputCallback)) {
        parser.SetText(input);
        parser.SetUniform(maskshader->GetID());
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

