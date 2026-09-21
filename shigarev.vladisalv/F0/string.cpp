#include "string.hpp"

namespace yafu {
  namespace detail {
    constexpr size_t initialStringCapacity = 16;
    constexpr size_t stringGrowthFactor = 2;
    constexpr size_t decimalBase = 10;
    constexpr size_t numberBufferSize = 32;

    size_t getRawLength(const char* text)
    {
      if (text == nullptr) {
        return 0;
      }
      size_t count = 0;
      while (text[count] != '\0') {
        ++count;
      }
      return count;
    }

    char toLowerSymbol(char symbol)
    {
      if ((symbol >= 'A') && (symbol <= 'Z')) {
        return static_cast< char >(symbol - 'A' + 'a');
      }
      return symbol;
    }
  }
}

void yafu::String::ensureCapacity(size_t required)
{
  if ((required + 1) <= capacity_) {
    return;
  }

  size_t newCapacity = (capacity_ == 0) ? detail::initialStringCapacity : capacity_;
  while (newCapacity < (required + 1)) {
    newCapacity *= detail::stringGrowthFactor;
  }

  char* buffer = new char[newCapacity];
  for (size_t i = 0; i < length_; ++i) {
    buffer[i] = data_[i];
  }
  buffer[length_] = '\0';

  delete[] data_;
  data_ = buffer;
  capacity_ = newCapacity;
}

yafu::String::String():
  data_(nullptr),
  length_(0),
  capacity_(0)
{
  ensureCapacity(0);
  data_[0] = '\0';
}

yafu::String::String(const String& other):
  data_(nullptr),
  length_(0),
  capacity_(0)
{
  ensureCapacity(other.length_);
  for (size_t i = 0; i < other.length_; ++i) {
    data_[i] = other.data_[i];
  }
  length_ = other.length_;
  data_[length_] = '\0';
}

yafu::String::String(const char* text):
  data_(nullptr),
  length_(0),
  capacity_(0)
{
  const size_t count = detail::getRawLength(text);
  ensureCapacity(count);
  for (size_t i = 0; i < count; ++i) {
    data_[i] = text[i];
  }
  length_ = count;
  data_[length_] = '\0';
}

yafu::String::String(const char* text, size_t count):
  data_(nullptr),
  length_(0),
  capacity_(0)
{
  ensureCapacity(count);
  for (size_t i = 0; i < count; ++i) {
    data_[i] = text[i];
  }
  length_ = count;
  data_[length_] = '\0';
}

yafu::String::~String()
{
  delete[] data_;
}

yafu::String& yafu::String::operator=(const String& other)
{
  if (this == &other) {
    return *this;
  }

  length_ = 0;
  ensureCapacity(other.length_);
  for (size_t i = 0; i < other.length_; ++i) {
    data_[i] = other.data_[i];
  }
  length_ = other.length_;
  data_[length_] = '\0';
  return *this;
}

bool yafu::String::operator==(const String& other) const
{
  if (length_ != other.length_) {
    return false;
  }
  for (size_t i = 0; i < length_; ++i) {
    if (data_[i] != other.data_[i]) {
      return false;
    }
  }
  return true;
}

bool yafu::String::operator==(const char* text) const
{
  const size_t count = detail::getRawLength(text);
  if (count != length_) {
    return false;
  }
  for (size_t i = 0; i < length_; ++i) {
    if (data_[i] != text[i]) {
      return false;
    }
  }
  return true;
}

bool yafu::String::operator!=(const String& other) const
{
  return !(*this == other);
}

bool yafu::String::operator!=(const char* text) const
{
  return !(*this == text);
}

char yafu::String::operator[](size_t index) const
{
  return data_[index];
}

const char* yafu::String::getCString() const
{
  return data_;
}

size_t yafu::String::getLength() const
{
  return length_;
}

bool yafu::String::isEmpty() const
{
  return length_ == 0;
}

void yafu::String::append(const char* text)
{
  const size_t count = detail::getRawLength(text);
  ensureCapacity(length_ + count);
  for (size_t i = 0; i < count; ++i) {
    data_[length_ + i] = text[i];
  }
  length_ += count;
  data_[length_] = '\0';
}

void yafu::String::append(const String& other)
{
  ensureCapacity(length_ + other.length_);
  for (size_t i = 0; i < other.length_; ++i) {
    data_[length_ + i] = other.data_[i];
  }
  length_ += other.length_;
  data_[length_] = '\0';
}

void yafu::String::pushBack(char symbol)
{
  ensureCapacity(length_ + 1);
  data_[length_] = symbol;
  ++length_;
  data_[length_] = '\0';
}

void yafu::String::clear()
{
  length_ = 0;
  data_[0] = '\0';
}

yafu::String yafu::String::getSubstring(size_t position, size_t count) const
{
  if (position >= length_) {
    return String();
  }

  size_t available = count;
  if ((position + available) > length_) {
    available = length_ - position;
  }
  return String(data_ + position, available);
}

yafu::String yafu::String::getLowerCase() const
{
  String result(*this);
  for (size_t i = 0; i < result.length_; ++i) {
    result.data_[i] = detail::toLowerSymbol(result.data_[i]);
  }
  return result;
}

long yafu::String::findChar(char symbol) const
{
  for (size_t i = 0; i < length_; ++i) {
    if (data_[i] == symbol) {
      return static_cast< long >(i);
    }
  }
  return -1;
}

long yafu::String::findLastChar(char symbol) const
{
  for (size_t i = length_; i > 0; --i) {
    if (data_[i - 1] == symbol) {
      return static_cast< long >(i - 1);
    }
  }
  return -1;
}

long yafu::String::findSubstring(const char* part) const
{
  const size_t count = detail::getRawLength(part);
  if ((count == 0) || (count > length_)) {
    return -1;
  }

  for (size_t start = 0; (start + count) <= length_; ++start) {
    size_t matched = 0;
    while ((matched < count) && (data_[start + matched] == part[matched])) {
      ++matched;
    }
    if (matched == count) {
      return static_cast< long >(start);
    }
  }
  return -1;
}

bool yafu::String::equalsIgnoreCase(const String& other) const
{
  if (length_ != other.length_) {
    return false;
  }
  for (size_t i = 0; i < length_; ++i) {
    const char left = detail::toLowerSymbol(data_[i]);
    const char right = detail::toLowerSymbol(other.data_[i]);
    if (left != right) {
      return false;
    }
  }
  return true;
}

yafu::String yafu::String::createFromInt(long value)
{
  char buffer[detail::numberBufferSize] = {0};
  size_t position = detail::numberBufferSize - 1;
  buffer[position] = '\0';

  const bool isNegative = value < 0;
  unsigned long rest = isNegative
    ? static_cast< unsigned long >(-(value + 1)) + 1
    : static_cast< unsigned long >(value);

  if (rest == 0) {
    --position;
    buffer[position] = '0';
  }
  while (rest > 0) {
    --position;
    buffer[position] = static_cast< char >('0' + (rest % detail::decimalBase));
    rest /= detail::decimalBase;
  }
  if (isNegative) {
    --position;
    buffer[position] = '-';
  }

  return String(buffer + position);
}
