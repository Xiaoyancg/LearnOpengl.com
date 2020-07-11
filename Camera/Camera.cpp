#include "../include/Helper_functions.h"
#include <stb/stb_image.h>

void framebufferSizeFunc ( GLFWwindow *window, int width, int height );
void processKeyboardInput ( GLFWwindow *window );
void cursorPosFunc ( GLFWwindow *window, double xpos, double ypos );

const int scr_width = 800;
const int scr_height = 800;

float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
float sensitivity = 0.1f;
float yaw = 0.0f;
float pitch = 0.0f;
float roll = 0.0f;
float moveSpeed = 2.0f;
float rotateSpeed = 45.0f; // 45 degree per sec
float deltaTime = 0.0f;
float lastTime = 0.0f;
bool defaultCamera = true;
bool firstCursor = true;
glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, 6.0f );
glm::vec3 cameraFront = glm::normalize ( glm::vec3 (
	sin ( glm::radians ( yaw ) ) * cos ( glm::radians ( pitch ) ),
	sin ( glm::radians ( pitch ) ) * cos ( glm::radians ( yaw ) ),
	-cos ( glm::radians ( yaw ) ) * cos ( glm::radians ( pitch ) ) ) );
// glm::vec3 cameraUp = glm::vec3 ( 0.0f, 1.0f, 0.0f );
glm::vec3 cameraUp = glm::normalize ( glm::vec3 (
	sin ( glm::radians ( roll ) ) * cos ( glm::radians ( pitch ) ),
	cos ( glm::radians ( pitch ) ) * cos ( glm::radians ( roll ) ),
	sin ( glm::radians ( pitch ) ) * cos ( glm::radians ( roll ) ) ) );
enum VAO_ID
{
	Box, numVAOs
};

enum Buffer_ID
{
	AbBox, numBuffers
};

enum Attrib_ID
{
	AvPos = 0, AvTexCoord = 1
};

enum Uniform_id
{
	Umodel, Uview, Uprojection, UsContainer, UsFace, numUniforms
};

enum Texture_ID
{
	TContainer, TFace, numTextures
};

enum Program_ID
{
	PBox, numPrograms
};

enum Shader_ID
{
	VBox, FBox, numShaders,
};

GLuint vaos[numVAOs];
GLuint bfrs[numBuffers];
GLuint pgms[numPrograms];
GLuint sdrs[numShaders];
GLuint unfs[numUniforms];
GLuint texs[numTextures];

void
init ()
{
	// Initialize Global gl variables
	glClearColorfv ( Color::grey );
	glViewport ( ZERO, ZERO, scr_width, scr_height );
	glEnable ( GL_DEPTH_TEST );

	// Varaibles Declaration
	GLfloat vArray[] =
	{
		-0.5, -0.5,  0.0,  0.0,  0.0,
		-0.5,  0.5,  0.0,  0.0,  1.0,
		 0.5,  0.5,  0.0,  1.0,  1.0,
		 0.5,  0.5,  0.0,  1.0,  1.0,
		 0.5, -0.5,  0.0,  1.0,  0.0,
		-0.5, -0.5,  0.0,  0.0,  0.0
	};
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	GLubyte *texData0, *texData1;
	GLint width, height, nrChannels;
	glCreateVertexArrays ( numVAOs, vaos );
	glCreateBuffers ( numBuffers, bfrs );
	glCreateTextures ( GL_TEXTURE_2D, numTextures, texs );
	pgms[PBox] = glCreateProgram ();
	sdrs[VBox] = glCreateShader ( GL_VERTEX_SHADER );
	sdrs[FBox] = glCreateShader ( GL_FRAGMENT_SHADER );

	// Programs
	Helper::shaderWorker ( pgms[PBox], sdrs[VBox], "vertex.glsl" );
	Helper::shaderWorker ( pgms[PBox], sdrs[FBox], "fragment.glsl" );
	glLinkProgram ( pgms[PBox] );
	glDeleteShader ( sdrs[VBox] );
	glDeleteShader ( sdrs[FBox] );

	// VAOs
	glBindVertexArray ( vaos[Box] );
	glBindBuffer ( GL_ARRAY_BUFFER, bfrs[AbBox] );
	/*glNamedBufferStorage ( bfrs[AbBox], sizeof ( vArray ), vArray,
						   GL_DYNAMIC_STORAGE_BIT );*/
	glNamedBufferStorage ( bfrs[AbBox], sizeof ( vertices ), vertices,
						   GL_DYNAMIC_STORAGE_BIT );
	glVertexAttribPointer ( AvPos, VEC_SIZE::VEC3, GL_FLOAT, GL_FALSE,
							Helper::typeSize ( GL_FLOAT_VEC3 )
							+ Helper::typeSize ( GL_FLOAT_VEC2 ),
							bufferOffset ( 0 ) );
	glEnableVertexAttribArray ( AvPos );
	glVertexAttribPointer ( AvTexCoord, VEC_SIZE::VEC2, GL_FLOAT, GL_FALSE,
							Helper::typeSize ( GL_FLOAT_VEC3 )
							+ Helper::typeSize ( GL_FLOAT_VEC2 ),
							bufferOffset ( Helper::typeSize ( GL_FLOAT_VEC3 ) ) );
	glEnableVertexAttribArray ( AvTexCoord );
	glBindVertexArray ( ClearBit );
	glBindBuffer ( GL_ARRAY_BUFFER, ClearBit );

	// Uniforms
	unfs[UsContainer] = glGetUniformLocation ( pgms[PBox], "sContainer" );
	unfs[UsFace] = glGetUniformLocation ( pgms[PBox], "sFace" );
	unfs[Umodel] = glGetUniformLocation ( pgms[PBox], "model" );
	unfs[Uview] = glGetUniformLocation ( pgms[PBox], "view" );
	unfs[Uprojection] = glGetUniformLocation ( pgms[PBox], "projection" );

	// Textures
	glActiveTexture ( GL_TEXTURE0 );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					  GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture ( GL_TEXTURE_2D, texs[TContainer] );
	glUseProgram ( pgms[PBox] );
	glUniform1i ( unfs[UsContainer], TContainer );
	glUseProgram ( ClearBit );
	texData0 = stbi_load ( "container.jpg", &width, &height, &nrChannels, 0 );
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				   GL_UNSIGNED_BYTE, texData0 );
	glGenerateMipmap ( GL_TEXTURE_2D );
	stbi_image_free ( texData0 );
	glActiveTexture ( GL_TEXTURE1 );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					  GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture ( GL_TEXTURE_2D, texs[TFace] );
	glUseProgram ( pgms[PBox] );
	glUniform1i ( unfs[UsFace], TFace );
	glUseProgram ( ClearBit );
	stbi_set_flip_vertically_on_load ( true );
	texData1 = stbi_load ( "face.png", &width, &height, &nrChannels, 0 );
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
				   GL_UNSIGNED_BYTE, texData1 );
	glGenerateMipmap ( GL_TEXTURE_2D );
	stbi_image_free ( texData1 );

}

void
display ()
{
	// Variables Declaration
	glm::mat4 model ( 1.0f );
	glm::mat4 view ( 1.0f );
	glm::mat4 projection;
	model = glm::rotate ( model, ( float ) glfwGetTime () * glm::radians ( 50.0f ),
						  glm::vec3 ( 0.5f, 1.0f, 0.0f ) );
	float timex = glfwGetTime ();
	float radian = 10.0f;
	float cx = ( float ) sin ( timex ) * radian;
	float cz = ( float ) cos ( timex ) * radian;

	if ( defaultCamera )
	{
		view = glm::lookAt ( glm::vec3 ( cx, 0.0f, cz ),
							 glm::vec3 ( 0.0f, 0.0f, 0.0f ),
							 glm::vec3 ( 0.0f, 1.0f, 0.0f ) );
	} else
	{
		view = glm::lookAt ( cameraPos, cameraPos + cameraFront, cameraUp );
	}
	projection = glm::perspective ( glm::radians ( 45.0f ), 1.0f, 0.01f, 100.0f );
	glm::vec3 cubePoss[] = {
		glm::vec3 ( 0.5f, 0.5f, 0.0f ),
		glm::vec3 ( -2.5f, -2.5f, -4.0f )
	};

	// Clear
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Draw
	glUseProgram ( pgms[PBox] );
	glBindVertexArray ( vaos[Box] );
	glUniformMatrix4fv ( unfs[Umodel], 1, GL_FALSE, glm::value_ptr ( model ) );
	glUniformMatrix4fv ( unfs[Uview], 1, GL_FALSE, glm::value_ptr ( view ) );
	glUniformMatrix4fv ( unfs[Uprojection], 1, GL_FALSE,
						 glm::value_ptr ( projection ) );
	glDrawArrays ( GL_TRIANGLES, 0, 36 );
	for ( unsigned int i = 0; i < 2; i++ )
	{
		glm::mat4 model ( 1.0f );
		model = glm::translate ( model, cubePoss[i] );
		model = glm::rotate ( model, glm::radians ( i * 40.0f + 60.0f ),
							  glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
		glUniformMatrix4fv ( unfs[Umodel], 1, GL_FALSE, glm::value_ptr ( model ) );
		glDrawArrays ( GL_TRIANGLES, 0, 36 );
	}
	glBindVertexArray ( ClearBit );
	glUseProgram ( ClearBit );


}

void
terminate ()
{
	glDeleteProgram ( pgms[PBox] );
}

int
main ( int argc, char **argv )
{
	glfwInit ();
	GLFWwindow *window = glfwCreateWindow ( scr_width, scr_height,
											"Coordinate System",
											NULL, NULL );
	glfwMakeContextCurrent ( window );
	gl3wInit ();
	init ();
	glfwSetFramebufferSizeCallback ( window, framebufferSizeFunc );
	glfwSetInputMode ( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	glfwSetCursorPosCallback ( window, cursorPosFunc );

	while ( !glfwWindowShouldClose ( window ) )
	{
		processKeyboardInput ( window );
		display ();
		glfwSwapBuffers ( window );
		glfwPollEvents ();
	}
	terminate ();
	glfwDestroyWindow ( window );
	glfwTerminate ();
	return EXIT_SUCCESS;
}

void
framebufferSizeFunc ( GLFWwindow *window, int width, int height )
{
	glViewport ( 0, 0, width, height );
}

void processKeyboardInput ( GLFWwindow *window )
{
	if ( glfwGetKey ( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
	{
		glfwSetWindowShouldClose ( window, true );
	}

	if ( defaultCamera )
	{
		if ( glfwGetKey ( window, GLFW_KEY_ENTER ) == GLFW_PRESS )
		{
			defaultCamera = false;
		}
		return;
	}
	float currentTime = glfwGetTime ();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	float deltaMovement = deltaTime * moveSpeed;
	float deltaRotateYaw = deltaTime * rotateSpeed;
	float deltaRoll = deltaTime * rotateSpeed;
	// WSAD space ctrl
	if ( glfwGetKey ( window, GLFW_KEY_W ) == GLFW_PRESS ||
		 glfwGetKey ( window, GLFW_KEY_UP ) == GLFW_PRESS )
	{
		cameraPos += deltaMovement * cameraFront;
	}
	if ( glfwGetKey ( window, GLFW_KEY_S ) == GLFW_PRESS ||
		 glfwGetKey ( window, GLFW_KEY_DOWN ) == GLFW_PRESS )
	{
		cameraPos -= deltaMovement * cameraFront;
	}
	if ( glfwGetKey ( window, GLFW_KEY_A ) == GLFW_PRESS )
	{
		cameraPos -=
			deltaMovement *
			glm::normalize ( glm::cross ( cameraFront, cameraUp ) );
	}
	if ( glfwGetKey ( window, GLFW_KEY_D ) == GLFW_PRESS )
	{
		cameraPos +=
			deltaMovement *
			glm::normalize ( glm::cross ( cameraFront, cameraUp ) );
	}
	if ( glfwGetKey ( window, GLFW_KEY_SPACE ) == GLFW_PRESS )
	{
		cameraPos += deltaMovement * cameraUp;
	}
	if ( glfwGetKey ( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS )
	{
		cameraPos -= deltaMovement * cameraUp;
	}

	// arrow
	if ( glfwGetKey ( window, GLFW_KEY_RIGHT ) == GLFW_PRESS )
	{
		yaw += deltaRotateYaw;
		glm::vec3 newFront = glm::vec3 ( sin ( glm::radians ( yaw ) )
										 * cos ( glm::radians ( pitch ) ),
										 sin ( glm::radians ( pitch ) ),
										 -cos ( glm::radians ( yaw ) )
										 * cos ( glm::radians ( pitch ) ) );
		cameraFront = glm::normalize ( newFront );
	}
	if ( glfwGetKey ( window, GLFW_KEY_LEFT ) == GLFW_PRESS )
	{
		yaw -= deltaRotateYaw;
		glm::vec3 newFront = glm::vec3 ( sin ( glm::radians ( yaw ) )
										 * cos ( glm::radians ( pitch ) ),
										 sin ( glm::radians ( pitch ) ),
										 -cos ( glm::radians ( yaw ) )
										 * cos ( glm::radians ( pitch ) ) );
		cameraFront = glm::normalize ( newFront );
	}
	if ( glfwGetKey ( window, GLFW_KEY_E ) == GLFW_PRESS )
	{
		roll += deltaRoll;
		glm::vec3 newUp = glm::vec3 ( sin ( glm::radians ( roll ) )
									  ,
									  cos ( glm::radians ( roll ) )
									  ,
									  0.0f );
		cameraUp = glm::normalize ( newUp );
	}
	if ( glfwGetKey ( window, GLFW_KEY_R ) == GLFW_PRESS )
	{
		roll = 0.0f;
		pitch = 0.0f;
		yaw = 0.0f;
		glm::vec3 newFront = glm::vec3 ( sin ( glm::radians ( yaw ) )
										 * cos ( glm::radians ( pitch ) ),
										 sin ( glm::radians ( pitch ) ),
										 -cos ( glm::radians ( yaw ) )
										 * cos ( glm::radians ( pitch ) ) );
		glm::vec3 newUp = glm::vec3 ( sin ( glm::radians ( roll ) )
									  ,
									  cos ( glm::radians ( roll ) )
									  ,
									  0.0f );
		cameraUp = glm::normalize ( newUp );
		cameraFront = glm::normalize ( newFront );
		cameraPos = glm::vec3 ( 0.0f, 0.0f, 6.0f );
	}
}

void cursorPosFunc ( GLFWwindow *window, double posX, double posY )
{
	if ( defaultCamera )
	{
		return;
	}
	if ( firstCursor )
	{
		firstCursor = false;
		lastX = posX;
		lastY = posY;
		return;
	}
	float deltaX = posX - lastX;
	float deltaY = posY - lastY;
	deltaY = -deltaY;
	lastX = posX;
	lastY = posY;
	float deltaRotateYaw = deltaX / 500.0f * rotateSpeed;
	float deltaRotatePitch = deltaY / 500.0f * rotateSpeed;
	yaw += deltaRotateYaw;
	pitch += deltaRotatePitch;
	if ( pitch >= 89.0f )
	{
		pitch = 89.0f;
	}
	if ( pitch <= -89.0f )
	{
		pitch = -89.0f;
	}

	glm::vec3 newFront = glm::vec3 ( sin ( glm::radians ( yaw ) )
									 * cos ( glm::radians ( pitch ) ),
									 sin ( glm::radians ( pitch ) )
									,
									 -cos ( glm::radians ( yaw ) )
									 * cos ( glm::radians ( pitch ) ) );
	glm::vec3 newUp = glm::vec3 ( sin ( glm::radians ( roll ) )
								  * cos ( glm::radians ( pitch ) ),
								  cos ( glm::radians ( roll ) )
								  * cos ( glm::radians ( pitch ) ),
								  sin ( glm::radians ( pitch ) )
								  * cos ( glm::radians ( roll ) ) );
	cameraFront = glm::normalize ( newFront );
	cameraUp = glm::normalize ( newUp );
}
