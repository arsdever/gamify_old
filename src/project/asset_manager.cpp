#include <fstream>

#include "project/asset_manager.hpp"

#include "common/logger.hpp"
#include "project/assets/material_asset.hpp"
#include "project/assets/mesh_asset.hpp"
#include "project/assets/shader_asset.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace g::project
{

auto logger = common::get_logger("project");

asset_manager::asset_manager() = default;

asset_manager::~asset_manager() = default;

std::unordered_map<std::string, std::shared_ptr<asset>>
asset_manager::load_asset(std::string_view file_path)
{
    auto importer = Assimp::Importer();
    const aiScene* scene = importer.ReadFile(
        file_path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene)
    {
        logger->error("Failed to load asset: {}", importer.GetErrorString());
        return {};
    }

    std::unordered_map<std::string, std::shared_ptr<asset>> result;
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

        result.emplace("mesh",
                       assets::mesh::create(mesh->mName.C_Str(),
                                            std::move(vertices),
                                            std::move(indices)));
    }

    logger->info("Loading materials: number of materials present {}",
                 scene->mNumMaterials);
    for (int i = 0; i < scene->mNumMaterials; ++i)
    {
        logger->info("Loading material {}", i);
        auto material = scene->mMaterials[ i ];
        aiString name;
        material->Get(AI_MATKEY_NAME, name);
        logger->info("Material name: {}", name.C_Str());
        aiColor3D color;
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        common::color diffuse_color { color.r, color.g, color.b, 1.0f };
        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        common::color ambient_color { color.r, color.g, color.b, 1.0f };
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        common::color specular_color { color.r, color.g, color.b, 1.0f };
        material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        common::color emissive_color { color.r, color.g, color.b, 1.0f };
        material->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
        common::color transparent_color { color.r, color.g, color.b, 1.0f };
        material->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
        common::color reflective_color { color.r, color.g, color.b, 1.0f };

        auto material_asset = assets::material::create(name.C_Str());
        material_asset->set_property("diffuse_color", diffuse_color);
        material_asset->set_property("ambient_color", ambient_color);
        material_asset->set_property("specular_color", specular_color);
        material_asset->set_property("emissive_color", emissive_color);
        material_asset->set_property("transparent_color", transparent_color);
        material_asset->set_property("reflective_color", reflective_color);

        std::ifstream default_vertex_shader_file("default.vert");
        std::string default_vertex_shader_source(
            (std::istreambuf_iterator<char>(default_vertex_shader_file)),
            std::istreambuf_iterator<char>());
        std::ifstream default_fragment_shader_file("default.frag");
        std::string default_fragment_shader_source(
            (std::istreambuf_iterator<char>(default_fragment_shader_file)),
            std::istreambuf_iterator<char>());

        auto shader_asset = std::static_pointer_cast<assets::shader>(
            assets::shader::create("default"));
        shader_asset->set_vertex_shader_source(default_vertex_shader_source);
        shader_asset->set_fragment_shader_source(
            default_fragment_shader_source);

        material_asset->set_shader(shader_asset);

        result.emplace("material", material_asset);
        result.emplace("shader", shader_asset);
    }

    return result;
}

void asset_manager::create_asset(std::string_view file_path) { }

} // namespace g::project
