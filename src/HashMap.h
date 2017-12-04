
#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <atomic>
#include <forward_list>
namespace aisdi
{


template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using list = std::vector<value_type>;
  typename std::forward_list<value_type>::iterator listit;
public:


private:
	static const unsigned buckets = 16384;
	size_t firstHash = {};
	size_t lastHash = {};
	list* tab[buckets] = {};

public:

  HashMap()
  {}

  ~HashMap()
  {
	  for (int i = firstHash; i <= lastHash; ++i)
	  {
		  if (tab[i])
			  delete tab[i];
	  }
  }

  size_t getHash(key_type key)
  {
	  return ((unsigned)(key) * 12 + 3321) % buckets;
  }


  HashMap(std::initializer_list<value_type> list)
  {
    (void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");
  }

  HashMap(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap& operator=(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap& operator=(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
    throw std::runtime_error("TODO");
  }

  mapped_type& operator[](const key_type& key)
  {
	  size_t hash = getHash(key) % buckets;
	  if (hash < firstHash) firstHash = hash;
	  if (hash > lastHash) lastHash = hash;
	  if (!tab[hash])
	  {
		  tab[hash] = new list;
		  tab[hash]->push_back(std::make_pair(key, mapped_type{}));
		  return (*tab[hash])[((*tab[hash]).size()) - 1].second;

	  }
	  auto it = tab[hash]->begin();
	  for (; it!=tab[hash]->end(); ++it)
	  {
		  if (it->first == key) break;
	  }
	  if (it != tab[hash]->end()) return it->second;
	  else tab[hash]->push_back(std::make_pair(key, mapped_type{}));
	  return (*tab[hash])[((*tab[hash]).size()) - 1].second;
	  
  }

  const mapped_type& valueOf(const key_type& key) const
  {
	 return (find(key)).second;

  }

  mapped_type& valueOf(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  const_iterator find(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  iterator find(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
    throw std::runtime_error("TODO");
  }

  bool operator==(const HashMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    throw std::runtime_error("TODO");
\
  }

  iterator end()
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

private:
	size_t hashnr;
	size_t vecnr;
	bool end = 0;

public:

  explicit ConstIterator()
  {}

  ConstIterator(size_t vecnr, size_t hashnr) vecnr(vecnr), hashnr(hashnr)
  {}

  ConstIterator(const ConstIterator& other): vecnr(other.vecnr), hashnr(other.hashnr)
  {
  }

  ConstIterator& operator++()
  {
	  if (end) throw std::out_of_range("can't increment end");
	  
	  if (vecnr < (*(tab[hashnr])).size() - 1)
	  {
		  ++vecnr;
		  return;
	  }

	  for (size_t i = hashnr; i <= lastHash; ++i)
	  {

	  }



	  return *this;
   }

  ConstIterator operator++(int)
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
    throw std::runtime_error("TODO");
  }

  reference operator*() const
  {
    throw std::runtime_error("TODO");
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
