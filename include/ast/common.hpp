#ifndef LAMBDA_COMMON_HPP
#define LAMBDA_COMMON_HPP

#include <memory> // std::shared_ptr
#include <string>
#include <sstream>
#include <utility> // std::move
#include <cassert> // assert
#include <vector>

namespace ast
{
    using std::string;
    using std::to_string;

    template<typename T>
    using list = std::vector<T>;
}

#endif
