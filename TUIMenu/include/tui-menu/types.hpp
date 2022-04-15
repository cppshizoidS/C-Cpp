#pragma once

#include <concepts>
#include <functional>
#include <string>
#include <string_view>


namespace tuim {


using Validator = std::function<bool(std::string_view)>;

template <typename T>
concept string_like = std::is_convertible_v<T, std::string_view>;


}

