#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/texture.h"	


using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(50.0f, 50.0f, 1, 1) {}

void SceneBasic_Uniform::initScene()
{
	glEnable(GL_DEPTH_TEST);
	// Camera View
	vec3 cameraPos(0.0f, 4.0f, 6.0f);
	vec3 cameraTarget(0.0f, 0.2f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);

    compile();
	model = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	projection = glm::perspective(glm::radians(70.0f), (float)width / height, 0.3f, 100.0f);

	// Light properties
	prog.setUniform("NumLights", 2); // Number of lights

	prog.setUniform("Lights[0].L", vec3(0.5f)); // Light intensity
	prog.setUniform("Lights[0].La", vec3(0.6f)); // Ambient light intensity
	prog.setUniform("Lights[0].Ld", vec3(0.9f)); // Diffuse light intensity

	prog.setUniform("Lights[1].L", vec3(0.6f)); // Light intensity
	prog.setUniform("Lights[1].La", vec3(0.6f)); // Ambient light intensity
	prog.setUniform("Lights[1].Ld", vec3(0.9f)); // Diffuse light intensity

	// Load Textures
	// https://cc0-textures.com/
	floorTexture = Texture::loadTexture("media/texture/asphalt_01_diff_4k.jpg");
	wallTexture = Texture::loadTexture("media/texture/broken_wall_diff_4k.jpg");

	damageTexture = Texture::loadTexture("media/texture/Damage.png");
	normalTexture = Texture::loadTexture("media/texture/asphalt_01_nor_gl_4k.jpg");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, damageTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normalTexture);
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
	vec4 lightPos = vec4(-15.0f, 4.0f, -12.0f, 1.0f); 
	vec4 lightPos2 = vec4(15.0f, 6.0f, 12.0f, 1.0f);
	prog.setUniform("Lights[0].Position", view * lightPos);
	prog.setUniform("Lights[1].Position", view * lightPos2);

	// Set material properties
	vec3 diffuseColor = vec3(0.5f, 0.0f, 0.0f); 
	vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambientColor = vec3(0.2f, 0.0f, 0.0f); 

	prog.setUniform("Material.Kd", diffuseColor);
	prog.setUniform("Material.Ks", specularColor);
	prog.setUniform("Material.Ka", ambientColor);
	prog.setUniform("Material.Shininess", 100.0f);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	model = mat4(1.0f);
	model = glm::scale(model, vec3(0.4f, 1.0f, 0.6f));
	setMatrices();
	plane.render();

	
	glBindTexture(GL_TEXTURE_2D, wallTexture); // Using floor binding for walls

	// Back Wall
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 4.0f, -15.0f));
	model = glm::rotate(model, glm::radians(90.0f), vec3(1, 0, 0));
	model = glm::scale(model, vec3(0.4f, 1.0f, 0.16f));
	setMatrices();
	plane.render();


	// Front Wall
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 4.0f, 15.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1, 0, 0));
	model = glm::scale(model, vec3(0.4f, 1.0f, 0.16f));
	setMatrices();
	plane.render();


	// Left Wall
	model = mat4(1.0f);
	model = glm::translate(model, vec3(-10.0f, 4.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(90.0f), vec3(0, 1, 0));// Flip to face inward
	model = glm::scale(model, vec3(0.6f, 1.0f, 0.16f));
	setMatrices();
	plane.render();


	// Right wall
	model = mat4(1.0f);
	model = glm::translate(model, vec3(10.0f, 4.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(0, 1, 0)); 
	model = glm::scale(model, vec3(0.6f, 1.0f, 0.16f));
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
