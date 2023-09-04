//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// vec3 e vec4 sono un tipo di dato GLSL
//// gl_Position deve essere normalized coord, quindi se necessario devo sistemare le coordinate
//// Nota: l'1.0 in fondo codifica una posizione nello spazio, mentre uno 0 codifica una direzione nello spazio
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);\n"
//"}\0";
//// Vado a colorare il frammento 
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(0.5f, 0.5f, 0.2f, 1.0f);\n"
//"}\n\0";
//
//int main()
//{
//    // inizializzo e configuro glfw
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    // Per Apple:
//    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
//    // creo una glfw windows
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // dico a openGl di compilare lo vertex shader
//    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    // mostro eventuali errori in fase di compilazione dello shader
//    int success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    // in questo momento ho caricato i vertici dalla mem centrale alla GPU e li ho letti 
//
//    // fragment shader
//    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // mostro eventuali errori in fase di compilazione dello shader
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    // linko gli shaders
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // mostro eventuali errori in fase di linking
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    float vertici[] = {
//        // I vertici sono in memoria centrale e gestiti dalla CPU e devono arrivare alla GPU per essere gestite dal VertexShader
//        // Sono già in normalize coordinates
//        -0.5f, -0.5f, 0.0f,  // left 
//        0.5f, -0.5f, 0.0f,  // right
//        0.0f, 0.5f, 0.0f,  // top 
//    };
//
//    unsigned int VBO, VAO;
//    // VAO: memorizza le connessioni tra i VBO e gli input
//    glGenVertexArrays(1, &VAO);
//    // genero il buffer
//    glGenBuffers(1, &VBO);
//    glBindVertexArray(VAO);
//
//    // collego il buffer alla memoria definendo la tipologia dell'oggetto che vado a memorizzare
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    // funzione che effettua il passaggio dalla mem centrale all GPU
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertici), vertici, GL_STATIC_DRAW);
//
//    // Va a dire che il nostro puntatore degli attr è composto da 3 byte 
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    glBindVertexArray(0);
//
//    // continuo ad eseguire il render loop fintantoche non dico a GLFW di chiudersi
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//
//        // render
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // disegno
//        glUseProgram(shaderProgram);
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // dealloco tutte le risorse che non uso più
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//        glfwSetWindowShouldClose(window, true);
//    }
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}