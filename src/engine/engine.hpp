#pragma once

namespace g::engine
{

class engine
{
private:
    engine();

public:
    static engine_ptr create();

    void pre_init();
    void init();
    void post_init();

    void pre_update();
    void update();
    void post_update();

    void pre_deinit();
    void deinit();
    void post_deinit();

    void run();

    void set_scene(scene_ptr scene);
    scene_ptr get_scene() const;

    void set_clock(clock_ptr clock);
    clock_ptr get_clock() const;

private:
    project::scene_ptr _scene;
    clock_ptr _clock;
}

} // namespace g::engine