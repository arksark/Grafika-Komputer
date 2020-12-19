#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shadowmapShader, 
		mejaVBO, mejaVAO, mejaEBO, mejatexture, 
		laciVBO, laciVAO, laciEBO, lacitexture, 
		lantaiVBO, lantaiVAO, lantaiEBO, lantaitexture, 
		dindingVBO, dindingVAO, dindingEBO, dindingtexture, 
		pintuVAO, pintuVBO, pintuEBO, pintutexture,
		lemariVBO, lemariVAO, lemariEBO, lemaritexture, 
		pilemVAO, pilemVBO, pilemEBO, pilemtexture,
		rakVBO, rakVAO, rakEBO, raktexture, 
		tvVBO, tvVAO, tvEBO, tvtexture, 
		layarVBO, layarVAO, layarEBO, layartexture,
		VAO2, VBO2, EBO2, texture2, stexture;

	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	void BuildMeja();
	void BuildLaciMeja();
	void BuildLemari();
	void BuildPilem();
	void BuildRakTV();
	void BuildTV();
	void BuildLayarTV();
	void BuildLantai();
	void BuildDinding();
	void BuildPintu();
	void DrawMeja();
	void DrawLaciMeja();
	void DrawLemari();
	void DrawPilem();
	void DrawRakTV();
	void DrawTV();
	void DrawLayarTV();
	void DrawLantai();
	void DrawDinding();
	void DrawPintu();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

