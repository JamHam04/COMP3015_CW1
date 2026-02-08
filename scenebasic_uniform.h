#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "helper/plane.h"
#include "helper/objmesh.h"

class SceneBasic_Uniform : public Scene
{
private:
    Plane plane;
	std::unique_ptr<ObjMesh> barrel;

    GLuint vaoHandle;
    GLSLProgram prog;
    float angle;
    float deltaTime;
	float tPrev;

    void compile();
	void setMatrices();

    // Textures
	GLuint floorDiffuseTexture;
	GLuint wallDiffuseTexture;
	GLuint damageDiffuseTexture;
	GLuint damageNormalTexture;
	GLuint floorNormalTexture;
	GLuint wallNormalTexture;

	GLuint barrelDiffuseTexture;
	GLuint barrelNormalTexture;

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
