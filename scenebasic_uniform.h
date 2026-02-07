#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "helper/plane.h"

class SceneBasic_Uniform : public Scene
{
private:
    Plane plane;
    GLuint vaoHandle;
    GLSLProgram prog;
    float angle;

    void compile();
	void setMatrices();

    // Textures
	GLuint floorTexture;
	GLuint wallTexture;
	GLuint damageTexture;

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
