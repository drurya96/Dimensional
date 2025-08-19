#pragma once
#include <type_traits>
#include "point_fwd.h"
#include "point_offset.h"
#include "frame_core.h"

namespace dimension::detail {

// Case 1: Same frame
template<typename Frame>
struct get_point_as_impl<Frame, Frame> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    return obj.value_;
  }
};

// Case 2: Target is the unit of the source's frame
template<typename Unit, typename Frame>
  requires std::is_same_v<Unit, frame_unit_t<Frame>>
struct get_point_as_impl<Unit, Frame> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    return get_point_as_impl<Frame, Frame>::convert(obj) + point_offset_v<Frame>;
  }
};

// Case 3: Target is a different unit entirely
template<typename TargetUnit, typename SourceFrame>
  requires (
    std::is_base_of_v<::dimension::FundamentalUnitTag, TargetUnit> &&
    !std::is_same_v<TargetUnit, frame_unit_t<SourceFrame>>
  )
struct get_point_as_impl<TargetUnit, SourceFrame> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    double raw = get_point_as_impl<SourceFrame, SourceFrame>::convert(obj) + point_offset_v<SourceFrame>;
    // keep your existing conversion surface:
    return ::dimension::details::do_conversion<TargetUnit, unit_exponent<frame_unit_t<SourceFrame>>>(raw);
  }
};

// Case 4: Target is a different frame
template<typename TargetFrame, typename SourceFrame>
  requires (
    !std::is_base_of_v<::dimension::FundamentalUnitTag, TargetFrame> &&
    !std::is_base_of_v<::dimension::FundamentalUnitTag, SourceFrame> &&
    !std::is_same_v<TargetFrame, SourceFrame>
  )
struct get_point_as_impl<TargetFrame, SourceFrame> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    double raw = get_point_as_impl<SourceFrame, SourceFrame>::convert(obj) + point_offset_v<SourceFrame>;
    double in_target_unit =
      ::dimension::details::do_conversion<frame_unit_t<TargetFrame>, unit_exponent<frame_unit_t<SourceFrame>>>(raw);
    return in_target_unit - point_offset_v<TargetFrame>;
  }
};

// Case 5: Target is a frame of this unit
template<typename TargetFrame, typename SourceUnit>
  requires (
    std::is_base_of_v<::dimension::FundamentalUnitTag, SourceUnit> &&
    std::is_same_v<SourceUnit, frame_unit_t<TargetFrame>>
  )
struct get_point_as_impl<TargetFrame, SourceUnit> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    double raw = get_point_as_impl<SourceUnit, SourceUnit>::convert(obj);
    return raw - point_offset_v<TargetFrame>;
  }
};

// Case 6: Target is a frame of a different unit
template<typename TargetFrame, typename SourceUnit>
  requires (
    std::is_base_of_v<::dimension::FundamentalUnitTag, SourceUnit> &&
    !std::is_same_v<SourceUnit, frame_unit_t<TargetFrame>>
  )
struct get_point_as_impl<TargetFrame, SourceUnit> {
  template<typename P>
  static constexpr double convert(const P& obj) {
    double raw = get_point_as_impl<SourceUnit, SourceUnit>::convert(obj);
    double in_target_unit =
      ::dimension::details::do_conversion<frame_unit_t<TargetFrame>, unit_exponent<SourceUnit>>(raw);
    return in_target_unit - point_offset_v<TargetFrame>;
  }
};

} // namespace dimension::detail
