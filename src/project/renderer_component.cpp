#include "project/renderer_component.hpp"

namespace g::project
{

renderer_component::renderer_component(std::shared_ptr<class object> parent)
    : component("Renderer", parent)
{
    // _vertexCoordinates = {
    //     { -.5, -.5, 0 }, { .5, -.5, 0 }, { .5, .5, 0 }, { -.5, .5, 0 }
    // };
    _vertexCoordinates.push_back(std::array<float, 3> { -.5f, -.5f, .0f });
    _vertexCoordinates.push_back(std::array<float, 3> { .5f, -.5f, .0f });
    _vertexCoordinates.push_back(std::array<float, 3> { .5f, .5f, .0f });
    _vertexCoordinates.push_back(std::array<float, 3> { -.5f, .5f, .0f });

    _vertexIndices = { 0, 1, 2, 2, 3, 0 };
}

renderer_component::~renderer_component() { }

std::shared_ptr<renderer_component>
renderer_component::create(std::shared_ptr<class object> object)
{
    return std::shared_ptr<renderer_component>(new renderer_component(object));
}

std::vector<std::array<float, 3>> const&
renderer_component::vertex_coordinates() const
{
    return _vertexCoordinates;
}

std::vector<unsigned> const& renderer_component::vertex_indices() const
{
    return _vertexIndices;
}

unsigned int renderer_component::shader_asset() { return _shader; }

} // namespace g::project
