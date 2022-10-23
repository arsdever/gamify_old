#include <stdafx.hpp>

#include "engine/clock.hpp"

namespace g::engine
{

clock::clock() { m_last_time = std::chrono::high_resolution_clock::now(); }

clock_ptr clock::create() { return clock_ptr(new clock()); }

void clock::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    m_delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(
                       now - m_last_time)
                       .count();
    m_last_time = now;
    m_time += m_delta_time;
}

float clock::get_delta_time() const { return m_delta_time; }

float clock::get_time() const { return m_time; }

} // namespace g::engine