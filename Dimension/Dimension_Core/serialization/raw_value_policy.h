#ifndef DIMENSIONAL_SERIALIZATION_POLICY_RAW_VALUE_H
#define DIMENSIONAL_SERIALIZATION_POLICY_RAW_VALUE_H

#include <array> // std::array
#include <cstdint> // uint8_t
#include <cstring> // std::memcpy

namespace dimension
{
   struct RawValuePolicy    // aka “Null” or “Unsafe” policy
   {
      // ────────────────────────────────────────────────────────────────
      //  tag_size – required by the common interface (always zero)
      // ────────────────────────────────────────────────────────────────
      template<is_base_dimension Dim>
      inline static constexpr std::size_t tag_size = 0;

      // ────────────────────────────────────────────────────────────────
      //  Serialize → std::array<std::uint8_t, 8>
      // ────────────────────────────────────────────────────────────────
      template<is_base_dimension Dim>
      [[nodiscard]]
      static auto serialize(const Dim& obj)
      {
         using Buffer = std::array<std::uint8_t, sizeof(double)>;
         Buffer out{};

         const double v = get_dimension_tuple<typename Dim::units>(obj);
         std::memcpy(out.data(), &v, sizeof(v));

         return out;                                    // no header, no tag
      }

      // ────────────────────────────────────────────────────────────────
      //  Deserialize – NO validation
      // ────────────────────────────────────────────────────────────────
      template<is_base_dimension Dim, typename Buffer>
      [[nodiscard]]
      static Dim deserialize(const Buffer& buf)
      {
         if (buf.size() != sizeof(double))
            throw std::invalid_argument("buffer size mismatch");

         double v;
         std::memcpy(&v, buf.data(), sizeof(v));
         return Dim{v};              // blindly construct
      }
   };

} // namespace dimension

#endif // DIMENSIONAL_SERIALIZATION_POLICY_RAW_VALUE_H
