#include "project/assets/texture_asset.hpp"

#include "project/resource_manager.hpp"

namespace g::project::assets
{

texture::texture(std::string_view name)
    : asset { name, asset_type::texture }
{
}

texture::~texture() { }

std::shared_ptr<texture> texture::create(std::string_view name)
{
    auto resource = std::shared_ptr<texture>(new texture(name));

    resource_manager::get()->register_resource(resource);
    return resource;
}

void texture::set_size(std::array<unsigned, 2> size) { _size = size; }

std::array<unsigned, 2> texture::size() const { return _size; }

void texture::set_format(std::array<char, 4> format) { _format = format; }

std::array<char, 4> const& texture::format() const { return _format; }

void texture::set_data(std::vector<char> data) { _data = std::move(data); }

std::vector<char> const& texture::data() const { return _data; }

} // namespace g::project::assets
