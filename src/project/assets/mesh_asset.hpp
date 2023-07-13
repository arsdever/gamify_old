#pragma once

#include <array>
#include <string_view>
#include <vector>

#include "project/asset.hpp"

namespace g::project::assets
{

struct vector3
{
    float x;
    float y;
    float z;
};

struct vector2
{
    float x;
    float y;
};

class mesh : public asset
{
public:
    using vertex_t = std::tuple<vector3, vector3, vector2>;
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
