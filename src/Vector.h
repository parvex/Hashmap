#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:

	size_t capacity = 0;
	size_t size = 0;
	Type* tab = NULL;

public:

  Vector()
  {
  }

  Vector(std::initializer_list<Type> l)
  {
	  size = l.size();
	  size_t computeAlloc;
	  for(computeAlloc = 8; computeAlloc <= size; computeAlloc *= 2);
	  capacity = computeAlloc;
	  tab = new Type[capacity];
	  unsigned i = 0;
	  for (auto iter = l.begin(); iter != l.end(); ++i, ++iter)
		  tab[i] = *iter;
  }

  Vector(const Vector& other)
  {
	  size = other.size;
	  capacity = other.capacity;
	  tab = new Type[capacity];

	  for (size_t i = 0; i < size; ++i)
		  tab[i] = other[i];
  }

  Vector(Vector&& other)
  {
	  size = other.size;
	  capacity = other.capacity;
	  tab = other.tab;
	  other.tab = NULL;
	  other.size = 0;
	  other.capacity = 0;
  }

  ~Vector()
  {
	  delete[] tab;
  }

  Vector& operator=(const Vector& other)
  {
	size = other.size;
	for (unsigned i = 0; i < other.size; ++i)
    {
		tab[i] = other[i];
	}
	return *this;
  }

  Vector& operator=(Vector&& other)
  {
	  erase(cbegin(), cend());
	  size = other.size;
	  capacity = other.capacity;
	  if(tab) delete[] tab;
	  tab = other.tab;
	  other.tab = NULL;
	  other.size = 0;
	  other.capacity = 0;

	  return *this;
  }

  bool isEmpty() const
  {
	  return size == 0;
  }

  size_type getSize() const
  {
	  return size;
  }

  void append(const Type& item)
  {
	  insert(cend(), item);
  }

  void prepend(const Type& item)
  {
	  insert(cbegin(), item);
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
	  unsigned position = insertPosition.index;
	  if (size >= capacity)
	  {
		  for (capacity = 8; capacity <= size; capacity *= 2);
		  Type* temp = new Type[capacity];
		  if (tab)
		  {
			  for (unsigned i = 0; i < position; i++)
				  temp[i] = tab[i];
			  for (unsigned i = position; i < size; i++)
				  temp[i + 1] = tab[i];
			  delete[] tab;
		  }
		  tab = temp;
	  }
	  else
	  {
		  for (unsigned i = size; i != position; --i)
			  tab[i] = tab[i - 1];
	  }
	  tab[position] = item;
	  ++size;
  }

  Type popFirst()
  {
		Type temp = *cbegin();
		erase(begin(), begin() + 1);
		return temp;
  }

  Type popLast()
  {
	Type temp = *(cend() - 1);
	erase(end() - 1, end());
	return temp;
  }

  void erase(const const_iterator& possition)
  {
	  erase(possition, possition + 1);
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
	  unsigned first = firstIncluded.index;
	  unsigned last = lastExcluded.index;
	  if (size && first >= 0 && first < size && last <= size && last >= 0)
	  {

		  for (unsigned i = first; i < last; ++i)
			  tab[i].~value_type();
		  for (unsigned i = last, j = 0; i < size; ++i, ++j)
		  {
			  tab[first + j] = tab[i];
		  }

		  size = size - last + first;
	  }
	  else
		  throw std::out_of_range("Out of range");
  }

  iterator begin()
  {
	  return iterator(this, 0);
  }

  iterator end()
  {
	  iterator test(this, size);
	  return iterator(this, size);
  }

  const_iterator cbegin() const
  {
	  return const_iterator(this, 0);
  }

  const_iterator cend() const
  {
	  const_iterator test(this, size);
	  return test;
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

  const Type& operator[](unsigned i) const
  {
	  return tab[i];
  }
  Type& operator[](unsigned i)
  {
	  return tab[i];
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
  friend class Vector<Type>;
private:
	const Vector* vecPtr;
	size_t index;

public:

  explicit ConstIterator()
  {
  }

  ConstIterator(const ConstIterator& other) : vecPtr(other.vecPtr), index(other.index) {}

  ConstIterator(const Vector* ptr, size_t indx) : vecPtr(ptr), index(indx) {}

  reference operator*() const
  {
	  if (index < vecPtr->getSize())
		  return (*vecPtr)[index];
	  else
		  throw std::out_of_range("Out of range!");
  }

  ConstIterator& operator++()
  {	
	  if (index < vecPtr->getSize())
	  {
		  index++;
		  return *this;
	  }
	  else
		  throw std::out_of_range("Out of range!");
  }

  ConstIterator operator++(int)
  {
	  ConstIterator iterator(*this);   
	  ++(*this);              
	  return iterator;
  }

  ConstIterator& operator--()
  {
	  if (index)
	  {
		  index--;
		  return *this;
	  }
	  else
		  throw std::out_of_range("Out of range");
  }

  ConstIterator operator--(int)
  {

	  if (index)
	  {
		  ConstIterator iterator(*this);
		  ++(*this);
		  return iterator;
	  }
	  else
		  throw std::out_of_range("Out of range");
  }

  ConstIterator operator+(difference_type d) const
  {
	  return ConstIterator(vecPtr, index+d);
  }

  ConstIterator operator-(difference_type d) const
  {
	  return ConstIterator(vecPtr, index-d);
  }

  bool operator==(const ConstIterator& other) const
  {
	  return other.vecPtr == vecPtr && other.index == index;
  }

  bool operator!=(const ConstIterator& other) const
  {
	  return !(other.vecPtr == vecPtr && other.index == index);
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;


public:

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other) : 
	  ConstIterator(other) 
  {}

  Iterator(const Vector* ptr, size_t indx) : ConstIterator(ptr, indx) {}

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

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
