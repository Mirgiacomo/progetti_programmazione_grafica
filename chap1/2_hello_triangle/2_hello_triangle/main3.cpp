#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// vec3 e vec4 sono un tipo di dato GLSL
// gl_Position deve essere normalized coord, quindi se necessario devo sistemare le coordinate
// Nota: l'1.0 in fondo codifica una posizione nello spazio, mentre uno 0 codifica una direzione nello spazio
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Vado a colorare il frammento 
const char* fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
    // inizializzo e configuro glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Per Apple:
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // creo una glfw windows
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Dimensione e resize finestra
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Controllo se ci sono problemi in fase di init di glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderArancione = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderGiallo = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramArancione = glCreateProgram();
    unsigned int shaderProgramGiallo = glCreateProgram();
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderArancione, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShaderArancione);
    glShaderSource(fragmentShaderGiallo, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShaderGiallo);*/


    // dico a openGl di compilare lo vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attaco il source code dello shader allo shader obj e lo compilo
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // mostro eventuali errori in fase di compilazione dello shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // in questo momento ho caricato i vertici dalla mem centrale alla GPU e li ho letti 

    // fragment shader
    //unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderArancione = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderGiallo = glCreateShader(GL_FRAGMENT_SHADER);

    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);
    glShaderSource(fragmentShaderArancione, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShaderArancione);
    glShaderSource(fragmentShaderGiallo, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShaderGiallo);


    // mostro eventuali errori in fase di compilazione dello shader
    glGetShaderiv(fragmentShaderArancione, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderArancione, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // linko gli shaders al programma
    // unsigned int shaderProgram = glCreateProgram();
    unsigned int shaderProgramArancione = glCreateProgram();
    unsigned int shaderProgramGiallo = glCreateProgram();
    glAttachShader(shaderProgramArancione, vertexShader);
    glAttachShader(shaderProgramArancione, fragmentShaderArancione);
    glLinkProgram(shaderProgramArancione);
    glAttachShader(shaderProgramGiallo, vertexShader);
    glAttachShader(shaderProgramGiallo, fragmentShaderGiallo);
    glLinkProgram(shaderProgramGiallo);

    // mostro eventuali errori in fase di linking
    glGetProgramiv(shaderProgramArancione, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramArancione, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glGetProgramiv(shaderProgramGiallo, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramGiallo, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Libero le risorse
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderArancione);
    glDeleteShader(fragmentShaderGiallo);

    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    };

    unsigned int VBO[2], VAO[2];
    // Genero un VAO e un VBO
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    // primo triangolo
    // Faccio il bind e ci copio i vertici 
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    // Specifico ad opengl come deve interpretare il vertex buffer data in fase di grafica
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // secondo triangolo
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // continuo ad eseguire il render loop fintantoche non dico a GLFW di chiudersi
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // disegno
        glUseProgram(shaderProgramArancione);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgramGiallo);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // dealloco tutte le risorse che non uso più
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteProgram(shaderProgramArancione);
    glDeleteProgram(shaderProgramGiallo);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}