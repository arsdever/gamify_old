#include <stdafx>

#include "project/scene.hpp"

#include "project/game_object.hpp"
#include "project/object.hpp"

namespace g::project
{

scene::scene() = default;

scene_ptr scene::create(std::string_view name)
{
    scene_ptr s { new scene {} };
    s->_name = name;
    return s;
}

std::string scene::name() const { return _name; }

object_ptr scene::create_object(std::string_view name, object_ptr parent)
{
    object_ptr obj = object::create(name, parent);
    _objects.push_back(obj);
    on_object_added(obj);
    return obj;
}

std::list<object_ptr> scene::objects() const { return _objects; }

std::list<behavior_ptr> scene::behaviors() const
{
    std::list<behavior_ptr> behaviors;
    std::for_each(std::begin(_objects),
                  std::end(_objects),
                  [ &behaviors ](object_ptr obj)
                  {
        std::list<behavior_ptr> obj_behaviors =
            std::static_pointer_cast<game_object>(obj)->behaviors();
        std::for_each(std::begin(obj_behaviors),
                      std::end(obj_behaviors),
                      [ &behaviors ](behavior_ptr behavior)
                      { behaviors.push_back(behavior); });
    });
    return behaviors;
}

} // namespace g::project
