#ifndef SCENE_INFO_H
#define SCENE_INFO_H

#include "tinyxml2.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <iostream>

struct ModelObject
{
	std::string filename;
	bool display = false;
	bool normalInterpolation = false;
	int renderMethod = 0;

	glm::vec3 scale{ 1.0f };
	glm::vec3 rotation{ 0.0f };
	glm::vec3 translation{ 0.0f };
	float refractiveIndex = 1.0f;
	glm::vec4 albedo{ 0.0f };
	glm::vec3 diffuse{ 0.0f };
	float shiness = 0.0f;
};

struct CameraInfo
{
	int width = 256;
	int height = 192;
	std::string outputname;
};

struct SceneConfig
{
	int depthMax = 4;
	bool showEnvMap = false;
	CameraInfo camera;

	std::vector<ModelObject> models;

	bool spheresDisplay[4] = { false, false, false, false };
};

bool ReadSceneConfigFromXML(const std::string& filename, SceneConfig& config)
{
	using namespace tinyxml2;

	XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) return false;

	XMLElement* root = doc.FirstChildElement("SceneInfo");
	if (!root) return false;

	// DepthMax
	if (XMLElement* depth = root->FirstChildElement("DepthMax"))
	{
		XMLElement* args = depth->FirstChildElement("Args");
		if (args) args->QueryIntAttribute("DepthMax", &config.depthMax);
	}

	// EnvironmentMap
	if (XMLElement* env = root->FirstChildElement("EnvironmentMap"))
	{
		XMLElement* args = env->FirstChildElement("Args");
		int display = 0;
		if (args && args->QueryIntAttribute("display", &display) == XML_SUCCESS)
		{
			config.showEnvMap = (display != 0);
		}
	}

	// Camera
	if (XMLElement* cam = root->FirstChildElement("Camera"))
	{
		XMLElement* args = cam->FirstChildElement("Args");
		if (args)
		{
			args->QueryIntAttribute("width", &config.camera.width);
			args->QueryIntAttribute("height", &config.camera.height);
			const char* output = args->Attribute("outputname");
			if (output) config.camera.outputname = output;
		}
	}

	// SpheresDisplay
	if (XMLElement* spheres = root->FirstChildElement("SpheresDisplay"))
	{
		XMLElement* args = spheres->FirstChildElement("Args");
		if (args)
		{
			for (int i = 0; i < 4; ++i)
			{
				std::string attr = "sphere" + std::to_string(i + 1);
				int val = 0;
				args->QueryIntAttribute(attr.c_str(), &val);
				config.spheresDisplay[i] = (val != 0);
			}
		}
	}

	// Lambda for parsing a model
	auto parseModel = [](XMLElement* modelElem) -> ModelObject
		{
			ModelObject model;

			if (XMLElement* args = modelElem->FirstChildElement("Args"))
			{
				const char* file = args->Attribute("filename");
				if (file) model.filename = file;
				int display = 0;
				args->QueryIntAttribute("display", &display);
				model.display = (display != 0);
				args->QueryBoolAttribute("normalinterpolation", &model.normalInterpolation);
				args->QueryIntAttribute("rendermethod", &model.renderMethod);
			}

			auto getVec3 = [](XMLElement* e, glm::vec3& v)
				{
					if (e)
					{
						e->QueryFloatAttribute("x", &v.x);
						e->QueryFloatAttribute("y", &v.y);
						e->QueryFloatAttribute("z", &v.z);
					}
				};

			if (XMLElement* scale = modelElem->FirstChildElement("Scale")) getVec3(scale, model.scale);
			if (XMLElement* rot = modelElem->FirstChildElement("Rotation")) getVec3(rot, model.rotation);
			if (XMLElement* trans = modelElem->FirstChildElement("Translation")) getVec3(trans, model.translation);

			if (XMLElement* refr = modelElem->FirstChildElement("RefractiveIndex"))
				refr->QueryFloatAttribute("x", &model.refractiveIndex);

			if (XMLElement* albedo = modelElem->FirstChildElement("Albedo"))
			{
				albedo->QueryFloatAttribute("x", &model.albedo.x);
				albedo->QueryFloatAttribute("y", &model.albedo.y);
				albedo->QueryFloatAttribute("z", &model.albedo.z);
				albedo->QueryFloatAttribute("w", &model.albedo.w);
			}

			if (XMLElement* diff = modelElem->FirstChildElement("Diffuse"))
			{
				diff->QueryFloatAttribute("r", &model.diffuse.r);
				diff->QueryFloatAttribute("g", &model.diffuse.g);
				diff->QueryFloatAttribute("b", &model.diffuse.b);
			}

			if (XMLElement* sh = modelElem->FirstChildElement("Shiness"))
			{
				sh->QueryFloatAttribute("p", &model.shiness);
			}

			return model;
		};

	// Parse all models
	const char* modelNames[] = { "Duck", "Asschercut", "Bunny", "Dragon", "Venus", "FudanLogo" };
	for (const char* name : modelNames)
	{
		if (XMLElement* elem = root->FirstChildElement(name))
		{
			config.models.push_back(parseModel(elem));
		}
	}

	return true;
}

#endif // !SCENE_INFO_H

