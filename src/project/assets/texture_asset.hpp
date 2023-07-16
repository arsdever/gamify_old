#pragma once

#include <array>
#include <memory>
#include <string_view>
#include <vector>

#include "common/vector.hpp"
#include "project/asset.hpp"

namespace g::project::assets
{

class texture : public asset
{
private:
    texture(std::string_view name);

public:
	~texture();

    static std::shared_ptr<texture> create(std::string_view name);

    void set_size(std::array<unsigned, 2> size);
    std::array<unsigned, 2> size() const;

    void set_format(std::array<char, 4> format);
    std::array<char, 4> const& format() const;

    void set_data(std::vector<char> data);
    std::vector<char> const& data() const;

private:
    std::array<unsigned, 2> _size;
    std::array<char, 4> _format;
    std::vector<char> _data;
};

} // namespace g::project::assets
