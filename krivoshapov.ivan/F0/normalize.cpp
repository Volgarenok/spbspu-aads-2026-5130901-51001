#include "normalize.hpp"

#include <cstddef>
#include <cstdint>

namespace krivoshapov
{
  namespace
  {
    const std::uint32_t asciiLimit = 0x80;
    const unsigned char twoByteLeadMask = 0xE0;
    const unsigned char twoByteLeadValue = 0xC0;
    const std::uint32_t leadDataMask = 0x1F;
    const std::uint32_t trailDataMask = 0x3F;
    const std::uint32_t trailMarkerBits = 0x80;
    const std::uint32_t leadMarkerBits = 0xC0;
    const unsigned int trailBitCount = 6;
    const std::uint32_t cyrillicUpperFirst = 0x0410;
    const std::uint32_t cyrillicLowerLast = 0x044F;
    const std::uint32_t cyrillicUpperLast = 0x042F;
    const std::uint32_t cyrillicYoUpper = 0x0401;
    const std::uint32_t cyrillicYoLower = 0x0451;
    const std::uint32_t cyrillicCaseShift = 0x20;

    bool isAsciiWordByte(unsigned char byte)
    {
      const bool isDigit = byte >= '0' && byte <= '9';
      const bool isLower = byte >= 'a' && byte <= 'z';
      const bool isUpper = byte >= 'A' && byte <= 'Z';
      return isDigit || isLower || isUpper;
    }

    char toLowerAscii(unsigned char byte)
    {
      if (byte >= 'A' && byte <= 'Z')
      {
        return static_cast<char>(byte + ('a' - 'A'));
      }
      return static_cast<char>(byte);
    }

    bool isCyrillicLetter(std::uint32_t codePoint)
    {
      const bool inMainRange = codePoint >= cyrillicUpperFirst && codePoint <= cyrillicLowerLast;
      const bool isYo = codePoint == cyrillicYoUpper || codePoint == cyrillicYoLower;
      return inMainRange || isYo;
    }

    std::uint32_t toLowerCyrillic(std::uint32_t codePoint)
    {
      if (codePoint >= cyrillicUpperFirst && codePoint <= cyrillicUpperLast)
      {
        return codePoint + cyrillicCaseShift;
      }
      if (codePoint == cyrillicYoUpper)
      {
        return cyrillicYoLower;
      }
      return codePoint;
    }

    void appendTwoByteUtf8(std::string &token, std::uint32_t codePoint)
    {
      const std::uint32_t lead = leadMarkerBits | (codePoint >> trailBitCount);
      const std::uint32_t trail = trailMarkerBits | (codePoint & trailDataMask);
      token += static_cast<char>(lead);
      token += static_cast<char>(trail);
    }

    void flushToken(Array<std::string> &words, std::string &token)
    {
      if (!token.empty())
      {
        words.push(token);
        token.clear();
      }
    }
  }
}

krivoshapov::Array<std::string> krivoshapov::splitIntoWords(const std::string &text)
{
  Array<std::string> words;
  std::string token;
  std::size_t i = 0;
  while (i < text.size())
  {
    const unsigned char byte = static_cast<unsigned char>(text[i]);
    if (byte < asciiLimit)
    {
      if (isAsciiWordByte(byte))
      {
        token += toLowerAscii(byte);
      }
      else
      {
        flushToken(words, token);
      }
      i += 1;
    }
    else if ((byte & twoByteLeadMask) == twoByteLeadValue && i + 1 < text.size())
    {
      const unsigned char second = static_cast<unsigned char>(text[i + 1]);
      const std::uint32_t high = static_cast<std::uint32_t>(byte & leadDataMask) << trailBitCount;
      const std::uint32_t low = static_cast<std::uint32_t>(second & trailDataMask);
      const std::uint32_t codePoint = high | low;
      if (isCyrillicLetter(codePoint))
      {
        appendTwoByteUtf8(token, toLowerCyrillic(codePoint));
      }
      else
      {
        flushToken(words, token);
      }
      i += 2;
    }
    else
    {
      flushToken(words, token);
      i += 1;
    }
  }
  flushToken(words, token);
  return words;
}
