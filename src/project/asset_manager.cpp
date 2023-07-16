#include <fstream>
#include <png.h>

#include "project/asset_manager.hpp"

#include "common/logger.hpp"
#include "project/assets/material_asset.hpp"
#include "project/assets/mesh_asset.hpp"
#include "project/assets/shader_asset.hpp"
#include "project/assets/texture_asset.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace g::project
{

auto logger = common::get_logger("project");

namespace helper
{
std::shared_ptr<assets::texture> load_texture(aiTexture* texture)
{
    logger->info("Loading texture: {}", texture->mFilename.C_Str());

    auto texture_asset = assets::texture::create(texture->mFilename.C_Str());
    texture_asset->set_size({ texture->mWidth, texture->mHeight });
    std::vector<char> data;
    data.resize(texture->mWidth * texture->mHeight * 4);
    std::memcpy(data.data(), texture->pcData, data.size());

    texture_asset->set_data(std::move(data));
    return texture_asset;
}
} // namespace helper

asset_manager::asset_manager() = default;

asset_manager::~asset_manager() = default;

std::unordered_map<std::string, std::shared_ptr<asset>>
asset_manager::load_asset(std::string_view file_path)
{
    auto importer = Assimp::Importer();
    const aiScene* scene = importer.ReadFile(
        file_path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
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

        aiString texture_path;
        material->GetTexture(
            aiTextureType::aiTextureType_DIFFUSE, 0, &texture_path);
        logger->info("Texture path: {}", texture_path.C_Str());
        auto texture_asset = load_texture(texture_path.C_Str());
        if (!texture_asset)
        {
            logger->error("Failed to load texture: {}", texture_path.C_Str());
            continue;
        }
        material_asset->set_property("texture_diffuse", texture_asset->uuid());
        result.emplace("texture", texture_asset);
    }

    for (int i = 0; i < scene->mNumTextures; ++i)
    {
        auto texture = scene->mTextures[ i ];
        result.emplace("texture", helper::load_texture(texture));
    }

    importer.FreeScene();
    return result;
}

std::shared_ptr<project::assets::texture>
asset_manager::load_texture(std::string_view file_path)
{
    FILE* fp = fopen(file_path.data(), "rb");
    png_structp png =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        logger->error("Failed to create png read struct");
        return nullptr;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        logger->error("Failed to create png info struct");
        return nullptr;
    }

    png_init_io(png, fp);
    png_read_info(png, info);
    png_uint_32 width = png_get_image_width(png, info);
    png_uint_32 height = png_get_image_height(png, info);

    std::vector<unsigned char> bytes(width * height * 4);
    png_bytep row_pointers[ height ];
    for (int i = 0; i < height; i++)
    {
        row_pointers[ i ] = &bytes[ i * width * 4 ];
    }

    // Read the bytes from the PNG file into the vector.
    png_read_image(png, row_pointers);

    std::vector<char> pixels(bytes.begin(), bytes.end());

    auto texture_asset = assets::texture::create("default");
    texture_asset->set_size({ width, height });
    texture_asset->set_data(std::move(pixels));

    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    return texture_asset;
}

void asset_manager::create_asset(std::string_view file_path) { }

} // namespace g::project
