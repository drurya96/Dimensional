#pragma once

// Public “point” surface. Only exposes the class, the free functions, and the
// _t/_v facades. All mechanics live in detail headers.

#include <type_traits>
#include <utility>
#include <concepts>

#include "detail/point_fwd.h"
#include "detail/point_offset.h"
#include "detail/point_convert_impl.h"
#include "detail/point_ops.h"
#include "frame_traits.h"

namespace dimension {

// Simple frame: ties a unit type to an offset via traits/struct members later.
// Keeping your original for now.
template<typename T>
struct reference_frame {
  using unit = T;
};

template<typename Frame, typename Dim>
class point {
public:
  using frame_type = Frame;
  using dimension  = Dim;

  constexpr explicit point(double val) : value_(val) {}

  template<typename T>
  // requires true; // your TODO
  // cppcheck-suppress noExplicitConstructor
  constexpr point(point<T, Dim> obj) : value_(get_point_as<Frame>(obj)) {}

private:
  double value_;

  // friend the “same-frame” conversion impl
  friend struct detail::get_point_as_impl<Frame, Frame>;
};


} // namespace dimension
