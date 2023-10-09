#include "Body.h"

namespace Pyewton
{


Body::Body()
{
	appearance.model.loadModel(GetModelPath("sphere.obj").string().data());

	physics.position = glm::vec3(0, 0, 0);
	physics.velocity = glm::vec3(0, 0, 0);

	name = std::string("default name");
}

Body::Body(const char* name_)
{
	appearance.model.loadModel(GetModelPath("sphere.obj").string().data());

	physics.position = glm::vec3(0, 0, 0);
	physics.velocity = glm::vec3(0, 0, 0);

	name = std::string(name_);
}


void Body::setColor(glm::vec3 colour_)
{
	appearance.color = Color(colour_);
}


void Body::setColor(float r, float g, float b)
{
	appearance.color = Color(r, g, b);
}

void Body::setPos(glm::vec3 pos)
{
	physics.position = pos;
} 

void Body::setPos(float x, float y, float z)
{
	physics.position = glm::vec3(x, y, z);
}

void Body::setLight(Color c, float strenght)
{
	appearance.light.color = c;
	appearance.light.strength = strenght;
}

glm::vec3 Body::getPos()
{
	return physics.position;
}

void Body::Draw(Shader shader)
{
	shader.use();
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, physics.position); // translate it down so it's at the center of the scene
	modelMat = glm::scale(modelMat, glm::vec3(appearance.size));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", modelMat);
	shader.setVec3("objectColor", appearance.color);
	shader.setBool("isObjEmissive", appearance.isEmissive);
	shader.setVec3("objLightColor", appearance.light.color);
	shader.setFloat("objLightStrength", appearance.light.strength);
	appearance.model.Draw(shader);
}

void Body::UpdateOrbit()
{
	orbit.AppendPoint(physics.position);
}

void Body::ResetOrbit()
{
	orbit.Reset();
}

} // Namespace Pyewton