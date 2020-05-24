#include "ModelLoader.h"
#include <string>
#include <iostream>

void ModelLoader::GetUntil(std::ifstream& fileStream, std::string& character)
{
    fileStream >> character;
    while (character != "v" && character != "vt" && character != "vn" && character != "f")
    {
        fileStream >> character;
    }
}

void ModelLoader::SplitToArray(std::string& source, std::string arr[])
{
    std::string tempStr;
    int it = 0;
    for (char c : source)
    {
        if (c == '/')
        {
            arr[it] = std::string(tempStr);
            it++;
            tempStr = "";
        }
        else
        {
            tempStr += c;
        }
    }
    arr[2] = std::string(tempStr);
}

MeshData ModelLoader::LoadObj(const char* path)
{
    float minX = 0.0f, maxX = 0.0f;
    float minY = 0.0f, maxY = 0.0f;
    float minZ = 0.0f, maxZ = 0.0f;

    std::string str;
    std::ifstream file(path);
    int lineNumber = 0;
    if (file.is_open())
    {
       // std::cout << "---Loading object---\n";
        std::string character;
        GetUntil(file, character);
        MeshData meshData;

       // std::cout << "\n Start character: " << character << "\n";

        //std::cout << "\t:: Vertices ::\n";
        while (character == "v")
        {
         //   std::cout << "Start character: " << character << " line: " << lineNumber << "\n";
            std::string x, y, z;

            file >> x >> y >> z;

            float floatX = stof(x);
            float floatY = stof(y);
            float floatZ = stof(z);

            meshData.vertices.push_back(floatX);
            meshData.vertices.push_back(floatY);
            meshData.vertices.push_back(floatZ);
           // std::cout << "x: " << stof(x) << " y: " << stof(y) << " z: " << stof(z) << "\n";

            if (floatX < minX)
                minX = floatX;
            else if (floatX > maxX)
                maxX = floatX;

            if (floatY < minY)
                minY = floatY;
            else if (floatY > maxY)
                maxY = floatY;

            if (floatZ < minZ)
                minZ = floatZ;
            else if (floatZ > maxZ)
                maxZ = floatZ;

            file >> character;
            lineNumber++;
        }
        //std::cout << "\t:: UVs ::\n";
        while (character == "vt")
        {
         //   std::cout << "Start character: " << character << " line: " << lineNumber << "\n";
            std::string x, y;

            file >> x >> y;
            meshData.uvs.push_back(stof(x));
            meshData.uvs.push_back(stof(y));
           // std::cout << "x: " << stof(x) << " y: " << stof(y) << "\n";

            file >> character;
            lineNumber++;
        }
       // std::cout << "\t:: Normals ::\n";
        while (character == "vn")
        {
          //  std::cout << "Start character: " << character << " line: " << lineNumber << "\n";
            std::string x, y, z;

            file >> x >> y >> z;
            meshData.normals.push_back(stof(x));
            meshData.normals.push_back(stof(y));
            meshData.normals.push_back(stof(z));
           // std::cout << "x: " << stof(x) << " y: " << stof(y) << " z: " << stof(z) << "\n";

            file >> character;
            lineNumber++;
        }
        GetUntil(file, character);
       // std::cout << "\t:: Faces ::\n";
        while (character == "f")
        {
          /*  std::cout << "Start character: " << character << " line: " << lineNumber << "\n";
            char x, y, z,    x2, y2, z2,    x3, y3, z3;
            char divider;

            file >> x >>divider>> y >>divider>> z    >> x2 >>divider>> y2 >>divider>> z2    >> x3 >>divider>> y3 >>divider>> z3;
            std::cout << "face: "<<  x <<divider<< y <<divider<< z << " "    << x2 <<divider<< y2 <<divider<< z2 << " "    << x3 <<divider<< y3 <<divider<< z3 << "\n";
           
            indices.push_back(x - 1- '0');
            indices.push_back(x2 - 1- '0');
            indices.push_back(x3 -1- '0');*/

            std::string str1;
            file >> str1;
          //  std::cout << "\n" << "GEt char1: " <<str1;
            
            std::string str2;
            file >> str2;
          //  std::cout << "\n" << "GEt char2: " <<str2;

            std::string str3;
            file >> str3;
           // std::cout << "\n" << "GEt char3: " << str3 << "\n";

            std::string first[3];
            SplitToArray(str1, first);
            meshData.faces.push_back(stoi(first[0])-1);
            meshData.faces.push_back(stoi(first[1])-1);
            meshData.faces.push_back(stoi(first[2])-1);
            
            std::string second[3];
            SplitToArray(str2, second);
            meshData.faces.push_back(stoi(second[0])-1);
            meshData.faces.push_back(stoi(second[1])-1);
            meshData.faces.push_back(stoi(second[2])-1);

            std::string third[3];
            SplitToArray(str3, third);
            meshData.faces.push_back(stoi(third[0])-1);
            meshData.faces.push_back(stoi(third[1])-1);
            meshData.faces.push_back(stoi(third[2])-1);

          //  std::cout << "\t" << "final 0: " << first[0] << "\n";
          //  std::cout << "\t" << "final 0: " << second[0] << "\n";
           // std::cout << "\t" << "final 0: " << third[0] << "\n";

            int index1 = stoi(first[0]) - 1;
            int index2 = stoi(second[0]) - 1;
            int index3 = stoi(third[0]) - 1;


           // std::cout << "Pushed indices: " << index1 << " | " << index2 << " | " << index3 << "\n";
           // std::cout << "Pushed indices: " << (x - 1 - '0') << " | " << (x2 - 1 - '0') << " | " << (x3 - 1 - '0') << "\n";

            meshData.indices.push_back(index1);
            meshData.indices.push_back(index2);
            meshData.indices.push_back(index3);

            file >> character;
            if (file.eof())
                break;
            lineNumber++;
        }

        meshData.bounds = Bounds(minX, maxX, minY, maxY, minZ, maxZ);
        file.close();

        OrganizeVertices(meshData);
        return std::move(meshData);
    }
    else
    {
        std::cout << "Can't find file!\n";
    }

    return MeshData();
}

void ModelLoader::OrganizeVertices(MeshData& meshData)
{
    std::vector<float> tempVerts;

    //std::cout << "FACES:" << faces.size() << " \n";
    for (size_t i = 0, in = 0; i < meshData.faces.size() / 9; i++, in += 9)
    {
        //std::cout << "in: " << in << "\n";
        //std::cout << "Face1: "  << faces[in] << " | " << vertices[faces[in]*3] << " " << vertices[faces[in]*3+1] << " " << vertices[faces[in]*3+2];  std::cout << " Uvs1: " << uvs[faces[in + 1] * 2] << " " << uvs[faces[in + 1] * 2 + 1] << "\n";
        //std::cout << "Face2: " << faces[in+3] << " | " << vertices[faces[in+3] *3] << " " << vertices[faces[in+3]*3 + 1] << " " << vertices[faces[in+3]*3 + 2]; std::cout << " Uvs2: " << uvs[faces[in + 4] * 2] << " " << uvs[faces[in + 4] * 2 + 1] << "\n";
        //std::cout << "Face3: " << faces[in+6] << " | " << vertices[faces[in + 6] * 3] << " " << vertices[faces[in + 6] * 3 + 1] << " " << vertices[faces[in + 6] * 3 + 2]; std::cout << " Uvs3: " << uvs[faces[in + 7] * 2] << " " << uvs[faces[in + 7] * 2 + 1] << "\n";

        tempVerts.push_back(meshData.vertices[meshData.faces[in] * 3]);      tempVerts.push_back(meshData.vertices[meshData.faces[in] * 3 + 1]);       tempVerts.push_back(meshData.vertices[meshData.faces[in] * 3 + 2]);
        tempVerts.push_back(meshData.uvs[meshData.faces[in + 1] * 2]);       tempVerts.push_back(meshData.uvs[meshData.faces[in + 1] * 2 + 1]);

        tempVerts.push_back(meshData.vertices[meshData.faces[in + 3] * 3]);  tempVerts.push_back(meshData.vertices[meshData.faces[in + 3] * 3 + 1]);   tempVerts.push_back(meshData.vertices[meshData.faces[in + 3] * 3 + 2]);
        tempVerts.push_back(meshData.uvs[meshData.faces[in + 4] * 2]);       tempVerts.push_back(meshData.uvs[meshData.faces[in + 4] * 2 + 1]);

        tempVerts.push_back(meshData.vertices[meshData.faces[in + 6] * 3]);  tempVerts.push_back(meshData.vertices[meshData.faces[in + 6] * 3 + 1]);   tempVerts.push_back(meshData.vertices[meshData.faces[in + 6] * 3 + 2]);
        tempVerts.push_back(meshData.uvs[meshData.faces[in + 7] * 2]);       tempVerts.push_back(meshData.uvs[meshData.faces[in + 7] * 2 + 1]);
    }

    meshData.vertices = std::move(tempVerts);
}