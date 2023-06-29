#include <filesystem>

#include "ui/asset.hpp"

#include "common/logger.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace g::ui
{

Asset::AssetContainerBase::AssetContainerBase(const void* data) noexcept
    : _data(data)
{
}

Asset::AssetContainerBase::AssetContainerBase(
    Asset::AssetContainerBase&& r) noexcept = default;

Asset::AssetContainerBase& Asset::AssetContainerBase::operator=(
    Asset::AssetContainerBase&& r) noexcept = default;

Asset::AssetContainerBase::~AssetContainerBase() = default;

template <>
struct Asset::AssetContainer<Asset::AssetType::FBX>
    : public Asset::AssetContainerBase
{
    AssetContainer(const aiScene* scene)
        : AssetContainerBase(scene)
    {
    }

    const aiScene* scene() const { return static_cast<const aiScene*>(_data); }

    AssetType type() const { return AssetType::FBX; }
};

auto logger = common::get_logger("project");

Asset::Asset(std::string_view file_path)
    : _file_path(file_path)
{
    auto importer = Assimp::Importer();
    const aiScene* scene = importer.ReadFile(
        file_path.data(),
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
            aiProcess_GenSmoothNormals);

    if (!scene)
    {
        logger->error("Failed to load asset: {}", importer.GetErrorString());
        return;
    }

    _container = std::make_unique<AssetContainer<AssetType::FBX>>(scene);
}

Asset::~Asset() = default;

std::string Asset::name() const
{
    return std::filesystem::path(_file_path).filename().string();
}

Asset::AssetType Asset::type() const
{
    if (_container)
        return _container->type();

    std::string extension =
        std::filesystem::path(_file_path).extension().string();
    std::transform(
        extension.begin(), extension.end(), extension.begin(), ::toupper);
    if (extension == ".FBX")
        return AssetType::FBX;
    else if (extension == ".PNG")
        return AssetType::PNG;

    return AssetType::UNKNOWN;
}

} // namespace g::ui
