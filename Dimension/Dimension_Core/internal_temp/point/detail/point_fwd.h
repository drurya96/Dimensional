#pragma once
#include <type_traits>

namespace dimension {

template<typename T>
struct reference_frame;

template<typename Frame, typename Dim>
class point;

namespace detail{
// impl primary template (undefined here; specializations in convert_impl)
template<typename Target, typename Source>
struct get_point_as_impl;
}
} // namespace dimension
