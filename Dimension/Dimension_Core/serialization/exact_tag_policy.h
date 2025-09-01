// ============================================================================
//  exact_tag.h
//  Collision‑free pair‑list tag for Dimensional serialization
// ============================================================================

#ifndef DIMENSIONAL_SERIALIZATION_POLICY_EXACT_TAG_H
#define DIMENSIONAL_SERIALIZATION_POLICY_EXACT_TAG_H

#include <array> // std::array
#include <cstdint> // (u)int8/16_t
#include <cstring> // std::memcpy, std::memcmp
#include <tuple> // std::tuple and related
#include <utility> // std::index_sequence, std::make_index_sequence

#include "uuid.h"
#include "../internal_temp/units/unit_exponent.h"
#include "../internal_temp/dimension/base_dimension_from_tuple.h"

namespace dimension
{

   template<is_base_dimension Dim>
   [[nodiscard]]
   constexpr double get_as_primary(Dim obj)
   {
      return get_dimension_tuple<to_primary_tuple_t<typename Dim::simplified>>(obj);
   }

   template<is_base_dimension Dim>
   [[nodiscard]]
   constexpr Dim make_from_primary(double val)
   {
      return typename base_dimension_from_tuple<to_primary_tuple_t<typename Dim::simplified>>::dim{val};
   }

   // ---------------------------------------------------------------------
   //  Tag element
   // ---------------------------------------------------------------------
   struct TagPair {
      std::array<std::uint64_t,2> uuid;
      std::int8_t  num;
      std::uint8_t den;
   };

   inline constexpr std::size_t TagPairSize = sizeof(TagPair);

   // ---------------------------------------------------------------------
   //  1.  Generic builder that inspects Dim::units  (tuple of unit_exponent)
   // ---------------------------------------------------------------------
   template<typename Dim>
   struct TagArrayBuilder
   {
      using Tuple = typename Dim::simplified;
      static constexpr std::size_t K = std::tuple_size_v<Tuple>;

      template<std::size_t... I>
      static consteval std::array<TagPair, K> make(std::index_sequence<I...>)
      {
         return { TagPair{
               /*uuid*/ detail::unit_uuid_v<typename std::tuple_element_t<I,Tuple>::unit>,
               /*num*/  static_cast<std::int8_t>(
                           std::tuple_element_t<I,Tuple>::exponent::num),
               /*den*/  static_cast<std::uint8_t>(
                           std::tuple_element_t<I,Tuple>::exponent::den)
         }... };
      }

      static constexpr std::array<TagPair, K> build() noexcept
      {
         return make(std::make_index_sequence<K>{});
      }

      static constexpr std::size_t tag_size = K * TagPairSize;
   };

   template<typename Dim>
   inline constexpr auto TagArray_v = TagArrayBuilder<Dim>::build();

   template<is_base_dimension Dim>
   static constexpr std::size_t get_total_bytes()
   {
      return 
         2 + // Version Byte + Count Byte
         TagArrayBuilder<Dim>::tag_size + // Tag Bytes
         sizeof(double); // Data Bytes
   }

   // ---------------------------------------------------------------------
   //  3. ExactTagPolicy  –  plugs into your existing Serializer machinery
   // ---------------------------------------------------------------------
   struct ExactTagPolicy
   {
      // *******************************
      //  Encode
      // *******************************
      template<is_base_dimension Dim>
      [[nodiscard]]
      static auto serialize(const Dim& obj)
      {
         static constexpr std::size_t total_bytes = get_total_bytes<Dim>();
         using Buffer = std::array<std::uint8_t, total_bytes>;

         Buffer out{};
         auto* p = out.data();

         // 1. header
         *p++ = 1; // version
         *p++ = std::tuple_size_v<typename Dim::simplified>; // pair‑count

         // 2. tag
         constexpr std::size_t sz = tag_size<Dim>;
         if constexpr (sz > 0)               // avoid null pointer when sz == 0
         {
            constexpr auto tag = TagArray_v<Dim>;
            std::memcpy(p, tag.data(), sz);
            p += sz;
         }

         // 3. numeric value – convert to **primary/SI** units first TODO
         const double canonical = get_as_primary(obj);
         std::memcpy(p, &canonical, sizeof(canonical));

         return out;
      }

      // *******************************
      //  Decode
      // *******************************
      template<is_base_dimension Dim, typename Buffer>
      [[nodiscard]]
      static Dim deserialize(const Buffer& buf)
      {
         static constexpr std::size_t total_bytes = get_total_bytes<Dim>();
         if (buf.size() != total_bytes)
            throw std::invalid_argument("buffer size mismatch");

         const auto* p = buf.data();

         // 1. header
         if (*p++ != 1)
            throw std::invalid_argument("unsupported version");
         const std::uint8_t cnt = *p++;
         if (cnt != std::tuple_size_v<typename Dim::simplified>)
            throw std::invalid_argument("pair‑count mismatch");

         // 2. tag compare
         constexpr std::size_t sz = tag_size<Dim>;
         if constexpr (sz > 0)
         {
            constexpr auto expected = TagArray_v<Dim>;
            if (std::memcmp(p, expected.data(), sz) != 0)
               throw std::invalid_argument("dimension tag mismatch");
         }
         p += sz;      // advance the cursor unconditionally

         // 3. numeric value
         double canonical;
         std::memcpy(&canonical, p, sizeof(canonical));


         // construct result from canonical value
         return make_from_primary<Dim>(canonical);
      }

      // *******************************
      //  Size helper (needed by buffer‑size checks in your Serializer)
      // *******************************
      template<is_base_dimension Dim>
      inline static constexpr std::size_t tag_size =
         TagArrayBuilder<Dim>::tag_size;
   };

} // namespace dimension

#endif //DIMENSIONAL_SERIALIZATION_POLICY_EXACT_TAG_H
