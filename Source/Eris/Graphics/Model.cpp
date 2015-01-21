//
// Copyright (c) 2008-2014 the Eris project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Graphics.h"
#include "Model.h"

#include "Core/Log.h"
#include "Core/Profiler.h"
#include "Memory/ArrayPointers.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Eris
{


    Model::Model(Context* context) :
        Resource(context)
    {
    }

    bool Model::load(Deserializer& deserializer)
    {
        PROFILE(LoadModel);

        std::size_t ds_size = deserializer.getSize();

        SharedArrayPtr<char> buffer = SharedArrayPtr<char>(ds_size + 1);
        std::size_t in_size = deserializer.read(buffer.get(), ds_size);

        if (in_size != ds_size)
            return false;

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFileFromMemory(buffer.get(), ds_size, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FixInfacingNormals | aiProcess_GenUVCoords | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);

        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Log::errorf("Failed loading Model: %s", importer.GetErrorString());
            return false;
        }

        if (!scene->HasMeshes())
        {
            Log::errorf("Failed loading Model: no meshes");
            return false;
        }

        for (auto i = 0U; i < scene->mNumMeshes; i++)
        {
            aiMesh* aimesh = scene->mMeshes[i];
            std::vector<Vertex> vertices;
            for (auto j = 0U; j < aimesh->mNumVertices; j++)
            {
                Vertex vertex;

                vertex.position.x = aimesh->mVertices[j].x;
                vertex.position.y = aimesh->mVertices[j].y;
                vertex.position.z = aimesh->mVertices[j].z;

                vertex.normal.x = aimesh->mNormals[j].x;
                vertex.normal.y = aimesh->mNormals[j].y;
                vertex.normal.z = aimesh->mNormals[j].z;

                if (aimesh->HasTextureCoords(0))
                {
                    vertex.texcoords.x = aimesh->mTextureCoords[0][j].x;
                    vertex.texcoords.y = aimesh->mTextureCoords[0][j].y;
                }
                else
                    vertex.texcoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }

            std::vector<glm::u32> indices;
            for (auto j = 0U; j < aimesh->mNumFaces; j++)
            {
                aiFace face = aimesh->mFaces[j];
                for (auto k = 0U; k < face.mNumIndices; k++)
                    indices.push_back(face.mIndices[k]);
            }

            SharedPtr<Mesh> mesh = SharedPtr<Mesh>(new Mesh(m_context));
            mesh->setVertices(vertices);
            mesh->setIndices(indices);
            m_meshes.push_back(mesh);
        }

        compile();

        return true;
    }

    bool Model::save(Serializer& serializer)
    {
        return true;
    }

    void Model::draw() const
    {
        for (auto mesh : m_meshes)
            mesh->draw();
    }

    void Model::compile()
    {
        GLFWwindow *win = glfwGetCurrentContext();
        Graphics* graphics = m_context->getModule<Graphics>();
        glfwMakeContextCurrent(win ? win : graphics->getResourceWindow());

        for (auto mesh : m_meshes)
            mesh->compile();
        
        glfwMakeContextCurrent(win);
    }
}