#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
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
	struct Node 
	{
		Type elem;
		Node* next = NULL;
		Node* prev = NULL;
		Node()  {}
		Node(const Type el) : elem(el){}
	};


	Node* head = NULL;
	Node* last = NULL;
	size_t size = 0;

public:

  LinkedList()
  {}

  LinkedList(std::initializer_list<Type> l)
  {
	  for (auto it = l.begin(); it != l.end(); ++it)
		  append(*it);
  }

  LinkedList(const LinkedList& other)
  {
	  for (auto it = other.begin(); it != other.end(); ++it)
		  append(*it);
	  
  }

  LinkedList(LinkedList&& other)
  {
	  erase(begin(), end());
	  size = other.size;
	  head = other.head;
	  last = other.last;
	  other.head = NULL;
	  other.last = NULL;
	  other.size = 0;
  }

  ~LinkedList()
  {
	  erase(begin(), end());
  }

  value_type& operator[](const unsigned i)
  {
	  auto it = ConstIterator(begin() + i);
	  return *it;
  }

  LinkedList& operator=(const LinkedList& other)
  {
	  if (&other == this) 
		  return *this;
	  erase(begin(), end());
	  for (auto it = other.cbegin(); it != other.cend(); ++it)
		  append(*it);
	  return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
	  erase(begin(), end());
	  size = other.size;
	  head = other.head;
	  last = other.last;
	  other.head = NULL;
	  other.last = NULL;
	  other.size = 0;
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
	  insert(end(), item);
  }

  void prepend(const Type& item)
  {
	  insert(begin(), item);
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
	  Node* addedNode = new Node(item);
	  if (insertPosition == cbegin())
	  {
		  if (head)
		  {
			  addedNode->next = head;
			  head->prev = addedNode;
			  head = addedNode;
		  }
		  else
			  head = last = addedNode;
	  }
	  else if (insertPosition == cend())
	  {
		  if (last)
		  {
			  addedNode->prev = last;
			  last->next = addedNode; ////
			  last = addedNode;
		  }
		  else
			  head = last = addedNode;
	  }
	  else
	  {
		  addedNode->next = const_cast<Node*>(insertPosition.node);
		  addedNode->prev = const_cast<Node*>(insertPosition.node->prev);
		  addedNode->prev->next = addedNode;
		  addedNode->next->prev = addedNode;
	  }
	  ++size;
  }

  Type& getlast()
  {
	  return last->elem;
  }

  Type popFirst()
  {
	  Type temp = *(cbegin());
	  erase(cbegin(), cbegin()+1);
	  return temp;
  }

  Type popLast()
  {
	  Type temp = *(cend() - 1);
	  erase(cend() - 1, cend());
	  return temp;
  }

  void erase(const const_iterator& possition)
  {
    erase(possition, possition+1);
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
	  if (isEmpty() || firstIncluded == lastExcluded || firstIncluded.node == NULL) return;
	  Node* firstNode = const_cast<Node*>(firstIncluded.node);
	  Node* lastNode = const_cast<Node*>(lastExcluded.node);
	  if (firstNode == head) 
	  { 
		  head = lastNode; 
		  if(head)
			head->prev = NULL;
	  }
	  else 
		  firstNode->prev->next = lastNode;
	  if (lastNode == NULL)
	  {
		  last = firstNode->prev;
		  if(last)
			last->next = NULL;
	  }
	  else
		  lastNode->prev = firstNode->prev;
	  Node* it = firstNode;
	  while(it != lastNode)
	  {
		  it = firstNode->next;
		  delete firstNode;
		  firstNode = it;
		  --size;
	  }
  }

  iterator begin()
  {
	  return iterator(cbegin());
  }

  iterator end()
  {
	  return iterator(cend());
  }

  const_iterator cbegin() const
  {
	  return const_iterator(this, head);
  }

  const_iterator cend() const
  {
	  return const_iterator(this, nullptr);
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

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;
  friend LinkedList<Type>;
 
private:
	const LinkedList* listPtr;
	const Node* node;
public:

  explicit ConstIterator() : listPtr(NULL), node(NULL)
  {}

  ConstIterator(const LinkedList* listPt, const Node* nod) : listPtr(listPt), node(nod) {}

  reference operator*() const
  {
	  if (node) return node->elem;
	  else throw std::out_of_range("Out of range");
  }

  ConstIterator& operator++()
  {
	  if (node) node = node->next;
	  else throw std::out_of_range("Out of range");
	  return *this;
  }

  ConstIterator operator++(int)
  {
	  ConstIterator temp(*this);
	  operator++();
	  return temp;
  }

  ConstIterator& operator--()
  {
	  if (node)
		  if (node->prev)
			  node = node->prev;
		  else throw std::out_of_range("Out of range");
	  else
		  if (listPtr->last)
			  node = listPtr->last;
	      else throw std::out_of_range("Out of range");
	  return *this;
  }

  ConstIterator operator--(int)
  {
	  ConstIterator temp(*this);
	  operator--();
	  return temp;
  }

  ConstIterator operator+(difference_type d) const
  {
	auto iter = *this;
	for (int i = 0; i < d; i++)
		++iter;
	return iter;
  }

  ConstIterator operator-(difference_type d) const
  {
	  auto iter = *this;
	  for (int i = 0; i < d; i++)
		  --iter;
	  return iter;
  }

  pointer operator->() const
  {
	  return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
	  return listPtr == other.listPtr && node == other.node;
  }

  bool operator!=(const ConstIterator& other) const
  {
	  return !(listPtr == other.listPtr && node == other.node);
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

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
  pointer operator->() const
  {
	  return &this->operator*();
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
