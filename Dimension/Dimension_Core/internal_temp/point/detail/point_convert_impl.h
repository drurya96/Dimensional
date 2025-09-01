#pragma once
#include <type_traits>
#include "point_fwd.h"
#include "point_offset.h"
#include "frame_core.h"

namespace dimension::detail {

// Case 1: Same frame
template<frame_like Frame>
struct get_point_as_impl<Frame, Frame> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    return obj.value_;
  }
};

// Case 2: Target is the unit of the source's frame
template<is_unit Unit, frame_like Frame>
  requires std::same_as<Unit, frame_unit_t<Frame>>
struct get_point_as_impl<Unit, Frame> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    return get_point_as_impl<Frame, Frame>::convert(obj) + point_offset_v<Frame>;
  }
};

// Case 3: Target is a different unit entirely
template<is_unit TargetUnit, frame_like SourceFrame>
  requires (!std::same_as<TargetUnit, frame_unit_t<SourceFrame>>)
struct get_point_as_impl<TargetUnit, SourceFrame> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    const double raw =
      get_point_as_impl<SourceFrame, SourceFrame>::convert(obj) + point_offset_v<SourceFrame>;

    // keep your existing conversion surface:
    return ::dimension::details::do_conversion<
              TargetUnit,
              unit_exponent<frame_unit_t<SourceFrame>>
           >(raw);
  }
};

// Case 4: Target is a different frame
template<is_frame TargetFrame, is_frame SourceFrame>
  requires (!std::same_as<TargetFrame, SourceFrame>)
struct get_point_as_impl<TargetFrame, SourceFrame> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    const double raw =
      get_point_as_impl<SourceFrame, SourceFrame>::convert(obj) + point_offset_v<SourceFrame>;

    const double in_target_unit =
      ::dimension::details::do_conversion<
        frame_unit_t<TargetFrame>,
        unit_exponent<frame_unit_t<SourceFrame>>
      >(raw);

    return in_target_unit - point_offset_v<TargetFrame>;
  }
};

// Case 5: Target is a frame of this unit
template<is_frame TargetFrame, is_unit SourceUnit>
  requires std::same_as<SourceUnit, frame_unit_t<TargetFrame>>
struct get_point_as_impl<TargetFrame, SourceUnit> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    const double raw = get_point_as_impl<SourceUnit, SourceUnit>::convert(obj);
    return raw - point_offset_v<TargetFrame>;
  }
};

// Case 6: Target is a frame of a different unit
template<is_frame TargetFrame, is_unit SourceUnit>
  requires (!std::same_as<SourceUnit, frame_unit_t<TargetFrame>>)
struct get_point_as_impl<TargetFrame, SourceUnit> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    const double raw = get_point_as_impl<SourceUnit, SourceUnit>::convert(obj);

    const double in_target_unit =
      ::dimension::details::do_conversion<
        frame_unit_t<TargetFrame>,
        unit_exponent<SourceUnit>
      >(raw);

    return in_target_unit - point_offset_v<TargetFrame>;
  }
};

} // namespace dimension::detail
