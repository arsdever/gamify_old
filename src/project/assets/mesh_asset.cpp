#include "project/assets/mesh_asset.hpp"

#include "project/resource_manager.hpp"

namespace g::project::assets
{

mesh::mesh(std::string_view name)
    : asset(name, asset_type::mesh)
{
}

mesh::~mesh() = default;

std::shared_ptr<mesh>
mesh::create(std::string_view name, vertices_t vertices, indices_t indices)
{
    auto resource = std::shared_ptr<mesh>(new mesh(name));
    resource->_vertices = std::move(vertices);
    resource->_indices = std::move(indices);

    resource_manager::get()->register_resource(resource);
    return resource;
}

mesh::vertices_t const& mesh::vertices() const { return _vertices; }

mesh::indices_t const& mesh::indices() const { return _indices; }

} // namespace g::project::assets
