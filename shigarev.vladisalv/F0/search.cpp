#include "search.hpp"
#include "dynarray.hpp"
#include "hashtable.hpp"

namespace yafu {
  namespace {
    constexpr size_t indexCapacity = 10007;
    constexpr size_t trigramLength = 3;

    void extractTrigrams(const String& source, DynArray< String >& result)
    {
      const String value = source.getLowerCase();
      if (value.getLength() < trigramLength) {
        if (!value.isEmpty()) {
          result.pushBack(value);
        }
        return;
      }

      for (size_t start = 0; (start + trigramLength) <= value.getLength(); ++start) {
        result.pushBack(value.getSubstring(start, trigramLength));
      }
    }

    void addToIndex(HashTable< DynArray< size_t > >& index, const String& key, size_t entry)
    {
      DynArray< size_t >* bucket = index.find(key);
      if (bucket == nullptr) {
        DynArray< size_t > created;
        created.pushBack(entry);
        index.insert(key, created);
        return;
      }

      const bool alreadyAdded = !bucket->isEmpty()
        && ((*bucket)[bucket->getSize() - 1] == entry);
      if (!alreadyAdded) {
        bucket->pushBack(entry);
      }
    }

    void buildTrigramIndex(const DataSet& source, HashTable< DynArray< size_t > >& index)
    {
      for (size_t i = 0; i < source.getSize(); ++i) {
        DynArray< String > grams;
        extractTrigrams(source[i].name, grams);
        for (size_t gram = 0; gram < grams.getSize(); ++gram) {
          addToIndex(index, grams[gram], i);
        }
      }
    }

    void sortByScore(DynArray< size_t >& matched, const DynArray< long >& score)
    {
      for (size_t i = 1; i < matched.getSize(); ++i) {
        const size_t current = matched[i];
        size_t place = i;
        while ((place > 0) && (score[matched[place - 1]] < score[current])) {
          matched[place] = matched[place - 1];
          --place;
        }
        matched[place] = current;
      }
    }

    void searchExact(const DataSet& source, const String& pattern, bool byExtension,
      DataSet& result)
    {
      if (source.isEmpty() || pattern.isEmpty()) {
        return;
      }

      HashTable< DynArray< size_t > > index(indexCapacity);
      for (size_t i = 0; i < source.getSize(); ++i) {
        const String key = byExtension ? getExtension(source[i].name) : getBaseName(source[i].name);
        if (!key.isEmpty()) {
          addToIndex(index, key, i);
        }
      }

      const DynArray< size_t >* bucket = index.find(pattern);
      if (bucket == nullptr) {
        return;
      }

      for (size_t i = 0; i < bucket->getSize(); ++i) {
        result.pushBack(source[(*bucket)[i]]);
      }
    }
  }
}

yafu::String yafu::getBaseName(const String& fileName)
{
  const long dot = fileName.findLastChar('.');
  if (dot <= 0) {
    return fileName;
  }
  return fileName.getSubstring(0, static_cast< size_t >(dot));
}

yafu::String yafu::getExtension(const String& fileName)
{
  const long dot = fileName.findLastChar('.');
  if (dot <= 0) {
    return String();
  }
  return fileName.getSubstring(static_cast< size_t >(dot) + 1, fileName.getLength());
}

void yafu::searchByTrigrams(const DataSet& source, const String& pattern, DataSet& result)
{
  if (source.isEmpty()) {
    return;
  }

  HashTable< DynArray< size_t > > index(indexCapacity);
  buildTrigramIndex(source, index);

  DynArray< String > patternGrams;
  extractTrigrams(pattern, patternGrams);
  if (patternGrams.isEmpty()) {
    return;
  }

  DynArray< long > score;
  score.reserve(source.getSize());
  for (size_t i = 0; i < source.getSize(); ++i) {
    score.pushBack(0);
  }

  for (size_t gram = 0; gram < patternGrams.getSize(); ++gram) {
    const DynArray< size_t >* bucket = index.find(patternGrams[gram]);
    if (bucket != nullptr) {
      for (size_t i = 0; i < bucket->getSize(); ++i) {
        score[(*bucket)[i]] += 1;
      }
    }
  }

  DynArray< size_t > matched;
  for (size_t i = 0; i < source.getSize(); ++i) {
    if (score[i] > 0) {
      matched.pushBack(i);
    }
  }

  sortByScore(matched, score);
  for (size_t i = 0; i < matched.getSize(); ++i) {
    result.pushBack(source[matched[i]]);
  }
}

void yafu::searchByName(const DataSet& source, const String& pattern, DataSet& result)
{
  searchExact(source, pattern, false, result);
}

void yafu::searchByExtension(const DataSet& source, const String& pattern, DataSet& result)
{
  searchExact(source, pattern, true, result);
}
