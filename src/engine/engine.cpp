#include <stdafx>

#include "engine/engine.hpp"

#include "project/behavior.hpp"
#include "project/scene.hpp"

namespace g::engine
{

engine::engine() { }

engine_ptr engine::create() { return engine_ptr(new engine()); }

void engine::init()
{
    for (auto& behavior : _scene->behaviors())
    {
        behavior->init();
    }
}

void engine::update()
{
    for (auto& behavior : _scene->behaviors())
    {
        behavior->update();
    }
}

void engine::deinit()
{
    for (auto& behavior : _scene->behaviors())
    {
        behavior->deinit();
    }
}

void engine::run()
{
    init();

    while (true)
    {
        update();
    }

    deinit();
}

void engine::set_scene(project::scene_ptr scene) { _scene = scene; }

project::scene_ptr engine::get_scene() const { return _scene; }

} // namespace g::engine
