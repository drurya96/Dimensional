#pragma once
#include <type_traits>
#include <concepts>

// Forward decl so we can detect unit-ness without including the whole unit header here.
namespace dimension {
  template<class U> struct unit_traits; // users specialize elsewhere
}

namespace dimension {

// Primary template (no definition)
template<class Frame, class = void>
struct frame_traits;

// 1) Structural fallback for real frames that expose `using unit` and `static constexpr offset`
template<class Frame>
struct frame_traits<Frame, std::void_t<typename Frame::unit, decltype(Frame::offset)>> {
  using unit = typename Frame::unit;
  static constexpr double offset = static_cast<double>(Frame::offset);
};

// 2) Unit-bridge: if `unit_traits<U>` exists, treat U as a frame with zero offset
template<class U>
struct frame_traits<U, std::void_t<typename unit_traits<U>::dimension>> {
  using unit = U;
  static constexpr double offset = 0.0;
};

// Accessors
template<class F>
using frame_unit_t = typename frame_traits<F>::unit;

template<class F>
inline constexpr double frame_offset_v = frame_traits<F>::offset;

// “frame-like” now includes both genuine frames and units (via the unit-bridge)
template<class F>
concept frame_like = requires {
  typename frame_unit_t<F>;
  { frame_offset_v<F> } -> std::convertible_to<double>;
};

} // namespace dimension
