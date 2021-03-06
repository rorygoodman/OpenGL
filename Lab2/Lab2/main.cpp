#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
int mode = 0;
// The MAIN function, from here we start the application and run the game loop
void keyCallback(GLFWwindow *window,int key, int scancode, int action, int mods);
int main()
{
    // Init GLFW
    glfwInit( );
    
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Lab2", nullptr, nullptr );
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwSetKeyCallback(window, keyCallback);
    
    glfwMakeContextCurrent( window );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );
    
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shader
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );
    
    // Check for compile time errors
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
    
    if ( !success )
    {
        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Link shaders
    GLuint shaderProgram = glCreateProgram( );
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );
    
    // Check for linking errors
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
    
    if ( !success )
    {
        glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
    
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f, // Left
        0.5f, -0.5f, 0.0f, // Right
        0.0f,  0.5f, 0.0f  // Top
    };
    
    GLuint VBO, VAO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray( VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray( 0 ); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
    glm::mat4 transform;
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Draw our first triangle
        glUseProgram( shaderProgram );
        
        
        //transform = glm::translate(transform, glm::vec3(0.5f,0.5f,0.0f));
        if(mode==0){
            //transform = glm::translate(transform, glm::vec3(0.0f,0.0f,0.0f));
        }
        else if(mode==1){
             transform = glm::rotate(transform, (GLfloat)glfwGetTime()*5.0f, glm::vec3(0.0f,1.0f,0.0f));
        }
        else if(mode==2){
            transform = glm::rotate(transform, (GLfloat)glfwGetTime()*5.0f, glm::vec3(1.0f,0.0f,0.0f));
        }
        else if(mode ==3) {
            transform = glm::rotate(transform, (GLfloat)glfwGetTime()*5.0f, glm::vec3(0.0f,0.0f,1.0f));
        }
        else if(mode ==4){
            transform = glm::translate(transform, glm::vec3(0.1f,0.0f,0.0f));
            mode=0;
        }
        else if(mode ==5){
            transform = glm::translate(transform, glm::vec3(-0.1f,0.0f,0.0f));
            mode=0;
        }
        else if(mode ==6){
            transform = glm::translate(transform, glm::vec3(0.0f,0.1f,0.0f));
            mode=0;
        }
        else if(mode ==7){
            transform = glm::translate(transform, glm::vec3(0.0f,-0.1f,0.0f));
            mode=0;
        }
        else if(mode ==8){
            transform = glm::translate(transform, glm::vec3(0.0f,0.0f,0.1f));
            mode=0;
        }
        else if(mode ==9){
            transform = glm::translate(transform, glm::vec3(0.0f,0.0f,-0.1f));
            mode=0;
        }
        else if(mode ==10){
            transform= glm::scale(transform, glm::vec3(1.5f,1.5f,1.5f));
            mode=0;
        }
        else if(mode ==11){
            transform= glm::scale(transform, glm::vec3(0.5f,0.5f,0.5f));
            mode=0;
        }
        else if(mode ==12){
            transform= glm::scale(transform, glm::vec3(1.0f,0.5f,1.0f));
            mode=0;
        }
        else if(mode ==13){
            transform= glm::scale(transform, glm::vec3(1.0f,1.5f,1.0f));
            mode=0;
        }
        else if(mode ==14){
            transform= glm::scale(transform, glm::vec3(0.5f,1.0f,0.0f));
            mode=0;
        }
        else if(mode ==15){
            transform= glm::scale(transform, glm::vec3(1.5f,1.0f,1.0f));
            mode=0;
        }
        else{
            
        }
        
        
        
       
        GLint transformLocation = glGetUniformLocation(shaderProgram,"transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 3 );
        glBindVertexArray( 0 );
        
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}
void keyCallback(GLFWwindow *window,int key, int scancode, int action, int mods){
    std::cout<< key << std::endl;
    if(key==81){
        mode=0;
    }
    else if(key==87){
        mode=1;
    }
    else if(key==69){
        mode=2;
    }
    else if(key==82){
        mode=3;
    }
    else if(key==84){
        mode=4;
    }
    else if(key==89){
        mode=5;
    }
    else if(key==85){
        mode=6;
    }
    else if(key==73){
        mode=7;
    }
    else if(key==79){
        mode=8;
    }
    else if(key==80){
        mode=9;
    }
    else if(key==66){
        mode=10;
    }
    else if(key==83){
        mode=11;
    }
    else if(key==72){
        mode=12;
    }
    else if(key==74){
        mode=13;
    }
    else if(key==75){
        mode=14;
    }
    else if(key==76){
        mode=15;
    }
    
    else{}
}
