#pragma once

#include <array>
#include <string_view>
#include <vector>

#include "common/vector.hpp"
#include "project/asset.hpp"

namespace g::project::assets
{

class mesh : public asset
{
public:
    using vertex_t = struct
    {
        common::vector3f position;
        common::vector3f normal;
        common::vector2 uv;
    };
    using vertices_t = std::vector<vertex_t>;
    using indices_t = std::vector<unsigned>;

private:
    mesh(std::string_view name);

public:
    ~mesh();

    static std::shared_ptr<mesh>
    create(std::string_view name, vertices_t vertices, indices_t indices);

    vertices_t const& vertices() const;
    indices_t const& indices() const;

private:
    vertices_t _vertices;
    indices_t _indices;
};

} // namespace g::project::assets
