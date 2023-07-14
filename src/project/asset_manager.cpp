#include "project/asset_manager.hpp"

#include "common/logger.hpp"
#include "project/assets/mesh_asset.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace g::project
{

auto logger = common::get_logger("project");

asset_manager::asset_manager() = default;

asset_manager::~asset_manager() = default;

std::shared_ptr<asset> asset_manager::load_asset(std::string_view file_path)
{
    auto importer = Assimp::Importer();
    const aiScene* scene = importer.ReadFile(file_path.data(), 0);

    if (!scene)
    {
        logger->error("Failed to load asset: {}", importer.GetErrorString());
        return nullptr;
    }

    for (int i = 0; i < scene->mNumMeshes; ++i)
    {
        auto mesh = scene->mMeshes[ i ];
        assets::mesh::vertices_t vertices;
        assets::mesh::indices_t indices;

        for (int j = 0; j < mesh->mNumVertices; ++j)
        {
            vertices.push_back({
                { mesh->mVertices[ j ].x,
                  mesh->mVertices[ j ].y,
                  mesh->mVertices[ j ].z },
                { mesh->mNormals[ j ].x,
                  mesh->mNormals[ j ].y,
                  mesh->mNormals[ j ].z },
                { mesh->mTextureCoords[ 0 ][ j ].x,
                  mesh->mTextureCoords[ 0 ][ j ].y },
            });
        }

        for (int j = 0; j < mesh->mNumFaces; ++j)
        {
            auto face = mesh->mFaces[ j ];
            for (int k = 0; k < face.mNumIndices; ++k)
            {
                indices.push_back(face.mIndices[ k ]);
            }
        }

        return assets::mesh::create(
            mesh->mName.C_Str(), std::move(vertices), std::move(indices));
    }

    return nullptr;
}

void asset_manager::create_asset(std::string_view file_path) { }

} // namespace g::project
