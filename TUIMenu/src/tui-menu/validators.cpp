#include "tui-menu/validators.hpp"


tuim::Validator
operator|(tuim::Validator lhs, tuim::Validator rhs)
{
    return [lhs, rhs] (std::string_view buffer) -> bool {
        return (lhs(buffer) && rhs(buffer));
    };
}

tuim::Validator
tuim::is_numeric()
{
    return [] (std::string_view buffer) -> bool {
        bool dot_once  = false;
        float num      = 0;
        size_t n       = (buffer.front() == '-' ? 1 : 0);

        for (; n < buffer.length(); n++) {
            if (!dot_once && buffer[n] == '.')
                dot_once = true;
            else if (buffer[n] < '0' || buffer[n] > '9')
                return false;
        }

        try {
            num = std::stof(std::string(buffer));
            return true;
        } catch (const std::invalid_argument&) {
            return false;
        }
    };
};

tuim::Validator
tuim::is_integer()
{
    return [] (std::string_view buffer) -> bool {
        try {
            int num = std::stoi(std::string(buffer));
            return (std::to_string(num) == buffer);
        } catch (const std::invalid_argument&) {
            return false;
        }
    };
}

tuim::Validator
tuim::in_range(float minimum, float maximum)
{
    return [minimum, maximum] (std::string_view buffer) -> bool {
        float num = std::stof(std::string(buffer));
        return ((minimum <= num) && (num <= maximum));
    };
}

tuim::Validator
tuim::positive()
{
    return [] (std::string_view buffer) -> bool {
        return std::stof(std::string(buffer)) >= 0;
    };
}

tuim::Validator
tuim::negative()
{
    return [] (std::string_view buffer) -> bool {
        return std::stof(std::string(buffer)) < 0;
    };
}

tuim::Validator
tuim::not_null()
{
    return [] (std::string_view buffer) -> bool {
        float num = std::stof(std::string(buffer));
        return num != 0;
    };
}

tuim::Validator
tuim::length_range(size_t minimum, size_t maximum)
{
    return [minimum, maximum] (std::string_view buffer) -> bool {
        return ((minimum <= buffer.length()) && (buffer.length() <= maximum));
    };
}

tuim::Validator
tuim::wild_text()
{ return [] (std::string_view buffer) { return true; }; }

