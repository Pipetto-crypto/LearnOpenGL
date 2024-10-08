#include <GL/glew.h>
#include "shader-parser.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
	glViewport(0, 0, 800, 600);
}

void process_input(GLFWwindow *window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(){

    GLFWwindow *window;
    float vertices[] = {
    	/*position*/      /*color*/
    	0.5f, 0.0f, 0.0f, 0.3f, 0.6f, 0.2f,
       -0.5f, 0.0f, 0.0f, 0.4f, 0.2f, 0.7f,
    	0.0f, 1.0f, 0.0f, 0.5f, 0.7f, 0.3f
    };

    /* Vertex shader source code. The vertex shader represents
    a 2D image of a 3D geometry. Header declares the version.
    location refers to the vertex attribute, 0 is the position of attr
    first attrib and contains all the input points in the vertices
    stored in aPos with the in keyword. vertices are in x,y,z so we use a vec3.
    gl_Position places the shader and it is put at the end of the main
    It is a vec4 variable. x,y,z, and w depth.
    second attrib represents color and we pass it to the frag shader with
    out variable vertexColor */
    
    std::string vertexShaderSource = parseShader("src/shaders/triangle_vertex_shaders.vs");
    const char* vs = vertexShaderSource.c_str();

    /* Fragment shader source code. The fragment shader represents the color
    of the pixels. out specify the variable that stores the attribute, the
    in variable with the same name of the one from the vertex shader links
    them together */
        
    std::string fragmentShaderSource = parseShader("src/shaders/triangle_fragment_shaders.vs");
    const char* fs = fragmentShaderSource.c_str();

	
    unsigned int VBO;
    unsigned int VAO;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
    int result;
    char log[512];
        
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "GARBAGE", NULL, NULL);

	if(window == NULL) {
		throw std::runtime_error("Can't create window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();

	if(GLEW_OK != err)
		throw std::runtime_error("Can't init glew");
		
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while(!glfwWindowShouldClose(window)){
	    //Here goes code processing input
	    process_input(window);
		//Here goes rendering code
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* We create the VAO and bind it */
		
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		/* We create the VBO, we bind the VBO to the context and then we send
		   data to it
		*/
        glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO); 
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	    /* This function tells how we want to interpret the vertex data. first -
		param specify the position, defined in the vertex shader, second param
		the number of vertices, third param their type, fourth param not important
		fifth param is the stride, aka the lenght of each attribute, in this case 
		every coordinate is 4 byte and also every color, so in total is 24 byte, 
		sixth param is the offset
	    */

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		/* We create the vertext shader, set the source code and compile it */

		vertexShader = glCreateShader(GL_VERTEX_SHADER); 
		glShaderSource(vertexShader, 1, &vs, NULL);
		glCompileShader(vertexShader);

		/* We do the same thing for the fragment shader */

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fs, NULL);
		glCompileShader(fragmentShader);
		
		/* Check if the shaders compiled successfully */
		
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
		if(!result) {
			glGetShaderInfoLog(vertexShader, 512, NULL, log);
			std::cout << "Failed to compile vertex shader:" << log;
			return -1;
		}
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
		if(!result) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		    std::cout << "Failed to compile fragment shader:" << log;
		    return -1;
		}

		/* We create the shader program, attach the shaders to it and then link
		  them all together
		*/

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		/* We check if the program was linked successfully and in case we 
		  destroy the shader
		*/

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
		if(!result) {
		    glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		    std::cout << "Failed to link program:" << log;
		}
		else{
		
			/* If everything went well, we destroy the linked shaders and use
			the program */
			
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			
			/* We draw the primitive specified by the first param, where the
			third is the number of vertices */
			
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		
	    /* Here we swap between back and front buffer and check events */
	    
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return -1;
	
}
