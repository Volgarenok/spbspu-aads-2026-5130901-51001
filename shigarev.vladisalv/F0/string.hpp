#ifndef YAFU_STRING_HPP
#define YAFU_STRING_HPP

#include <cstddef>

namespace yafu {
  class String
  {
  public:
    String();
    String(const String& other);
    explicit String(const char* text);
    String(const char* text, size_t count);
    ~String();

    String& operator=(const String& other);
    bool operator==(const String& other) const;
    bool operator==(const char* text) const;
    bool operator!=(const String& other) const;
    bool operator!=(const char* text) const;
    char operator[](size_t index) const;

    const char* getCString() const;
    size_t getLength() const;
    bool isEmpty() const;

    void append(const char* text);
    void append(const String& other);
    void pushBack(char symbol);
    void clear();

    String getSubstring(size_t position, size_t count) const;
    String getLowerCase() const;

    long findChar(char symbol) const;
    long findLastChar(char symbol) const;
    long findSubstring(const char* part) const;

    bool equalsIgnoreCase(const String& other) const;

    static String createFromInt(long value);

  private:
    char* data_;
    size_t length_;
    size_t capacity_;

    void ensureCapacity(size_t required);
  };
}

#endif
