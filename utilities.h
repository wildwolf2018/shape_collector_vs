#ifndef UTILITIES_H
#define UTILITIES_H
#include <type_traits>

template <typename E>
constexpr auto EnumToInt(E _enum) noexcept
{
	return static_cast<std::underlying_type_t<E>>(_enum);
}

#endif
