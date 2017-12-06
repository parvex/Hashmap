
#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <atomic>
#include <forward_list>
#include <LinkedList.h>
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
  using list = aisdi::LinkedList<value_type>;

public:
	friend class HashMap::ConstIterator;

private:
	static const unsigned buckets = 16384;
	size_t firstHash = {};
	size_t lastHash = {};
	list** tab = {};
	size_t size = {};
public:

	HashMap() : tab(new list*[buckets]())
  {}

  ~HashMap()
  {
	  clearMemory();
  }
  void clearMemory()
  {
	  if (tab)
	  {
		  for (size_t i = firstHash; i <= lastHash; ++i)
			  if (tab[i])
			  {
				  delete tab[i];
				  tab[i] = nullptr;
			  }
		  delete tab;
	  }
  }

  size_t getHash(key_type key)
  {
	  return ((unsigned)(key) * 12 + 3321) % buckets;
  }


  HashMap(std::initializer_list<value_type> list) : tab(new aisdi::LinkedList<value_type>*[buckets]())
  {
	  for (auto element : list)
		  operator[](element.first) = element.second;

  }

  HashMap(const HashMap& other) : tab(new list*[buckets]())
  {
	  if (other.isEmpty()) return;
	  for (auto it = other.begin(); it != other.end(); it++)
		  operator[](it->first) = it->second;
  }

  HashMap(HashMap&& other)
  {
	  tab = other.tab;
	  other.tab = nullptr;
	  size = other.size;
	  firstHash = other.firstHash;
	  lastHash = other.lastHash;
	  other.firstHash = 0;
	  other.lastHash = 0;
	  other.size = 0;
  }

  HashMap& operator=(const HashMap& other)
  {
	  if (!tab) tab = new list*[buckets]();
	  if (other.tab == tab) return *this;
	  for (size_t i = firstHash; i <= lastHash; ++i)
		  if (tab[i])
		  {
			  delete tab[i];
			  tab[i] = nullptr;
		  }

	  for (auto it = other.begin(); it != other.end(); it++)
		  operator[](it->first) = it->second;
	  firstHash = other.firstHash;
	  lastHash = other.lastHash;
	  size = other.size;

	  return *this;

  }

  HashMap& operator=(HashMap&& other)
  {
	  clearMemory();
	  tab = other.tab;
	  other.tab = nullptr;
	  size = other.size;
	  other.size = 0;
	  firstHash = other.firstHash;
	  lastHash = other.lastHash;
	  other.firstHash = 0;
	  other.lastHash = 0;
	  return *this;
  }

  bool isEmpty() const
  {
	  return size == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
	  if (!tab) tab = new list*[buckets]();
	  size_t hash = getHash(key) % buckets;
	  if (hash < firstHash || isEmpty()) 
		  firstHash = hash;
	  if (hash > lastHash) 
		  lastHash = hash;
	  if (!tab[hash])
	  {
		  tab[hash] = new list;
		  tab[hash]->append(std::make_pair(key, mapped_type{}));
		  ++size;
		  return (*tab[hash]).getlast().second;

	  }
	  auto it = tab[hash]->begin();
	  for (; it!=tab[hash]->end(); ++it)
	  {
		  if (it->first == key) break;
	  }
	  if (it != tab[hash]->end()) return it->second;
	  else tab[hash]->append(std::make_pair(key, mapped_type{}));
	  ++size;
	  return (*tab[hash]).getlast().second;
	  
  }

  const mapped_type& valueOf(const key_type& key) const
  {
	  if (isEmpty()) throw std::out_of_range("out of range");
	  auto it = find(key);
	  if (it != end()) return it->second;
	  else throw std::out_of_range("out of range");
  }

  mapped_type& valueOf(const key_type& key)
  {
	  if (isEmpty()) throw std::out_of_range("out of range");
	  auto it = find(key);
	  if (it != end()) return it->second;
	  else throw std::out_of_range("out of range");

  }

  const_iterator find(const key_type& key) const
  {
	  if (isEmpty()) return cend();
	  auto it = cbegin();
	  for (; it != end(); ++it)
		  if (it->first == key) break;
	  return it;
  }

  iterator find(const key_type& key)
  {
	  if (isEmpty()) return cend();
	  auto it = begin();
	  for (; it != end(); ++it)
	  {
		  if (it->first == key)
			  break;
	  }
	  return it;
  }

  void remove(const key_type& key)
  {
	  remove(find(key));
  }

  void remove(const const_iterator& it)
  {
	  if (it == cend()) throw std::out_of_range("element doesn't exist");
	  list& vec = (*tab[it.hashnr]);
	  vec.erase(it.it);
	  --size;
	  if (it.hashnr == firstHash)
	  {
		  size_t i;
		  for (i = it.hashnr + 1; i <= lastHash + 1 && tab[i] == nullptr; ++i);

		  if (tab[i]) firstHash = i;
	  }
	  else if (it.hashnr == lastHash)
	  {
		  size_t i;
		  for (i = it.hashnr; i >= firstHash && tab[i] == nullptr; --i);
		  if (tab[i]) lastHash = i;
	  }
  }

  size_type getSize() const
  {
	  return size;
  }

  bool operator==(const HashMap& other) const
  {
	  if (size != other.size) return false;
	  auto it1 = cbegin();
	  auto it2 = other.cbegin();
	  bool equal = true;
	  for (; it1 != end() && it2 != other.end(); ++it1, ++it2)
		  if (it1->first != it2->first || it1->second != it2->second) equal = 0;
	  return equal;
  }

  bool operator!=(const HashMap& other) const
  {
	  return !(*this == other);
  }

  iterator begin()
  {
	  return cbegin();

  }

  iterator end()
  {
	  return cend();
  }

  const_iterator cbegin() const
  {		
	  if (isEmpty()) return ConstIterator(firstHash, *this, true);
	  else return ConstIterator(firstHash, *this);

  }

  const_iterator cend() const
  {
	  return ConstIterator(0, *this, true);
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
  friend class HashMap;
private:
	size_t hashnr;
	typename LinkedList<value_type>::Iterator it;
	const HashMap& mapref;
	bool end = 0;
	
public:

  explicit ConstIterator()
  {}

  ConstIterator(size_t hashnr,const HashMap&  mapref, bool end = 0): hashnr(hashnr), mapref(mapref), end(end)
  {
	  if (!mapref.tab)
	  {
		  end = true; return;
	  }
	  if (end) return;
	  it = mapref.tab[hashnr]->begin();
  }

  ConstIterator(const ConstIterator& other):  hashnr(other.hashnr), it(other.it), mapref(other.mapref), end(other.end)
  {
  }

  ConstIterator& operator++()
  {
	  if (!mapref.tab)
	  {
		  end = true; return *this;
	  }
	  if (end) throw std::out_of_range("can't increment end");
	  ++it;
	  if (it != mapref.tab[hashnr]->end())
		  return *this;

	  size_t i;
	  for (i = hashnr+1; i <= mapref.lastHash+1 && mapref.tab[i] == nullptr; ++i)
	  {
		  ;
	  }

	  if (mapref.tab[i] && i < mapref.buckets)
	  {
		  it = mapref.tab[i]->begin();
		  hashnr = i;
	  }
	  else 
		  end = true;


	  return *this;
   }

  ConstIterator operator++(int)
  {
	  auto result = *this;
	  ConstIterator::operator++();
	  return result;
  }

  ConstIterator& operator--()
  {
	  if (end && !mapref.isEmpty())
	  {
		  hashnr = mapref.lastHash;
		  it = mapref.tab[hashnr]->begin();
		  end = false;
		  return *this;
	  }
	  else if (end) throw std::out_of_range("out of range");

	  if (it != mapref.tab[hashnr]->begin()) 
		  --it;
	  else
	  {
		  unsigned i;
		  for (i = hashnr; i >= mapref.firstHash && mapref.tab[i] == nullptr; --i);
		  if (mapref.tab[i])
			  it = mapref.tab[hashnr]->begin();
		  else end = true;
	  }
	  return *this;
  }

  ConstIterator operator--(int)
  {
	  auto result = *this;
	  ConstIterator::operator--();
	  return result;
  }

  reference operator*() const
  {
	  if (end) throw std::out_of_range("dereferencing end");
	  return *it;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
	  return (end == 1 && other.end == 1) || (other.hashnr == hashnr && other.mapref.tab == mapref.tab && other.it == it && other.end == end);
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
