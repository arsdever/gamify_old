#pragma once

namespace g::project
{

class asset;
enum class asset_type;

class asset_manager
{
public:
    asset_manager();
    ~asset_manager();

    std::shared_ptr<asset> load_asset(std::string_view file_path);
    void create_asset(std::string_view file_path);
};

} // namespace g::project
