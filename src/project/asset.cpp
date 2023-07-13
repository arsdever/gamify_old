#include <filesystem>

#include "project/asset.hpp"

namespace g::project
{

// asset_type asset_type_from_string_view(std::string_view typestr)
// {
//     std::transform(typestr.begin(), typestr.end(), typestr.begin(),
//     ::tolower); if (typestr == "fbx")
//         type = asset_type::FBX;
//     else if (typestr == "png")
//         type = asset_type::PNG;
//     else if (typestr == "jpg")
//         type = asset_type::JPG;
//     else if (typestr == "jpeg")
//         type = asset_type::JPEG;
//     else if (typestr == "tga")
//         type = asset_type::TGA;
//     else if (typestr == "bmp")
//         type = asset_type::BMP;
//     else if (typestr == "dds")
//         type = asset_type::DDS;
//     else if (typestr == "wav")
//         type = asset_type::WAV;
//     else if (typestr == "mp3")
//         type = asset_type::MP3;
//     else if (typestr == "ogg")
//         type = asset_type::OGG;
//     else if (typestr == "flac")
//         type = asset_type::FLAC;
//     else if (typestr == "glsl")
//         type = asset_type::SHADER;
//     else if (typestr == "material")
//         type = asset_type::MATERIAL;
//     else if (typestr == "texture")
//         type = asset_type::TEXTURE;
//     else if (typestr == "font")
//         type = asset_type::FONT;
//     else if (typestr == "scene")
//         type = asset_type::SCENE;
//     else if (typestr == "prefab")
//         type = asset_type::PREFAB;
//     else if (typestr == "script")
//         type = asset_type::SCRIPT;
// }

asset::asset(std::string_view name, enum class asset_type type)
    : resource(resource_type::asset)
    , _name(name)
    , _asset_type(type)
{
}

asset::~asset() = default;

std::string asset::name() const { return _name; }

asset_type asset::asset_type() const { return _asset_type; }

} // namespace g::project
