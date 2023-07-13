#pragma once

#include <array>
#include <vector>

#include "project/component.hpp"

namespace g::project
{

class object;

class mesh_component : public component
{
    mesh_component(std::shared_ptr<class object> parent);

public:
    virtual ~mesh_component() override;

    static std::shared_ptr<mesh_component>
    create(std::shared_ptr<class object> object);
	static std::string type();

    std::vector<std::array<float, 3>> const& vertex_coordinates() const;
    std::vector<std::array<float, 3>> const& vertex_normals() const;
    std::vector<std::array<float, 2>> const& vertex_uvs() const;
    std::vector<unsigned> const& vertex_indices() const;

    void set_vertex_coordinates(
        std::vector<std::array<float, 3>> const& vertexCoordinates);
    void
    set_vertex_normals(std::vector<std::array<float, 3>> const& vertexNormals);
    void set_vertex_uvs(std::vector<std::array<float, 2>> const& vertexUVs);
    void set_vertex_indices(std::vector<unsigned> const& vertexIndices);

private:
    std::vector<std::array<float, 3>> _vertexCoordinates;
    std::vector<std::array<float, 3>> _vertexNormals;
    std::vector<std::array<float, 2>> _vertexUVs;
    std::vector<unsigned> _vertexIndices;
};

} // namespace g::project
