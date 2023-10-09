#include "Loader.h"

Pyewton::BodyList Pyewton::Json::LoadFile(char* path)
{

	Pyewton::BodyList bodyList;


	// Load and unserialize the file
	std::ifstream file(path);
	std::stringstream file_buffer;

	file_buffer << file.rdbuf();

	folly::dynamic file_parsed = folly::parseJson(file_buffer.str());

	// Load body after body
	folly::dynamic bodylist_json = file_parsed["bodylist"];

	for (auto& body_json : bodylist_json)
	{
		Pyewton::Body body;

		body.name = body_json["name"].asString();

		//Appearance
		glm::vec3 color;
		color.x = body_json["appearance"]["color"]["r"].asDouble();
		color.y = body_json["appearance"]["color"]["g"].asDouble();
		color.z = body_json["appearance"]["color"]["b"].asDouble();
		body.appearance.color = color;

		body.appearance.size = body_json["appearance"]["size"].asDouble();

		body.appearance.isEmissive = body_json["appearance"]["isEmissive"].asBool();

		glm::vec3 lightColor;
		lightColor.x = body_json["appearance"]["light"]["color"]["r"].asDouble();
		lightColor.y = body_json["appearance"]["light"]["color"]["g"].asDouble();
		lightColor.z = body_json["appearance"]["light"]["color"]["b"].asDouble();
		body.appearance.light.color = lightColor;

		body.appearance.light.strength = body_json["appearance"]["light"]["strength"].asDouble();

		


		//Physics
		{
			auto& physics = body.physics;
			glm::vec3 position;
			glm::vec3 velocity;

			position.x = body_json["physics"]["position"]["x"].asDouble();
			position.y = body_json["physics"]["position"]["y"].asDouble();
			position.z = body_json["physics"]["position"]["z"].asDouble();

			velocity.x = body_json["physics"]["velocity"]["x"].asDouble();
			velocity.y = body_json["physics"]["velocity"]["y"].asDouble();
			velocity.z = body_json["physics"]["velocity"]["z"].asDouble();

			physics.position = position;
			physics.velocity = velocity;

			physics.mass = body_json["physics"]["mass"].asDouble();
			physics.isAffected = body_json["physics"]["isAffected"].asBool();
			physics.affectOther = body_json["physics"]["affectOther"].asBool();
		}

		bodyList.push_back(body);
	}

	return bodyList;
}

void Pyewton::Json::SaveToFile(BodyList& bodylist, std::filesystem::path path)
{

	folly::dynamic file_obj = folly::dynamic::object();

	file_obj["bodylist"] = folly::dynamic::array();

	for (auto& body : bodylist)
	{
		folly::dynamic json_body = folly::dynamic::object();

		json_body["name"] = body.name;
		
		{ //Appearance
			json_body["appearance"] = folly::dynamic::object();

			json_body["appearance"]["color"] = folly::dynamic::object();
			auto color = body.appearance.color.getColor();
			json_body["appearance"]["color"]["r"] = color[0];
			json_body["appearance"]["color"]["g"] = color[1];
			json_body["appearance"]["color"]["b"] = color[2];

			json_body["appearance"]["size"] = body.appearance.size;

			json_body["appearance"]["isEmissive"] = body.appearance.isEmissive;

			//Light
			json_body["appearance"]["light"] = folly::dynamic::object();
			json_body["appearance"]["light"]["color"] = folly::dynamic::object();

			auto color_ = body.appearance.light.color.getColor();
			json_body["appearance"]["light"]["color"]["r"] = color_[0];
			json_body["appearance"]["light"]["color"]["g"] = color_[1];
			json_body["appearance"]["light"]["color"]["b"] = color_[2];

			json_body["appearance"]["light"]["strength"] = body.appearance.light.strength;

		}

		{ //Physics
			json_body["physics"] = folly::dynamic::object();
			json_body["physics"]["position"] = folly::dynamic::object();
			json_body["physics"]["velocity"] = folly::dynamic::object();

			auto& position = body.physics.position;
			json_body["physics"]["position"]["x"] = position.x;
			json_body["physics"]["position"]["y"] = position.y;
			json_body["physics"]["position"]["z"] = position.z;

			auto& velocity = body.physics.velocity;
			json_body["physics"]["velocity"]["x"] = velocity.x;
			json_body["physics"]["velocity"]["y"] = velocity.y;
			json_body["physics"]["velocity"]["z"] = velocity.z;

			json_body["physics"]["mass"] = body.physics.mass;

			json_body["physics"]["affectOther"] = body.physics.affectOther;

			json_body["physics"]["isAffected"] = body.physics.isAffected;
		}

		file_obj["bodylist"].push_back(json_body);
	}

	folly::json::serialization_opts opts;
	opts.pretty_formatting = true;

	std::string str = folly::json::serialize(file_obj, opts);
	std::cout << path << '\n';

	if (!std::filesystem::is_directory(path.parent_path()) || !std::filesystem::exists(path.parent_path())) { // Check if src folder exists
		std::filesystem::create_directory(path.parent_path()); // create src folder
	}

	std::ofstream outputFile(path);
	if (outputFile.is_open())
	{
		outputFile << str;
		outputFile.close();
	}
	else
	{
		ImGui::InsertNotification({ ImGuiToastType_Error, 3000, "Failed to open the file for writing !" });
	}

}
