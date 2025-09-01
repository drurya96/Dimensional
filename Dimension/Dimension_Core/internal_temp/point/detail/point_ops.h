#pragma once
#include "point_fwd.h"
#include "point_convert_impl.h" // for get_point_as_impl
#include "../../units/Conversion.h"
#include "../../units/get_factor.h"
#include "../frame_traits.h"

namespace dimension {

// Entry wrapper (public) still lives here for now
template<typename Target, typename P>
constexpr double get_point_as(const P& obj) {
  return detail::get_point_as_impl<Target, typename P::frame_type>::convert(obj);
}

// ===================== Addition/Subtraction =====================

// point + unit -> point
template<typename T, typename U, typename Dim>
requires true // TODO: require base-dimension matches lhs
constexpr point<T, Dim> operator+(point<T, Dim> lhs, base_dimension_impl<double, unit_exponent<U>> rhs)
{
  return point<T, Dim>(get_point_as<T>(lhs) + get_dimension_as<unit_exponent<frame_unit_t<T>>>(rhs));
}

// unit + point -> point
template<typename T, typename U, typename Dim>
requires true
constexpr point<T, Dim> operator+(base_dimension_impl<double, unit_exponent<U>> lhs, point<T, Dim> rhs)
{
  return point<T, Dim>(get_point_as<T>(rhs) + get_dimension_as<unit_exponent<frame_unit_t<T>>>(lhs));
}

// point - unit -> point
template<typename T, typename U, typename Dim>
requires true
constexpr point<T, Dim> operator-(point<T, Dim> lhs, base_dimension_impl<double, unit_exponent<U>> rhs)
{
  return point<T, Dim>(get_point_as<T>(lhs) - get_dimension_as<unit_exponent<frame_unit_t<T>>>(rhs));
}

// point - point -> vector quantity (in T::unit)
template<typename T, typename U, typename Dim>
requires true
constexpr base_dimension_impl<double, unit_exponent<frame_unit_t<T>>> operator-(point<T, Dim> lhs, point<U, Dim> rhs)
{
  return base_dimension_impl<double, unit_exponent<frame_unit_t<T>>>(
      get_point_as<frame_unit_t<T>>(lhs) - get_point_as<frame_unit_t<T>>(rhs)
  );
}

// ===================== Multiplication/Division =====================

template<typename T, is_base_dimension Rhs, typename Dim>
requires true
constexpr auto operator*(point<T, Dim> lhs, Rhs rhs)
{
  return base_dimension_impl<double, unit_exponent<frame_unit_t<T>>>(get_point_as<frame_unit_t<T>>(lhs)) * rhs;
}

template<typename T, is_base_dimension Lhs, typename Dim>
requires true
constexpr auto operator*(Lhs lhs, point<T, Dim> rhs)
{
  return lhs * base_dimension_impl<double, unit_exponent<frame_unit_t<T>>>(get_point_as<frame_unit_t<T>>(rhs));
}

template<typename T, is_base_dimension Rhs, typename Dim>
requires true
constexpr auto operator/(point<T, Dim> lhs, Rhs rhs)
{
  return base_dimension_impl<double, unit_exponent<frame_unit_t<T>>>(get_point_as<frame_unit_t<T>>(lhs)) / rhs;
}

template<typename T, is_base_dimension Lhs, typename Dim>
requires true
constexpr auto operator/(Lhs lhs, point<T, Dim> rhs)
{
  return lhs / base_dimension_impl<double, unit_exponent<frame_unit_t<T>>>(get_point_as<frame_unit_t<T>>(rhs));
}

} // namespace dimension
