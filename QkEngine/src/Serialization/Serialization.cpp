#include "Serialization.h"
#include <iostream>
#include "../Scene.h"

void Serializer::Serialize()
{
    // 1. Parse a JSON string into DOM.
    Document d(kObjectType);
    Document::AllocatorType& allocator = d.GetAllocator();
    
    Value array(kArrayType);
    for (const std::unique_ptr<GameObject>& obj : Scene::GetCurrentScene().objects)
    {
        glm::vec3 pos = obj->transform.GetLocalPosition();
        glm::quat rot = obj->transform.GetLocalRotation();
        glm::vec3 scale = obj->transform.GetLocalScale();
        Value object(kObjectType);

        Value posObj(kObjectType);
        SerializeVec3(pos, posObj, allocator);
        object.AddMember("Position", posObj, allocator);
        Value rotObj(kObjectType);
        SerializeVec3(glm::vec3(rot.x, rot.y, rot.z), rotObj, allocator);
        object.AddMember("Rotation", rotObj, allocator);
        Value scaleObj(kObjectType);
        SerializeVec3(scale, scaleObj, allocator);
        object.AddMember("Scale", scaleObj, allocator);

        array.PushBack(object, allocator);
    }

    d.AddMember("Objects", array, allocator);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    std::fstream file("Project.json", std::ios::out | std::ios::trunc);
    if (file.is_open())
    {
        std::cout << "Saving!\n";
        file << buffer.GetString();
        file.close();
    }
}

void Serializer::Deserialize()
{
    std::ifstream ifs("Project.json", std::ios::in);
    if (ifs.is_open())
    {
        std::cout << "Loading\n";
        /*for (const std::unique_ptr<GameObject>& obj : Scene::GetCurrentScene().objects)
        {
            Scene::GetCurrentScene().Destroy(obj.get());
        }*/

        std::ostringstream ss;
        ss << ifs.rdbuf(); // reading data

        Document d;
        d.Parse(ss.str().c_str());

        std::cout << "READ: " << d.HasMember("Objects") << std::endl;
        if (d.HasMember("Objects"))
        {
            std::cout << "IS Arr: " << d["Objects"].IsArray() << std::endl;
            if (d["Objects"].IsArray())
            {
                int i = 0;
                for (Value::ConstValueIterator itr = d["Objects"].Begin(); itr != d["Objects"].End(); ++itr)
                {
                    auto gameObj = itr->GetObject();

                    //Transform START
                    //Position
                    glm::vec3 pos = DeserializeVec3(gameObj["Position"].GetObject());
                    Scene::GetCurrentScene().objects[i]->transform.SetLocalPosition(pos);

                    //Rotation
                    glm::vec3 rot = DeserializeVec3(gameObj["Rotation"].GetObject());
                    Scene::GetCurrentScene().objects[i]->transform.SetLocalRotation(glm::quat(1, rot.x, rot.y, rot.z));

                    //Scale
                    glm::vec3 scale = DeserializeVec3(gameObj["Scale"].GetObject());
                    Scene::GetCurrentScene().objects[i]->transform.SetLocalScale(scale);
                    //Transform END


                    i++;
                }
            }
        }
    }
    

}

void Serializer::SerializeVec3(const glm::vec3& vec, Value& val, Document::AllocatorType& allocator)
{
    val.AddMember("x", vec.x, allocator);
    val.AddMember("y", vec.y, allocator);
    val.AddMember("z", vec.z, allocator);
}

glm::vec3 Serializer::DeserializeVec3(const GenericObject<true, Value>& obj)
{
    return glm::vec3(obj["x"].GetDouble(), obj["y"].GetDouble(), obj["z"].GetDouble());
}
