#ifndef YARMOLINSKAYA_HASH_TABLE_HPP
#define YARMOLINSKAYA_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace yarmolinskaya
{
  struct StringHash
  {
    size_t operator()(
      const std::string& str
    ) const
    {
      size_t result = 0;

      for (char c : str)
      {
        result =
          result * 131 +
          static_cast< unsigned char >(c);
      }

      return result;
    }
  };

  template< class Key, class Value >
  struct HashNode
  {
    std::pair< Key, Value > data;

    bool occupied;

    HashNode():
      data(),
      occupied(false)
    {}
  };

  template<
    class Key,
    class Value,
    class Hash = std::hash< Key >,
    class Equal = std::equal_to< Key >
  >
  class HashTable
  {
  public:
    using value_type =
      std::pair< Key, Value >;

    class iterator;

    class const_iterator;

    HashTable(
      size_t bucketCount,
      size_t bucketSize,
      size_t overflowSize
    ):
      data_(nullptr),
      bucketCount_(bucketCount),
      bucketSize_(bucketSize),
      overflowSize_(overflowSize),
      size_(0)
    {
      if (
        bucketCount_ == 0 ||
        bucketSize_ == 0
      )
      {
        throw std::logic_error(
          "invalid table size"
        );
      }

      data_ =
        new HashNode< Key, Value >[
          totalSize()
        ];
    }

    HashTable(
      const HashTable& rhs
    ):
      HashTable(
        rhs.bucketCount_,
        rhs.bucketSize_,
        rhs.overflowSize_
      )
    {
      for (
        size_t i = 0;
        i < totalSize();
        ++i
      )
      {
        data_[i] = rhs.data_[i];
      }

      size_ = rhs.size_;
    }

    HashTable(
      HashTable&& rhs
    ) noexcept:
      data_(rhs.data_),
      bucketCount_(rhs.bucketCount_),
      bucketSize_(rhs.bucketSize_),
      overflowSize_(rhs.overflowSize_),
      size_(rhs.size_)
    {
      rhs.data_ = nullptr;

      rhs.bucketCount_ = 0;

      rhs.bucketSize_ = 0;

      rhs.overflowSize_ = 0;

      rhs.size_ = 0;
    }

    ~HashTable()
    {
      delete[] data_;
    }

    HashTable& operator=(
      HashTable rhs
    )
    {
      swap(rhs);

      return *this;
    }

    void swap(
      HashTable& rhs
    ) noexcept
    {
      std::swap(data_, rhs.data_);

      std::swap(
        bucketCount_,
        rhs.bucketCount_
      );

      std::swap(
        bucketSize_,
        rhs.bucketSize_
      );

      std::swap(
        overflowSize_,
        rhs.overflowSize_
      );

      std::swap(size_, rhs.size_);
    }

    class iterator
    {
      friend class HashTable;

    public:
      iterator():
        current_(nullptr),
        end_(nullptr)
      {}

      value_type& operator*() const
      {
        return current_->data;
      }

      value_type* operator->() const
      {
        return std::addressof(
          current_->data
        );
      }

      iterator& operator++()
      {
        ++current_;

        while (
          current_ != end_ &&
          !current_->occupied
        )
        {
          ++current_;
        }

        return *this;
      }

      bool operator==(
        const iterator& rhs
      ) const
      {
        return current_ == rhs.current_;
      }

      bool operator!=(
        const iterator& rhs
      ) const
      {
        return !(*this == rhs);
      }

    private:
      iterator(
        HashNode< Key, Value >* current,
        HashNode< Key, Value >* end
      ):
        current_(current),
        end_(end)
      {}

      HashNode< Key, Value >* current_;

      HashNode< Key, Value >* end_;
    };

    class const_iterator
    {
      friend class HashTable;

    public:
      const_iterator():
        current_(nullptr),
        end_(nullptr)
      {}

      const_iterator(
        const iterator& rhs
      ):
        current_(rhs.current_),
        end_(rhs.end_)
      {}

      const value_type& operator*() const
      {
        return current_->data;
      }

      const value_type* operator->() const
      {
        return std::addressof(
          current_->data
        );
      }

      const_iterator& operator++()
      {
        ++current_;

        while (
          current_ != end_ &&
          !current_->occupied
        )
        {
          ++current_;
        }

        return *this;
      }

      bool operator==(
        const const_iterator& rhs
      ) const
      {
        return current_ == rhs.current_;
      }

      bool operator!=(
        const const_iterator& rhs
      ) const
      {
        return !(*this == rhs);
      }

    private:
      const_iterator(
        const HashNode< Key, Value >* current,
        const HashNode< Key, Value >* end
      ):
        current_(current),
        end_(end)
      {}

      const HashNode< Key, Value >* current_;

      const HashNode< Key, Value >* end_;
    };

    iterator begin()
    {
      HashNode< Key, Value >* ptr =
        data_;

      HashNode< Key, Value >* endPtr =
        data_ + totalSize();

      while (
        ptr != endPtr &&
        !ptr->occupied
      )
      {
        ++ptr;
      }

      return iterator(ptr, endPtr);
    }

    iterator end()
    {
      HashNode< Key, Value >* endPtr =
        data_ + totalSize();

      return iterator(endPtr, endPtr);
    }

    const_iterator begin() const
    {
      const HashNode< Key, Value >* ptr =
        data_;

      const HashNode< Key, Value >* endPtr =
        data_ + totalSize();

      while (
        ptr != endPtr &&
        !ptr->occupied
      )
      {
        ++ptr;
      }

      return const_iterator(ptr, endPtr);
    }

    const_iterator end() const
    {
      const HashNode< Key, Value >* endPtr =
        data_ + totalSize();

      return const_iterator(endPtr, endPtr);
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    bool contains(
      const Key& key
    ) const
    {
      return findNode(key) != nullptr;
    }

    Value& at(
      const Key& key
    )
    {
      HashNode< Key, Value >* node =
        findNode(key);

      if (!node)
      {
        throw std::out_of_range(
          "key not found"
        );
      }

      return node->data.second;
    }

    const Value& at(
      const Key& key
    ) const
    {
      const HashNode< Key, Value >* node =
        findNode(key);

      if (!node)
      {
        throw std::out_of_range(
          "key not found"
        );
      }

      return node->data.second;
    }

    bool insert(
      const Key& key,
      const Value& value
    )
    {
      if (contains(key))
      {
        return false;
      }

      value_type pair(key, value);

      size_t bucket =
        bucketIndex(key);

      size_t begin =
        bucket * bucketSize_;

      size_t end =
        begin + bucketSize_;

      if (
        insertIntoRange(
          pair,
          begin,
          end
        )
      )
      {
        ++size_;

        return true;
      }

      size_t overflowBegin =
        bucketCount_ * bucketSize_;

      size_t overflowEnd =
        overflowBegin + overflowSize_;

      if (
        insertIntoRange(
          pair,
          overflowBegin,
          overflowEnd
        )
      )
      {
        ++size_;

        return true;
      }

      throw std::overflow_error(
        "hash table overflow"
      );
    }

    bool erase(
      const Key& key
    )
    {
      HashNode< Key, Value >* node =
        findNode(key);

      if (!node)
      {
        return false;
      }

      node->occupied = false;

      --size_;

      return true;
    }

    void rehash(
      size_t bucketCount
    )
    {
      HashTable temp(
        bucketCount,
        bucketSize_,
        overflowSize_
      );

      for (
        const_iterator it = begin();
        it != end();
        ++it
      )
      {
        temp.insert(
          it->first,
          it->second
        );
      }

      swap(temp);
    }

  private:
    HashNode< Key, Value >* data_;

    size_t bucketCount_;

    size_t bucketSize_;

    size_t overflowSize_;

    size_t size_;

    Hash hash_;

    Equal equal_;

    size_t totalSize() const noexcept
    {
      return
        bucketCount_ * bucketSize_ +
        overflowSize_;
    }

    size_t bucketIndex(
      const Key& key
    ) const
    {
      return
        hash_(key) % bucketCount_;
    }

    bool insertIntoRange(
      const value_type& value,
      size_t begin,
      size_t end
    )
    {
      for (size_t i = begin; i < end; ++i)
      {
        if (!data_[i].occupied)
        {
          data_[i].data = value;

          data_[i].occupied = true;

          return true;
        }
      }

      return false;
    }

    HashNode< Key, Value >* findNode(
      const Key& key
    ) const
    {
      size_t bucket =
        bucketIndex(key);

      size_t begin =
        bucket * bucketSize_;

      size_t end =
        begin + bucketSize_;

      for (size_t i = begin; i < end; ++i)
      {
        if (
          data_[i].occupied &&
          equal_(
            data_[i].data.first,
            key
          )
        )
        {
          return data_ + i;
        }
      }

      size_t overflowBegin =
        bucketCount_ * bucketSize_;

      size_t overflowEnd =
        overflowBegin + overflowSize_;

      for (
        size_t i = overflowBegin;
        i < overflowEnd;
        ++i
      )
      {
        if (
          data_[i].occupied &&
          equal_(
            data_[i].data.first,
            key
          )
        )
        {
          return data_ + i;
        }
      }

      return nullptr;
    }
  };
}

#endif
