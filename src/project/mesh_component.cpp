#include "project/mesh_component.hpp"

#include "project/resource_manager.hpp"

namespace g::project
{

mesh_component::mesh_component(std::shared_ptr<class object> parent)
    : component("mesh", parent)
{
}

mesh_component::~mesh_component() = default;

std::shared_ptr<mesh_component>
mesh_component::create(std::shared_ptr<class object> object)
{
    auto resource = std::shared_ptr<mesh_component>(new mesh_component(object));
    resource_manager::get()->register_resource(resource);
    return resource;
}

std::string mesh_component::type() { return "mesh"; }

std::vector<std::array<float, 3>> const&
mesh_component::vertex_coordinates() const
{
    return _vertexCoordinates;
}

std::vector<std::array<float, 3>> const& mesh_component::vertex_normals() const
{
    return _vertexNormals;
}

std::vector<std::array<float, 2>> const& mesh_component::vertex_uvs() const
{
    return _vertexUVs;
}

std::vector<unsigned> const& mesh_component::vertex_indices() const
{
    return _vertexIndices;
}

void mesh_component::set_vertex_coordinates(
    std::vector<std::array<float, 3>> const& vertexCoordinates)
{
    _vertexCoordinates = vertexCoordinates;
}

void mesh_component::set_vertex_normals(
    std::vector<std::array<float, 3>> const& vertexNormals)
{
    _vertexNormals = vertexNormals;
}

void mesh_component::set_vertex_uvs(
    std::vector<std::array<float, 2>> const& vertexUVs)
{
    _vertexUVs = vertexUVs;
}

void mesh_component::set_vertex_indices(
    std::vector<unsigned> const& vertexIndices)
{
    _vertexIndices = vertexIndices;
}

} // namespace g::project
