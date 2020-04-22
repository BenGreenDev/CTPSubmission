#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <initializer_list>
#include <string>
#include <fstream>
#include <istream>
#include <d3d11.h>
#include <stdio.h>

class GenericOBJ
{
public:
    template <class V>
    static IndexedTriangleList<V> MakeOBJ(const char* Filename, bool IsRHCoordSys)
    {
        std::vector<V> output;
        std::vector<unsigned short> vertexIndicesFinal;
        UINT numberOfVertices = 0, numberOfNormals = 0, numberOfUVs = 0, numberOfFaces = 0;
        std::vector<V> outputDaMettere;
        std::vector< unsigned short> vertexIndices, uvIndices, normalIndices;
        std::vector<V> temp_vertices;
        std::vector<DirectX::XMFLOAT2> temp_uvs;
        std::vector<DirectX::XMFLOAT3> temp_normals;

        std::vector<DirectX::XMFLOAT3> outVertices;
        std::vector<DirectX::XMFLOAT3> outNormals;
        std::vector<DirectX::XMFLOAT2> outUvs;

        FILE* file;
        fopen_s(&file, Filename, "rb");
        if (file == NULL) {
            MessageBox(0, "NULL FILE", 0, 0);
        }
        while (1) {
            char lineHeader[128];

            int res = fscanf_s(file, "%s \n", lineHeader, sizeof(lineHeader));
            if (res == EOF)
                break;

            if (strcmp(lineHeader, "v") == 0) {
                V vertex;
                if (IsRHCoordSys)
                {
                    int a = fscanf_s(file, "%f %f %f \n", &vertex.pos.x, &vertex.pos.y, &vertex.pos.z);
                    vertex.pos.z = vertex.pos.z * -1.0f;
                }
                else
                {
                    int a = fscanf_s(file, "%f %f %f \n", &vertex.pos.x, &vertex.pos.y, &vertex.pos.z);
                }
                temp_vertices.push_back(vertex);
                numberOfVertices++;
            }
            else if (strcmp(lineHeader, "vt") == 0) {
                DirectX::XMFLOAT2 uv;
                if (IsRHCoordSys)
                {
                    fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
                    uv.y = 1.0f - uv.y;
                }
                else
                {
                    fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
                }
                temp_uvs.push_back(uv);
                numberOfUVs++;
            }
            else if (strcmp(lineHeader, "vn") == 0) {
                DirectX::XMFLOAT3 normals;
                if (IsRHCoordSys)
                {
                    fscanf_s(file, "%f %f %f \n", &normals.x, &normals.y, &normals.z);
                    normals.z = normals.z * -1.0f;
                }
                else
                {
                    fscanf_s(file, "%f %f %f \n", &normals.x, &normals.y, &normals.z);
                }
                temp_normals.push_back(normals);
                numberOfNormals++;
            }
            else if (strcmp(lineHeader, "f") == 0) {
                std::string vertex1, vertex2, vertex3;
                UINT vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf_s(file, "%i / %i / %i  %i / %i / %i  %i / %i / %i \n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                    &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                    &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

                if (matches != 9 && matches != 1) {
                    MessageBox(0, "File cant be read", 0, 0);
                    break;
                }
                else if(matches != 1)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                    numberOfFaces++;
                }
            }
        }

        numberOfFaces *= 3;
        int cont = 0;
        for (int i = 0; i < numberOfFaces; i++) {

            UINT vertexIndex = vertexIndices[i];
            UINT normalIndex = normalIndices[i];
            UINT uvIndex = uvIndices[i];
            V temp;
            temp.pos = temp_vertices[vertexIndex - 1].pos;
            //temp.n = temp_normals[normalIndex - 1];
            //temp.tc = temp_uvs[uvIndex - 1];

            output.push_back(temp);

            cont++;
        }
        for (int i = 0; i < numberOfVertices; i++) {
            vertexIndices[i]--; 
        }

        vertexIndicesFinal = vertexIndices;

        return{
            std::move(output),{vertexIndices}
        };

    }
};
