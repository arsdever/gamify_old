#pragma once

#include <array>
#include <vector>

#include "project/component.hpp"

namespace g::project
{

class object;
class material;

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

    material* material() const;
    void setMaterial(std::unique_ptr<class material>&& material);

    render_context* renderContext() const;
    void setRenderContext(std::unique_ptr<render_context>&& renderContext);

private:
    std::unique_ptr<render_context> _renderContext { nullptr };
    std::unique_ptr<class material> _material { nullptr };
};

} // namespace g::project
