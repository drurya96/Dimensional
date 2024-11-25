# Serialization Documentation

## Overview

The Dimensional Serialization API enables serialization and deserialization of BaseDimension objects. The implementation is policy-based, allowing users to define custom serialization logic. By default, the library provides DefaultSerializationPolicy, which hashes type information alongside the scalar value for validation. Users can create their own policies to extend or modify behavior.

[[_TOC_]]

## Core Concepts

### BaseDimension
BaseDimension objects encapsulate physical dimensions defined by numerator and denominator tuples (NumTuple and DenTuple). The serialization API works with these objects.

### Policy-Based Design
The serialization API relies on policies for flexibility. Policies define how:

- Data is written to a buffer (serialization).
- Data is reconstructed from a buffer (deserialization).
- Validation is performed (e.g., type tags).

### Tagging and Validation
The default serialization policy uses hashed type tags derived from tuples (NumTuple, DenTuple) to ensure data integrity during deserialization.
This prevents mismatches between serialized data and expected types.



## Serialization API

### Functions
The API provides serialization and deserialization functions via the Serializer struct and global helper functions.

**Serialize**
Serializes a BaseDimension object into a buffer or returns a new buffer:

```cpp
// Serialize into an existing buffer
template<typename NumTuple, typename DenTuple, typename OutputBuf, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
void serialize(OutputBuf& out, const BaseDimension<NumTuple, DenTuple>& obj);

// Serialize and return a new buffer
template<typename NumTuple, typename DenTuple, typename OutputBuf = std::vector<uint8_t>, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
OutputBuf serialize(const BaseDimension<NumTuple, DenTuple>& obj);
```

**Deserialize**
Deserializes a buffer into a BaseDimension object:

```cpp
// Deserialize into a new object
template<typename NumTuple, typename DenTuple, typename InputBuf, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
BaseDimension<NumTuple, DenTuple> deserialize(const InputBuf& in);

// Deserialize into an existing object
template<typename NumTuple, typename DenTuple, typename InputBuf, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
void deserialize(const InputBuf& in, BaseDimension<NumTuple, DenTuple>& obj);
```

**Examples**
Basic Serialization
```cpp
using NumTuple = std::tuple<Meters>;
using DenTuple = std::tuple<>;

BaseDimension<NumTuple, DenTuple> dimension(10.0);

// Serialize into a buffer
std::vector<uint8_t> buffer = serialize(dimension);

// Deserialize back into a new object
BaseDimension<NumTuple, DenTuple> result = deserialize<NumTuple, DenTuple>(buffer);
```

**Custom Buffer Types**
```cpp
std::vector<uint16_t> buffer;
serialize<NumTuple, DenTuple>(buffer, dimension);

BaseDimension<NumTuple, DenTuple> result;
deserialize<NumTuple, DenTuple>(buffer, result);
```

## DefaultSerializationPolicy

### Key Features

- Tagging: Hashes type information from NumTuple and DenTuple using the HashPolicy.
- Validation: Ensures that deserialized data matches the expected type.
- Precision: Serializes and deserializes a double value for the scalar.

### Design

The default policy is defined as:

```cpp
template<typename HashPolicy>
struct DefaultSerializationPolicy;
```

- HashPolicy: Determines how type tags are generated. The library provides FNV_1a_32Bit as a built-in hash policy.
**Example**
```cpp
using Policy = DefaultSerializationPolicy<FNV_1a_32Bit>;

std::vector<uint8_t> buffer = Serializer<NumTuple, DenTuple, Policy>::serialize(dimension);
BaseDimension<NumTuple, DenTuple> result = Serializer<NumTuple, DenTuple, Policy>::deserialize(buffer);
```




## Custom Serialization Policies

### Creating a Policy
To define a custom serialization policy, implement:

- serialize: Writes data to a buffer.
- deserialize: Reads data from a buffer.

**Example**

```cpp
struct MinimalPolicy
{
    template<typename NumTuple, typename DenTuple, typename OutputBuf>
    static void serialize(OutputBuf& out, const BaseDimension<NumTuple, DenTuple>& obj)
    {
        double value = obj.template GetVal<NumTuple, DenTuple>();
        std::memcpy(&*out.begin(), &value, sizeof(double));
    }

    template<typename NumTuple, typename DenTuple, typename InputBuf>
    static BaseDimension<NumTuple, DenTuple> deserialize(const InputBuf& in)
    {
        double value;
        std::memcpy(&value, &*in.begin(), sizeof(double));
        return BaseDimension<NumTuple, DenTuple>(value);
    }
};
```

## Future Enhancements

- Endianess Handling: Future updates may include endianess conversion in the default policy to ensure cross-platform compatibility.
- Metadata Extensions: Adding optional metadata serialization for richer context.
- Optimized Buffering: Support for custom allocator strategies in buffer types.
By merging this serialization API, you enable robust extensibility while ensuring stability through policy-based design.


## Testing and Validation

### Unit Tests

Test coverage includes

- Serialization and deserialization using default and custom policies.
- Validation of type tags during deserialization.
- Various buffer types (std::vector<uint8_t>, std::vector<uint16_t>, etc.).
