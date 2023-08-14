#ifndef DIFF_PATCH_DEFS_HPP
#define DIFF_PATCH_DEFS_HPP

#include <string>
#include <tuple>
#include <cstdint>

using String = std::string;
using Char = typename std::string::value_type;
using ConstIter = typename std::string::const_iterator;
using UInt32 = std::uint32_t;
using Point = std::tuple<std::uint32_t, std::uint32_t>;

#endif // DIFF_PATCH_DEFS_HPP