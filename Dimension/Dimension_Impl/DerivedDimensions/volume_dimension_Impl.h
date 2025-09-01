#ifndef STATIC_DIMENSION_VOLUME_IMPL_H
#define STATIC_DIMENSION_VOLUME_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/length_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0
   >
   concept are_volume_units =
         (
               is_length_unit<T0>
         )
   ;

   /// @brief Concept to verify a type can serve as a named volume unit
   template<typename T>
   concept IsNamedvolumeUnit =
      (std::tuple_size_v<unit_units_t<T>> == 1) &&
      is_length_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a volume type
   template<typename T, typename Rep>
   concept is_volume_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_length, 3>
   >;

   template<typename T>
   concept is_volume = is_volume_as<T, double>;

   /// @brief Retrieves the value of a volume object with specific units
   /// @tparam lengthUnit The length unit used for all length components of volume
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_length_unit lengthUnit,
      is_volume DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_volume_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 3>
      >(obj);
   }

   /// @brief Retrieves the value of a named volume object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedvolumeUnit Named, is_volume DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_volume_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class volume;

   template<
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_volume_units<
      T0
   >
   class volume<T0, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<lengthType, T0>::type, 3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<lengthType, T0>::type, 3>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_volume<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr volume(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_volume_units<
      T0
   >
   class volume<Rep, T0, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<lengthType, T0>::type, 3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<lengthType, T0>::type, 3>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_volume<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr volume(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<IsNamedvolumeUnit Named, is_coefficient... Cs>
   class volume<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_volume<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr volume(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<rep_type Rep, IsNamedvolumeUnit Named, is_coefficient... Cs>
   class volume<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_volume<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr volume(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_volume_units<
      T0
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_volume(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return volume<double, T0, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_volume_units<
      T0
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_volume(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return volume<Rep, T0, Cs...>(value);
   }

   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<IsNamedvolumeUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_volume(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return volume<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<IsNamedvolumeUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_volume(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return volume<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_volume Dim>
   volume(Dim) ->
   volume<
      simplified_unit_filter<lengthType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_VOLUME_IMPL_H