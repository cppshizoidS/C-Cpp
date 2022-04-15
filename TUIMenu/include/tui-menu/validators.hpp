#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "types.hpp"


tuim::Validator
operator|(tuim::Validator lhs, tuim::Validator rhs);


namespace tuim {


Validator
is_numeric();

Validator
is_integer();

Validator
in_range(float minimum, float maximum);

Validator
positive();

Validator
negative();

Validator
not_null();

Validator
length_range(size_t minimum, size_t maximum);

Validator
wild_text();


}

