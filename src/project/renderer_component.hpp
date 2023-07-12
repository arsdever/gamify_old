#pragma once

#include <array>
#include <vector>

#include "project/component.hpp"

namespace g::project
{

class object;

class renderer_component : public component
{
protected:
    renderer_component(std::shared_ptr<class object> object);

public:
    virtual ~renderer_component() override;

    static std::shared_ptr<renderer_component>
    create(std::shared_ptr<class object> object);

    // TODO: replace with mesh
    std::vector<std::array<float, 3>> const& vertex_coordinates() const;
    std::vector<unsigned> const& vertex_indices() const;

    // TODO: replace with material and move shader_asset into material
    unsigned int shader_asset();

private:
    std::vector<std::array<float, 3>> _vertexCoordinates;
    std::vector<unsigned> _vertexIndices;
    unsigned int _shader { 0 };
};

} // namespace g::project
