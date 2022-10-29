#pragma once

#include <project/behavior.hpp>

namespace g::sample_game
{

class print_behavior : public project::behavior
{
protected:
    print_behavior(std::string_view name);

public:
	static std::shared_ptr<print_behavior> create(std::string_view name);

	void init() override;
    void update() override;
	void deinit() override;

private:
	std::string _name;
};

} // namespace g::sample_game