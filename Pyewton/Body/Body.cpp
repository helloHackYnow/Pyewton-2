#include "Body.h"

using namespace std::filesystem;

Body::Body()
{
	path current_dir = current_path();
	path sphere_path("3dfiles/sphere.obj");
	path full = current_dir / sphere_path;
	model.loadModel(full.string());
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	name = std::string("default name");
}

Body::Body(const char* name_)
{
	path current_dir = current_path();
	path sphere_path("3dfiles/sphere.obj");
	path full = current_dir / sphere_path;
	model.loadModel(full.string());
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	name = std::string(name_);
}


void Body::setColor(glm::vec3 colour_)
{
	color = Color(colour_);
}


void Body::setColor(float r, float g, float b)
{
	color = Color(r, g, b);
}

void Body::setPos(glm::vec3 pos)
{
	position = pos;
} 

void Body::setPos(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void Body::setLight(Color c, float strenght)
{
	light.color = c;
	light.strength = strenght;
}

glm::vec3 Body::getPos()
{
	return position;
}

void Body::Draw(Shader shader)
{
	shader.use();
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, position); // translate it down so it's at the center of the scene
	modelMat = glm::scale(modelMat, glm::vec3(size));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", modelMat);
	shader.setVec3("objectColor", color);
	shader.setBool("isObjEmissive", isEmissive);
	shader.setVec3("objLightColor", light.color);
	shader.setFloat("objLightStrength", light.strength);
	model.Draw(shader);
}

void Body::UpdateOrbit()
{
	orbit.AppendPoint(position);
}

void Body::ResetOrbit()
{
	orbit.Reset();
}
