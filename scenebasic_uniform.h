#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "helper/plane.h"
#include "helper/objmesh.h"
#include <GLFW/glfw3.h>
#include "helper/skybox.h"

class SceneBasic_Uniform : public Scene
{
private:
    Plane plane;
	std::unique_ptr<ObjMesh> barrel;
	SkyBox skybox;

    GLuint vaoHandle;
	GLuint hdrFBO, hdrTexture, quad;

    GLSLProgram prog;
	GLSLProgram skyboxProg;
    float angle;
    float deltaTime;
	float tPrev;
	void setMatrices();

    void compile();
	void setupFBO();
	void pass1(); void pass2();
	void drawScene();
	void userInput(GLFWwindow* WindowIn);   

    // Textures
	GLuint floorDiffuseTexture;
	GLuint wallDiffuseTexture;
	GLuint damageDiffuseTexture;
	GLuint damageNormalTexture;
	GLuint floorNormalTexture;
	GLuint wallNormalTexture;

	GLuint barrelDiffuseTexture;
	GLuint barrelNormalTexture;

	GLuint skyboxTexture;

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t , GLFWwindow* window);
    void render();
    void resize(int, int);
    
};

#endif // SCENEBASIC_UNIFORM_H
