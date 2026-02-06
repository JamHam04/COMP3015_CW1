#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

void SceneBasic_Uniform::initScene()
{
    compile();
    
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );

    glBindVertexArray(0);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
