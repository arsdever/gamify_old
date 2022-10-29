#pragma once

template <typename T>
struct pointer_traits
{
    using element_type = T;
    using sptr = std::shared_ptr<T>;
    using wptr = std::weak_ptr<T>;
    using uptr = std::unique_ptr<T>;
    using scptr = std::shared_ptr<const T>;
    using wcptr = std::weak_ptr<const T>;
    using ucptr = std::unique_ptr<const T>;
};