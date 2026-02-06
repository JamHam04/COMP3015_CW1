#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

void SceneBasic_Uniform::initScene()
{
	
    compile();
	mat4 model = glm::mat4(1.0f);
	mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	
	

    
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set light position
	vec4 lightPos = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	prog.setUniform("Light.Position", lightPos);

	// Set material properties
	vec3 diffuseColor = vec3(0.5f, 0.0f, 0.0f);
	vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambientColor = vec3(0.1f, 0.0f, 0.0f);

	prog.setUniform("Material.Kd", diffuseColor);
	prog.setUniform("Material.Ks", specularColor);
	prog.setUniform("Material.Ka", ambientColor);
	prog.setUniform("Material.Shininess", 100.0f);


}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
