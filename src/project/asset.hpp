#pragma once

#include <string>
#include <vector>

#include "project/resource.hpp"

namespace g::project
{

// enum class asset_type
// {
//     FBX,
//     PNG,
//     JPG,
//     JPEG,
//     TGA,
//     BMP,
//     DDS,
//     WAV,
//     MP3,
//     OGG,
//     FLAC,
//     SHADER,
//     MATERIAL,
//     TEXTURE,
//     FONT,
//     SCENE,
//     PREFAB,
//     SCRIPT,
//     UNKNOWN
// };

enum class asset_type
{
    unknown,
    mesh,
    texture,
    material,
    shader,
};

asset_type asset_type_from_string_view(std::string_view str);

class asset : public resource
{
public:
    asset(std::string_view name, asset_type type);
    virtual ~asset();

    std::string name() const;
    asset_type asset_type() const;
    std::vector<char> const& data() const;

private:
    std::string _name;
    enum class asset_type _asset_type;
};

template <>
struct resource_type_traits<asset>
{
    static constexpr resource_type type = resource_type::asset;
};

} // namespace g::project
