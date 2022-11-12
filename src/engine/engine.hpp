#pragma once

#include <engine/prototypes.hpp>
#include <project/prototypes.hpp>

namespace g::engine
{

class engine
{
private:
    engine();

public:
    static engine_ptr create();

    void init();
    void update();
    void deinit();

    void run();

    void set_scene(project::scene_ptr scene);
    project::scene_ptr get_scene() const;

private:
    project::scene_ptr _scene;
};

} // namespace g::engine