#include <stdafx>

#include "engine/engine.hpp"

namespace g::engine
{

engine::engine()
    : _clock(clock::create())
{
}

engine_ptr engine::create() { return engine_ptr(new engine()); }

void engine::pre_init() { _scene->pre_init(); }

void engine::init() { _scene->init(); }

void engine::post_init() { _scene->post_init(); }

void engine::pre_update() { _scene->pre_update(); }

void engine::update() { _scene->update(); }

void engine::post_update() { _scene->post_update(); }

void engine::pre_deinit() { _scene->pre_deinit(); }

void engine::deinit() { _scene->deinit(); }

void engine::post_deinit() { _scene->post_deinit(); }

void engine::run()
{
    pre_init();
    init();
    post_init();

    while (true)
    {
        pre_update();
        update();
        post_update();
    }

    pre_deinit();
    deinit();
    post_deinit();
}

void engine::set_scene(scene_ptr scene) { _scene = scene; }

scene_ptr engine::get_scene() const { return _scene; }

void engine::set_clock(clock_ptr clock) { _clock = clock; }

clock_ptr engine::get_clock() const { return _clock; }

} // namespace g::engine
