#pragma once
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <rapidjson/writer.h>
#include<fstream>
#include<sstream>
#include<string>
#include <glm\fwd.hpp>

using namespace rapidjson;

class Serializer
{

public:
	Serializer() = delete;
	~Serializer() = delete;

	static void Serialize(std::string fileName);
	static void Deserialize(std::string fileName);

private:
	static void SerializeVec3(const char* name, const glm::vec3& vec, Value& val, Document::AllocatorType& allocator);
	static glm::vec3 DeserializeVec3(const GenericObject<true, Value>& obj);
	static glm::vec3 DeserializeVec3(const GenericObject<false, Value>& obj);

	static void SerializeVec4(const char* name, const glm::vec4& vec, Value& saveTo, Document::AllocatorType& allocator);
	static glm::vec4 DeserializeVec4(const GenericObject<true, Value>& obj);
	static glm::vec4 DeserializeVec4(const GenericObject<false, Value>& obj);
};