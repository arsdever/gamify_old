#pragma once

#include <array>
#include <vector>

#include "project/component.hpp"

namespace g::project
{

class object;
class material;

namespace assets
{
class mesh;
} // namespace assets

struct render_context
{
    virtual ~render_context() = 0;
};

class renderer_component : public component
{
protected:
    renderer_component(std::shared_ptr<class object> object);

public:
    virtual ~renderer_component() override;

    static std::shared_ptr<renderer_component>
    create(std::shared_ptr<class object> object);
    static std::string type();

    std::shared_ptr<assets::mesh> mesh() const;
    void set_mesh(std::shared_ptr<assets::mesh> mesh);

    material* material() const;
    void set_material(std::unique_ptr<class material>&& material);

    render_context* render_context() const;
    void set_render_context(std::unique_ptr<struct render_context>&& renderContext);

private:
    std::unique_ptr<struct render_context> _render_context { nullptr };
    common::uuid _mesh_uuid;
    std::unique_ptr<class material> _material { nullptr };
};

} // namespace g::project
