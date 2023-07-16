#pragma once

#include <string_view>
#include <memory>
#include <unordered_map>

namespace g::project
{

namespace assets
{
class texture;
}

class asset;
enum class asset_type;

class asset_manager
{
public:
    asset_manager();
    ~asset_manager();

    // TODO: Find a better way to do this
    std::unordered_map<std::string, std::shared_ptr<asset>>
    load_asset(std::string_view file_path);
    void create_asset(std::string_view file_path);

private:
    std::shared_ptr<assets::texture>
    load_texture(std::string_view file_path);
};

} // namespace g::project
