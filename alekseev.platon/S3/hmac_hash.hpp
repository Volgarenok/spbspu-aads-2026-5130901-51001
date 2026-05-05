#ifndef ALEKSEEV_S3_HMAC_HASH_HPP
#define ALEKSEEV_S3_HMAC_HASH_HPP

#include <cstddef>
#include <cstdint>
#include <string>

#if __has_include(<boost/hash2/hmac.hpp>) && __has_include(<boost/hash2/sha2.hpp>) && __has_include(<boost/hash2/get_integral_result.hpp>)
#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/hmac.hpp>
#include <boost/hash2/sha2.hpp>
#define ALEKSEEV_S3_HAS_BOOST_HASH2 1
#else
#define ALEKSEEV_S3_HAS_BOOST_HASH2 0
#endif

#include "edge_key.hpp"
#include "sequence.hpp"

namespace alekseev
{
  class HmacHash
  {
  public:
    size_t hash_bytes(const unsigned char* data, size_t size) const
    {
#if ALEKSEEV_S3_HAS_BOOST_HASH2
      static const unsigned char key[] = {
        'a', 'l', 'e', 'k', 's', 'e', 'e', 'v', '-', 's', '3', '-', 'h', 'm', 'a', 'c'
      };
      boost::hash2::hmac< boost::hash2::sha2_256 > hash(key, sizeof(key));
      hash.update(data, size);
      return boost::hash2::get_integral_result< size_t >(hash);
#else
      // Fallback for environments without Boost.Hash2; сдача варианта HMAC требует Boost.Hash2.
      return fallback_hmac_like(data, size);
#endif
    }

    size_t operator()(const std::string& value) const
    {
      const unsigned char* bytes = reinterpret_cast< const unsigned char* >(value.data());
      return hash_bytes(bytes, value.size());
    }

    size_t operator()(const EdgeKey& key) const
    {
      HmacHashCombiner combiner;
      combiner.append_size(key.from.size());
      combiner.append_string(key.from);
      combiner.append_size(key.to.size());
      combiner.append_string(key.to);
      return combiner.result(*this);
    }

  private:
    class HmacHashCombiner
    {
    public:
      HmacHashCombiner():
        data_(),
        size_(0)
      {}

      void append_size(size_t value)
      {
        for (size_t i = 0; i < sizeof(size_t); ++i)
        {
          append_byte(static_cast< unsigned char >((value >> (i * 8)) & 0xffu));
        }
      }

      void append_string(const std::string& value)
      {
        for (size_t i = 0; i < value.size(); ++i)
        {
          append_byte(static_cast< unsigned char >(value[i]));
        }
      }

      size_t result(const HmacHash& hash) const
      {
        return hash.hash_bytes(data_.data(), size_);
      }

    private:
      void append_byte(unsigned char byte)
      {
        data_.push_back(byte);
        ++size_;
      }

      Sequence< unsigned char > data_;
      size_t size_;
    };

    static size_t byte_hash(const unsigned char* data, size_t size, size_t seed)
    {
      size_t result = seed;
      for (size_t i = 0; i < size; ++i)
      {
        result ^= static_cast< size_t >(data[i]);
        result *= static_cast< size_t >(1099511628211ull);
        result ^= result >> 32;
      }
      return result;
    }

    static size_t fallback_hmac_like(const unsigned char* data, size_t size)
    {
      const size_t ipad = static_cast< size_t >(0x3636363636363636ull);
      const size_t opad = static_cast< size_t >(0x5c5c5c5c5c5c5c5cull);
      const size_t inner = byte_hash(data, size, static_cast< size_t >(1469598103934665603ull) ^ ipad);
      unsigned char inner_bytes[sizeof(size_t)];
      for (size_t i = 0; i < sizeof(size_t); ++i)
      {
        inner_bytes[i] = static_cast< unsigned char >((inner >> (i * 8)) & 0xffu);
      }
      return byte_hash(inner_bytes, sizeof(inner_bytes), static_cast< size_t >(1469598103934665603ull) ^ opad);
    }
  };
}

#endif
