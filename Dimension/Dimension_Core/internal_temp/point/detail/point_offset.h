#pragma once
#include <type_traits>
#include "../../units/new_unit_stuff.h"        // for registered_unit<>, etc.
#include "../frame_traits.h"         // for frame_like, frame_offset_v

namespace dimension::detail {

// Unified offset query that works for BOTH frames and units.
// - If T is a frame (frame_like), use its frame offset.
// - If T is a unit (registered_unit), offset is 0.0.
// - Otherwise, hard error.
template<class T>
constexpr double point_offset() {
  if constexpr (dimension::frame_like<T>) {
    return dimension::frame_offset_v<T>;
  } else if constexpr (dimension::registered_unit<T>) {
    return 0.0;
  } else {
    static_assert(sizeof(T) == 0,
      "point_offset<T>: T is neither a frame nor a registered unit type.");
  }
}

} // namespace dimension::detail

// Public convenience variable-template facade.
// (We’ll gradually migrate call sites to use this.)
namespace dimension {
  template<class T>
  inline constexpr double point_offset_v = detail::point_offset<T>();
}
