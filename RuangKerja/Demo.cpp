#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shadowmapShader = BuildShader("texturedCube.vert", "texturedCube.frag", nullptr);
	BuildMeja();
	BuildLaciMeja();
	BuildLemari();
	BuildPilem();
	BuildRakTV();
	BuildTV();
	BuildLayarTV();
	BuildPintu();
	BuildLantai();
	BuildDinding();
	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &mejaVAO);
	glDeleteBuffers(1, &mejaVBO);
	glDeleteBuffers(1, &mejaEBO);
	glDeleteVertexArrays(1, &laciVAO);
	glDeleteBuffers(1, &laciVBO);
	glDeleteBuffers(1, &laciEBO);
	glDeleteVertexArrays(1, &pilemVAO);
	glDeleteBuffers(1, &pilemVBO);
	glDeleteBuffers(1, &pilemEBO);
	glDeleteVertexArrays(1, &lemariVAO);
	glDeleteBuffers(1, &lemariVBO);
	glDeleteBuffers(1, &lemariEBO);
	glDeleteVertexArrays(1, &rakVAO);
	glDeleteBuffers(1, &rakVBO);
	glDeleteBuffers(1, &rakEBO);
	glDeleteVertexArrays(1, &tvVAO);
	glDeleteBuffers(1, &tvVBO);
	glDeleteBuffers(1, &tvEBO);
	glDeleteVertexArrays(1, &layarVAO);
	glDeleteBuffers(1, &layarVBO);
	glDeleteBuffers(1, &layarEBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);
}

void Demo::Update(double deltaTime) {
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shadowmapShader, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shadowmapShader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attributes
	GLint lightPosLoc = glGetUniformLocation(this->shadowmapShader, "lightPos");
	glUniform3f(lightPosLoc, 0, 3.5, 5);
	GLint viewPosLoc = glGetUniformLocation(this->shadowmapShader, "viewPos");
	glUniform3f(viewPosLoc, 0, 0, 0);
	GLint lightColorLoc = glGetUniformLocation(this->shadowmapShader, "lightColor");
	glUniform3f(lightColorLoc, 0.7f, 0.7f, 0.7f);

	DrawMeja();
	DrawLaciMeja();
	DrawLemari();
	DrawPilem();
	DrawRakTV();
	DrawTV();
	DrawLayarTV();
	DrawPintu();
	DrawLantai();
	DrawDinding();
	

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildMeja()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &mejatexture);
	glBindTexture(GL_TEXTURE_2D, mejatexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// Tengah
		-0.3, -0.2, 6.5, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		-0.3, -0.2, 7.5, 1, 0,  0.0f,  0.0f,  1.0f, // 1
		-0.3,  0.5, 7.5, 1, 1,  0.0f,  0.0f,  1.0f, // 2
		-0.3,  0.5, 6.5, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		 // Kanan
		 0.5,  0.55,  7.5, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 0.5,  0.55,  6.5, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 0.5, -0.55,  6.5, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 0.5, -0.55,  7.5, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		 // Kanan Luar
		 0.55,  0.55,  7.5, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 0.55,  0.55,  6.5, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 0.55, -0.55,  6.5, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 0.55, -0.55,  7.5, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// Belakang
		-1.55, -0.55, 6.5, 0, 0, 0.0f,  0.0f,  -1.0f, // 8 
		 0.55, -0.55, 6.5, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		 0.55,  0.55, 6.5, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		-1.55,  0.55, 6.5, 0, 1, 0.0f,  0.0f,  -1.0f, // 11

		 // Kiri
		 -1.5, -0.55,  6.5, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		 -1.5, -0.55,  7.5, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		 -1.5,  0.55,  7.5, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		 -1.5,  0.55,  6.5, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		 // Kiri Luar
		 -1.55, -0.55,  6.5, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		 -1.55, -0.55,  7.5, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		 -1.55,  0.55,  7.5, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		 -1.55,  0.55,  6.5, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		 // Depan
		 -1.55, 0.5,  7.5, 0, 0,   0.0f, -1.0f,  0.0f, // 16
		  0.55, 0.5,  7.5, 1, 0,   0.0f, -1.0f,  0.0f, // 17
		  0.55, 0.55, 7.5, 1, 1,  0.0f,  -1.0f,  0.0f, // 18
		 -1.55, 0.55, 7.5, 0, 1,   0.0f, -1.0f,  0.0f, // 19

		 // Depan Kiri
		-1.55, -0.55, 7.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		-1.5,  -0.55, 7.5, 1, 0,  0.0f,  -1.0f,  0.0f, // 21
		-1.5,   0.55, 7.5, 1, 1,  0.0f,  -1.0f,  0.0f, // 22
		-1.55,  0.55, 7.5, 0, 1, 0.0f,  -1.0f,  0.0f, // 23

		// Depan Kanan
		 0.5, -0.55, 7.5, 0, 0, 0.0f,  -1.0f,  0.0f,  // 32
		 0.55, -0.55, 7.5, 1, 0, 0.0f,  -1.0f,  0.0f,  // 33
		 0.55,  0.55, 7.5, 1, 1, 0.0f,  -1.0f,  0.0f,  // 34
		 0.5,  0.55, 7.5, 0, 1, 0.0f,  -1.0f,  0.0f, // 35

		//Atas
		0.55, 0.55,  7.5, 0, 0, 0.0f,  -1.0f,  0.0f,  // 36
		-1.55, 0.55, 7.5, 1, 0, 0.0f,  -1.0f,  0.0f, // 37
		-1.55, 0.55, 6.5, 1, 1, 0.0f,  -1.0f,  0.0f, // 38
		0.55, 0.55, 6.5, 0, 1, 0.0f,  -1.0f,  0.0f,  // 39

		// Bawah
		-0.3, -0.2, 6.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 40
		0.5, -0.2, 6.5, 1, 0, 0.0f,  -1.0f,  0.0f, // 41
		0.5, -0.2,  7.5, 1, 1, 0.0f,  -1.0f,  0.0f, // 42
		-0.3, -0.2,  7.5, 0, 1, 0.0f,  -1.0f,  0.0f, // 43
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,	 // Tengah
		4,  5,  6,  4,  6,  7,   // Kanan
		8,  9,  10, 8,  10, 11,  // Kanan Luar
		12, 14, 13, 12, 15, 14,  // Belakang
		16, 18, 17, 16, 19, 18,  // Kiri 
		20, 22, 21, 20, 23, 22,  // Kiri Luar
		24, 26, 25, 24, 27, 26,	 // Depan
		28, 30, 29, 28, 31, 30,	 // Depan Kiri
		32, 34, 33, 32, 35, 34,  // Depan Kanan
		36, 38, 37, 36, 39, 38,	 // Atas
		40, 42, 41, 40, 43, 42   // Bawah
	};

	glGenVertexArrays(1, &mejaVAO);
	glGenBuffers(1, &mejaVBO);
	glGenBuffers(1, &mejaEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(mejaVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mejaVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mejaEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildLaciMeja()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &lacitexture);
	glBindTexture(GL_TEXTURE_2D, lacitexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("laci.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// Tengah
		-0.3, -0.2, 7.5, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		0.5, -0.2, 7.5, 1, 0,  0.0f,  0.0f,  1.0f, // 1
		0.5,  0.5, 7.5, 1, 1,  0.0f,  0.0f,  1.0f, // 2
		-0.3,  0.5, 7.5, 0, 1, 0.0f,  0.0f,  1.0f, // 3
	};

	unsigned int indices[] = {0,  1,  2,  0,  2,  3};

	glGenVertexArrays(1, &laciVAO);
	glGenBuffers(1, &laciVBO);
	glGenBuffers(1, &laciEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(laciVAO);

	glBindBuffer(GL_ARRAY_BUFFER, laciVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, laciEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildLemari()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &lemaritexture);
	glBindTexture(GL_TEXTURE_2D, lemaritexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("lemari2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// Kanan
		 4.9, -0.5, 2.0, 0, 0,  0.0f,  0.0f,  1.0f, // 0
		 4.2, -0.5, 2.0, 1, 0,  0.0f,  0.0f,  1.0f, // 1
		 4.2,  3.0, 2.0, 1, 1,  0.0f,  0.0f,  1.0f, // 2
		 4.9,  3.0, 2.0, 0, 1,  0.0f,  0.0f,  1.0f, // 3

		 // Kiri
		 4.9, -0.5, -1.0, 0, 0,  0.0f,  0.0f,  1.0f, // 4
		 4.2, -0.5, -1.0, 1, 0,  0.0f,  0.0f,  1.0f, // 5
		 4.2,  3.0, -1.0, 1, 1,  0.0f,  0.0f,  1.0f, // 6
		 4.9,  3.0, -1.0, 0, 1,  0.0f,  0.0f,  1.0f, // 7

		 // Belakang
		 4.9, -0.5, -1.0, 0, 0,  0.0f,  0.0f,  0.0f, // 8
		 4.9, -0.5,  2.0, 1, 0,  0.0f,  0.0f,  0.0f, // 9
		 4.9,  3.0,  2.0, 1, 1,  0.0f,  0.0f,  0.0f, // 10
		 4.9,  3.0, -1.0, 0, 1,  0.0f,  0.0f,  0.0f, // 11

		 // Atas
		 4.9,  3.0,  2.0, 0, 0,  0.0f,  0.0f,  1.0f, // 12
		 4.2,  3.0,  2.0, 1, 0,  0.0f,  0.0f,  1.0f, // 13
		 4.2,  3.0, -1.0, 1, 1,  0.0f,  0.0f,  1.0f, // 14
		 4.9,  3.0, -1.0, 0, 1,  0.0f,  0.0f,  1.0f, // 15

		 // Bawah
		 4.2, -0.49, -1.0, 0, 0,  0.0f,  0.0f,  0.0f, // 16
		 4.9, -0.49, -1.0, 1, 0,  0.0f,  0.0f,  0.0f, // 17
		 4.9, -0.49,  2.0, 1, 1,  0.0f,  0.0f,  0.0f, // 18
		 4.2, -0.49,  2.0, 0, 1,  0.0f,  0.0f,  0.0f, // 19

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,	 // Kanan
		4,  5,  6,  4,  6,  7,   // Kiri
		8,  9,  10, 8,  10, 11,  // Belakang
		12, 14, 13, 12, 15, 14,  // Atas
		16, 18, 17, 16, 19, 18,  // Bawah 
	};

	glGenVertexArrays(1, &lemariVAO);
	glGenBuffers(1, &lemariVBO);
	glGenBuffers(1, &lemariEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(lemariVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lemariVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lemariEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildPilem()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &pilemtexture);
	glBindTexture(GL_TEXTURE_2D, pilemtexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pilem.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		4.2,  3.0,   2.0, 0, 0,  0.0f,  0.0f,  1.0f, // 0
		4.2,  3.0,  -1.0, 1, 0,  0.0f,  0.0f,  1.0f, // 1
		4.2, -0.5,  -1.0, 1, 1,  0.0f,  0.0f,  1.0f, // 2
		4.2, -0.5,   2.0, 0, 1,  0.0f,  0.0f,  1.0f, // 3
	};

	unsigned int indices[] = { 0,  1,  2,  0,  2,  3 };

	glGenVertexArrays(1, &pilemVAO);
	glGenBuffers(1, &pilemVBO);
	glGenBuffers(1, &pilemEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(pilemVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pilemVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pilemEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildRakTV()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &raktexture);
	glBindTexture(GL_TEXTURE_2D, raktexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("lemari.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// Kiri
		 -4.9, -0.5, 3.0, 0, 0,  0.0f,  0.0f,  1.0f, // 0
		 -4.2, -0.5, 3.0, 1, 0,  0.0f,  0.0f,  1.0f, // 1
		 -4.2,  0.4, 3.0, 1, 1,  0.0f,  0.0f,  1.0f, // 2
		 -4.9,  0.4, 3.0, 0, 1,  0.0f,  0.0f,  1.0f, // 3

		 // Kanan
		 -4.9, -0.5, -2.0, 0, 0,  0.0f,  0.0f,  1.0f, // 4
		 -4.2, -0.5, -2.0, 1, 0,  0.0f,  0.0f,  1.0f, // 5
		 -4.2,  0.4, -2.0, 1, 1,  0.0f,  0.0f,  1.0f, // 6
		 -4.9,  0.4, -2.0, 0, 1,  0.0f,  0.0f,  1.0f, // 7

		 // Belakang1
		 -4.9, -0.5, -2.0, 0, 0,  0.0f,  0.0f,  0.0f, // 8
		 -4.9, -0.5,  3.0, 1, 0,  0.0f,  0.0f,  0.0f, // 9
		 -4.9,  0.4,  3.0, 1, 1,  0.0f,  0.0f,  0.0f, // 10
		 -4.9,  0.4, -2.0, 0, 1,  0.0f,  0.0f,  0.0f, // 11

		 // Belakang2
		 -4.9,  0.4, -0.2, 0, 0,  0.0f,  0.0f,  0.0f, // 12
		 -4.9,  0.4,  1.8, 1, 0,  0.0f,  0.0f,  0.0f, // 13
		 -4.9,  2.0,  1.8, 1, 1,  0.0f,  0.0f,  0.0f, // 14
		 -4.9,  2.0, -0.2, 0, 1,  0.0f,  0.0f,  0.0f, // 15

		 // Atas
		 -4.9, 2.0,  1.8, 0, 0,  0.0f,  0.0f,  1.0f, // 16
		 -4.2, 2.0,  1.8, 1, 0,  0.0f,  0.0f,  1.0f, // 17
		 -4.2, 2.0, -0.2, 1, 1,  0.0f,  0.0f,  1.0f, // 18
		 -4.9, 2.0, -0.2, 0, 1,  0.0f,  0.0f,  1.0f, // 19

		 // Bawah
		 -4.2, -0.49, -2.0, 0, 0,  0.0f,  0.0f,  0.0f, // 20
		 -4.9, -0.49, -2.0, 1, 0,  0.0f,  0.0f,  0.0f, // 21
		 -4.9, -0.49,  3.0, 1, 1,  0.0f,  0.0f,  0.0f, // 22
		 -4.2, -0.49,  3.0, 0, 1,  0.0f,  0.0f,  0.0f, // 23

		 // Tengah
		 -4.9, 0.4,  3.0, 0, 0,  0.0f,  0.0f,  1.0f, // 24
		 -4.2, 0.4,  3.0, 1, 0,  0.0f,  0.0f,  1.0f, // 25
		 -4.2, 0.4, -2.0, 1, 1,  0.0f,  0.0f,  1.0f, // 26
		 -4.9, 0.4, -2.0, 0, 1,  0.0f,  0.0f,  1.0f, // 27

		 // Verti1
		 -4.9, -0.5, -0.2, 0, 0,  0.0f,  0.0f,  1.0f, // 28
		 -4.2, -0.5, -0.2, 1, 0,  0.0f,  0.0f,  1.0f, // 29
		 -4.2,  2.0, -0.2, 1, 1,  0.0f,  0.0f,  1.0f, // 30
		 -4.9,  2.0, -0.2, 0, 1,  0.0f,  0.0f,  1.0f, // 31

		 // Verti2
		 -4.9, -0.5, 1.8, 0, 0,  0.0f,  0.0f,  1.0f, // 32
		 -4.2, -0.5, 1.8, 1, 0,  0.0f,  0.0f,  1.0f, // 33
		 -4.2,  2.0, 1.8, 1, 1,  0.0f,  0.0f,  1.0f, // 34
		 -4.9,  2.0, 1.8, 0, 1,  0.0f,  0.0f,  1.0f, // 35
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,	// Kiri
		4,  6,  5,  4,  6,  7,  // Kanan
		8,  9,  10, 8,  10, 11, // Belakang1
		12, 14, 13, 12, 15, 14, // Belakang2
		16, 18, 17, 16, 19, 18, // Atas
		20, 22, 21, 20, 23, 22, // Bawah
		24, 26, 25, 24, 26, 27,	// Tengah
		28, 30, 29, 28, 31, 30,	// Verti1
		32, 34, 33, 32, 34, 35	// Verti2
	};

	glGenVertexArrays(1, &rakVAO);
	glGenBuffers(1, &rakVBO);
	glGenBuffers(1, &rakEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(rakVAO);

	glBindBuffer(GL_ARRAY_BUFFER, rakVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rakEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildTV()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &tvtexture);
	glBindTexture(GL_TEXTURE_2D, tvtexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("tv.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// Kanan
		 // Belakang
		-4.6,  0.7, 0.0, 0, 0, 0.0f, 0.0f, 0.0f, // 0
		-4.6,  0.7, 1.6, 1, 0, 0.0f, 0.0f, 0.0f,// 1
		-4.6,  1.7, 1.6, 1, 1, 0.0f, 0.0f, 0.0f,// 2
		-4.6,  1.7, 0.0, 0, 1, 0.0f, 0.0f, 0.0f,// 3

		// Atas
		-4.6, 1.7,  1.6, 0, 0, 0.0f, 0.0f, 1.0f,  // 4
		-4.5, 1.7,  1.6, 1, 0, 0.0f, 0.0f, 1.0f, // 5
		-4.5, 1.7, 0.0, 1, 1,  0.0f, 0.0f, 1.0f,// 6
		-4.6, 1.7, 0.0, 0, 1,  0.0f, 0.0f, 1.0f, // 7

		// Bawah
		-4.6, 0.7, 1.6, 0, 0, 0.0f, 0.0f, 0.0f,// 8
		-4.5, 0.7, 1.6, 1, 0, 0.0f, 0.0f, 0.0f, // 9
		-4.5, 0.7, 0.0, 1, 1, 0.0f, 0.0f, 0.0f, // 10
		-4.6, 0.7, 0.0, 0, 1, 0.0f, 0.0f, 0.0f,  // 11

		// Kiri
		-4.6,  0.7, 0.0, 0, 0, 0.0f, 0.0f, 1.0f,// 12
		-4.5,  0.7, 0.0, 1, 0, 0.0f, 0.0f, 1.0f,// 13
		-4.5,  1.7, 0.0, 1, 1, 0.0f, 0.0f, 1.0f,// 14
		-4.6,  1.7, 0.0, 0, 1, 0.0f, 0.0f, 1.0f,// 15

		// Kanan
		-4.6, 0.7, 1.6, 0, 0, 0.0f, 0.0f, 1.0f,// 16
		-4.5, 0.7, 1.6, 1, 0, 0.0f, 0.0f, 1.0f,// 17
		-4.5, 1.7, 1.6, 1, 1, 0.0f, 0.0f, 1.0f,// 18
		-4.6, 1.7, 1.6, 0, 1, 0.0f, 0.0f, 1.0f,// 19

		// Kaki
		-4.6, 0.41, 1.1, 0, 0, 0.0f, 0.0f, 1.0f,// 20
		-4.5, 0.41, 1.1, 1, 0, 0.0f, 0.0f, 1.0f,// 21
		-4.5, 0.41, 0.5, 1, 1, 0.0f, 0.0f, 1.0f,// 22
		-4.6, 0.41, 0.5, 0, 1, 0.0f, 0.0f, 1.0f,// 23

		// Leher
		-4.6,  0.41, 0.7, 0, 0, 0.0f, 0.0f, 1.0f,// 24
		-4.6,  0.41, 0.9, 1, 0, 0.0f, 0.0f, 1.0f,// 25
		-4.6,  0.7, 0.9, 1, 1, 0.0f, 0.0f, 1.0f,// 26
		-4.6,  0.7, 0.7, 0, 1, 0.0f, 0.0f, 1.0f,// 27

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,	// Belakang
		4,  6,  5,  4,  6,  7,  // Atas
		8,  9,  10, 8,  10, 11, // Bawah
		12, 14, 13, 12, 15, 14, // Kiri
		16, 18, 17, 16, 19, 18, // Kanan
		20, 22, 21, 20, 23, 22, // Kaki
		24, 26, 25, 24, 26, 27,	// Leher
	};

	glGenVertexArrays(1, &tvVAO);
	glGenBuffers(1, &tvVBO);
	glGenBuffers(1, &tvEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(tvVAO);

	glBindBuffer(GL_ARRAY_BUFFER, tvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tvEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

};

void Demo::BuildLayarTV()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &layartexture);
	glBindTexture(GL_TEXTURE_2D, layartexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("layar.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		-4.5,  0.7, 0.0, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		-4.5,  0.7, 1.6, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		-4.5,  1.7, 1.6, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-4.5,  1.7, 0.0, 0, 1, 0.0f,  0.0f,  1.0f, // 3
	};

	unsigned int indices[] = { 0,  1,  2,  0,  2,  3 };

	glGenVertexArrays(1, &layarVAO);
	glGenBuffers(1, &layarVBO);
	glGenBuffers(1, &layarEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(layarVAO);

	glBindBuffer(GL_ARRAY_BUFFER, layarVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, layarEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildPintu()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &pintutexture);
	glBindTexture(GL_TEXTURE_2D, pintutexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pintu.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		 -1.0, -0.5, -7.9,  0, 0, 0.0f,  0.0f,  1.0f,
		  0.5, -0.5, -7.9,  1, 0, 0.0f,  0.0f,  1.0f,
		  0.5,  2.5, -7.9,  1, 1, 0.0f,  0.0f,  1.0f,
		 -1.0,  2.5, -7.9,  0, 1, 0.0f,  0.0f,  1.0f
	};

	unsigned int indices[] = { 0,  1,  2,  0,  2,  3 };

	glGenVertexArrays(1, &pintuVAO);
	glGenBuffers(1, &pintuVBO);
	glGenBuffers(1, &pintuEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(pintuVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pintuVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pintuEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildLantai()
{
	// Load and create a texture 
	glGenTextures(1, &lantaitexture);
	glBindTexture(GL_TEXTURE_2D, lantaitexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("lantai.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-5.0, -0.5, -8.0,  0,  0, 0.0f,  1.0f,  0.0f,
		5.0, -0.5, -8.0, 50,  0, 0.0f,  1.0f,  0.0f,
		5.0, -0.5,  10.0, 50, 50, 0.0f,  1.0f,  0.0f,
		-5.0, -0.5,  10.0,  0, 50, 0.0f,  1.0f,  0.0f,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &lantaiVAO);
	glGenBuffers(1, &lantaiVBO);
	glGenBuffers(1, &lantaiEBO);

	glBindVertexArray(lantaiVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lantaiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lantaiEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::BuildDinding()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &dindingtexture);
	glBindTexture(GL_TEXTURE_2D, dindingtexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("dinding.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// Kanan
		5.0, -0.5, -8.0,  0,  0, 0.0f,  0.0f,  1.0f, // 0
		5.0, 3.5, -8.0, 10,  0,  0.0f,  0.0f,  1.0f, // 1
		5.0, 3.5,  10.0, 10, 10,  0.0f,  0.0f,  1.0f, // 2
		5.0, -0.5, 10.0,  0, 10, 0.0f,  0.0f,  1.0f, // 3

		 // Kiri
		-5.0, -0.5, -8.0,  0,  0, 0.0f,  0.0f,  1.0f, // 4
		-5.0,  3.5, -8.0, 10,  0, 0.0f,  0.0f,  1.0f, // 5
		-5.0,  3.5, 10.0, 10, 10, 0.0f,  0.0f,  1.0f, // 6
		-5.0, -0.5, 10.0,  0, 10, 0.0f,  0.0f,  1.0f, // 7

		 // Depan
		-5.0, -0.5, -8.0,  0,  0, 0.0f,  0.0f,  1.0f, // 8
		 5.0, -0.5, -8.0, 10,  0, 0.0f,  0.0f,  1.0f, // 9
		 5.0,  3.5, -8.0, 10, 10, 0.0f,  0.0f,  1.0f, // 10
		-5.0,  3.5, -8.0,  0, 10, 0.0f,  0.0f,  1.0f, // 11

		// Belakang
		-5.0, -0.5, 10.0,  0,  0, 0.0f,  0.0f,  1.0f, // 12
		 5.0, -0.5, 10.0, 10,  0, 0.0f,  0.0f,  1.0f, // 13
		 5.0,  3.5, 10.0, 10, 10, 0.0f,  0.0f,  1.0f, // 14
		-5.0,  3.5, 10.0,  0, 10, 0.0f,  0.0f,  1.0f, // 15

		 // Langit
		 5.0, 3.5, 10.0,  0,  0, 0.0f,  0.0f,  1.0f, // 16
		-5.0, 3.5, 10.0, 10,  0, 0.0f,  0.0f,  1.0f, // 17
		-5.0, 3.5, -8.0, 10, 10, 0.0f,  0.0f,  1.0f, // 18
		 5.0, 3.5, -8.0,  0, 10, 0.0f,  0.0f,  1.0f // 19
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,	 // Kanan
		4,  5,  6,  4,  6,  7,   // Kiri
		8,  9,  10, 8,  10, 11,  // Depan
		12, 14, 13, 12, 15, 14,   // Belakang
		16, 18, 17, 16, 19, 18  // Langit
	};

	glGenVertexArrays(1, &dindingVAO);
	glGenBuffers(1, &dindingVBO);
	glGenBuffers(1, &dindingEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(dindingVAO);

	glBindBuffer(GL_ARRAY_BUFFER, dindingVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dindingEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::DrawMeja()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mejatexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(mejaVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 66, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLaciMeja()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lacitexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(laciVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLemari()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lemaritexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(lemariVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawPilem()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pilemtexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(pilemVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawRakTV()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, raktexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(rakVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTV()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tvtexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(tvVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 42, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLayarTV()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, layartexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(layarVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawPintu()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pintutexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 0);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(pintuVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLantai()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, lantaitexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.specular"), 3);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 0.4f);

	glBindVertexArray(lantaiVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawDinding()
{
	UseShader(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dindingtexture);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "material.diffuse"), 2);

	GLint shininessMatLoc = glGetUniformLocation(this->shadowmapShader, "material.shininess");
	glUniform1f(shininessMatLoc, 0.4f);

	glBindVertexArray(dindingVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shadowmapShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.5f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Lighting Materials and Lighting Maps: Press [SPACE] to toggle textured cube", 800, 600, false, false);
}