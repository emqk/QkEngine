#include "Serialization.h"
#include <iostream>
#include <fstream>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <rapidjson/writer.h>
#include <fstream>
#include<fstream>
#include<sstream>
#include<string>
#include "../Scene.h"

using namespace rapidjson;

void Serializer::Serialize()
{
    // 1. Parse a JSON string into DOM.
    const char* na = "from c++";
    Document d(kObjectType);
    Document::AllocatorType& allocator = d.GetAllocator();

    glm::vec3 pos = Scene::GetCurrentScene().GetCamera().GetLocalPosition();
    Value o(kObjectType);
    o.AddMember("x", pos.x, allocator);
    o.AddMember("y", pos.y, allocator);
    o.AddMember("z", pos.z, allocator);
    
    d.AddMember("Camera", o, allocator);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    std::fstream file("Project.json", std::ios::out | std::ios::trunc);
    if (file.is_open())
    {
        std::cout << "OPENED!\n";
        file << buffer.GetString();
        file.close();
    }
}

void Serializer::Deserialize()
{
    std::ifstream ifs("Project.json", std::ios::in);
    if (ifs.is_open())
    {
        std::ostringstream ss;
        ss << ifs.rdbuf(); // reading data

        Document d;
        d.Parse(ss.str().c_str());

        std::cout << "READ: " << d.HasMember("Camera") << std::endl;
        if (d.HasMember("Camera"))
        {
            std::cout << "IS OBJ: " << d["Camera"].IsObject() << std::endl;
            if (d["Camera"].IsObject())
            {
               auto a = d["Camera"].GetObject();
               std::cout << "Try get " << a["x"].GetDouble() << std::endl;
               std::cout << "Try get " << a["y"].GetDouble() << std::endl;
               std::cout << "Try get " << a["z"].GetDouble() << std::endl;
               Scene::GetCurrentScene().GetCamera().SetLocalPosition(glm::vec3(a["x"].GetDouble(), a["y"].GetDouble(), a["z"].GetDouble()));
            }
        }
    }
    

}
