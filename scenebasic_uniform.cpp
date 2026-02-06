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

SceneBasic_Uniform::SceneBasic_Uniform() : plane(50.0f, 50.0f, 1, 1) {}

void SceneBasic_Uniform::initScene()
{
	// Camera View
	vec3 cameraPos(0.0f, 4.0f, 6.0f);
	vec3 cameraTarget(0.0f, 0.2f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);

    compile();
	model = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	projection = glm::perspective(glm::radians(70.0f), (float)width / height, 0.3f, 100.0f);

	// Light properties
	prog.setUniform("Light.L", vec3(0.9f)); // Light intensity
	prog.setUniform("Light.La", vec3(0.6f)); // Ambient light intensity
	prog.setUniform("Light.Ld", vec3(0.9f)); // Diffuse light intensity

	

    
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
	prog.setUniform("Light.Position", view * lightPos);

	// Set material properties
	vec3 diffuseColor = vec3(0.5f, 0.0f, 0.0f); 
	vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambientColor = vec3(0.1f, 0.0f, 0.0f); 

	prog.setUniform("Material.Kd", diffuseColor);
	prog.setUniform("Material.Ks", specularColor);
	prog.setUniform("Material.Ka", ambientColor);
	prog.setUniform("Material.Shininess", 100.0f);



	model = mat4(1.0f);
	setMatrices();
	plane.render();
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}

void SceneBasic_Uniform::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}
