#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

	template <typename KeyType, typename ValueType>
	class TreeMap
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

	private:
		struct Node {
			Node* left = nullptr;
			Node* right = nullptr;
			Node* parent = nullptr;
			value_type data;
			Node() {}
			Node(const key_type& key) : left(nullptr), right(nullptr), data(std::make_pair(key, mapped_type{})) {}

			Node* searchMin(Node* node)
			{
				while (node->left)
					node = node->left;
				return node;
			}

			Node* searchMax(Node* node)
			{
				while (node->right)
					node = node->right;
				return node;
			}
		};
		Node* head = nullptr;
		size_t size = 0;


		const_iterator search(Node* startNode, const key_type& key) const
		{
			Node* next = startNode;
			Node* act = nullptr;
			while (next != nullptr)
			{
				act = next;
				if (act->data.first == key)
					return ConstIterator(act);
				if (act->data.first > key)
					next = act->left;
				else
					next = act->right;
			}
			return cend();
		}



	public:

		void initialize()
		{
			head = new Node;
			head->right = head;
			head->left = head;
			head->parent = nullptr;
		}

		void freeMemory(Node* node)
		{
			if (node->left != nullptr)
				freeMemory(node->left);
			if (node->right != nullptr)
				freeMemory(node->right);
			delete (node);
		}

		TreeMap()
		{
			initialize();
		}

		~TreeMap()
		{
			if (!isEmpty())
				freeMemory(head->left);
			if(head) delete head;
		}

		TreeMap(std::initializer_list<value_type> list)
		{
			initialize();
			for (auto element : list)
				operator[](element.first) = element.second;
		}

		TreeMap(const TreeMap& other)
		{
			initialize();
			if (other.isEmpty())
				return;
			for (auto it = other.begin(); it != other.end(); it++)
			{
				operator[](it->first) = it->second;
			}
		}

		TreeMap(TreeMap&& other)
		{
			head = other.head;
			other.head = nullptr;
			size = other.size;
			other.size = 0;
		}

		TreeMap& operator=(const TreeMap& other)
		{
			if (other.head == head)
				return *this;
			if (!isEmpty())
			{
				freeMemory(head->left);
				head->right = head;
				head->left = head;
				size = 0;
			}
			if (other.isEmpty())
				return *this;
	
			for (auto it = other.begin(); it != other.end(); it++)
				operator[](it->first) = it->second;

			return *this;
		}

		TreeMap& operator=(TreeMap&& other)
		{
			if (!isEmpty())
			{
				freeMemory(head->left);
				head->right = head;
				head->left = head;
			}
			Node* temp = head;
			head = other.head;
			other.head = temp;
			size = other.size;
			other.size = 0;
			return *this;
		}

		bool isEmpty() const
		{
			return size == 0;
		}

		mapped_type& operator[](const key_type& key)
		{
			if (!head)
				initialize();
			if (isEmpty())
			{
				Node* newNode = new Node(key);
				head->left = newNode;
				head->right = nullptr;
				newNode->parent = head;
				++size;
				return newNode->data.second;
			}
			Node* next = head->left;
			Node* act = nullptr;
			while (next != nullptr)
			{
				act = next;
				if (act->data.first == key)
					return act->data.second;
				if (act->data.first > key)
					next = act->left;
				else
					next = act->right;
			}
			Node* newNode = new Node(key);
			newNode->parent = act;
			if (key < act->data.first)
				act->left = newNode;
			else
				act->right = newNode;
			++size;
			return newNode->data.second;
		}

		const mapped_type& valueOf(const key_type& key) const
		{
			if (isEmpty()) throw std::out_of_range("empty map");
			ConstIterator found = find(key);
			if (found == cend()) throw std::out_of_range("key not found");
			return found->second;
		}

		mapped_type& valueOf(const key_type& key)
		{
			if (isEmpty()) throw std::out_of_range("empty map");
			Iterator found = find(key);
			if (found == cend()) throw std::out_of_range("key not found");
			return found->second;
		}

		const_iterator find(const key_type& key) const
		{
			if (isEmpty()) return cend();
			return search(head->left, key);
		}

		iterator find(const key_type& key)
		{
			if (isEmpty()) return end();
			return search(head->left, key);
		}

		void remove(const key_type& key)
		{
			remove(find(key));
		}

		void remove(const const_iterator& it)
		{
			if (isEmpty()) throw std::out_of_range("cannot remove from empty list");
			Node* node = it.node;
			if(node == head) throw std::out_of_range("cannot remove, element does not exist");
			auto tempIt = it;
			Node* succ = nullptr;

			if (!node->right && !node->left)
			{
				if(node == node->parent->left) node->parent->left = nullptr;
				else node->parent->right = nullptr;
			}
			else if (!node->left)
				succ = node->right;
			else if (!node->right)
				succ = node->left;
			else
				succ = (++tempIt).node;
			if (succ != head && succ)
			{
				succ->parent = node->parent;

				if (succ == succ->parent->left)
					succ->parent->left = nullptr;
				else
					succ->parent->right = nullptr;

				if (node == node->parent->left)
					node->parent->left = succ;
				else
					node->parent->right = succ;

				if (node->right && node->right != succ) succ->right = node->right;
				if (node->left && node->left != succ) succ->left = node->left;
			}
			delete node;
			--size;
			if (isEmpty())
			{
				head->left = head;
				head->right = head;
			}

		}

		size_type getSize() const
		{
			return size;
		}

		bool operator==(const TreeMap& other) const
		{
			if (size != other.size) return false;
			auto otherIt = other.cbegin();
			auto thisIt = cbegin();
			for (size_type i = 0; i < size; i++)
			{
				if (!(otherIt->first == thisIt->first && otherIt->second == thisIt->second))
					return false;
				++otherIt;
				++thisIt;
			}
			return true;
		}

		bool operator!=(const TreeMap& other) const
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
			Node* minNode = head;
			if (head == head->left) return ConstIterator(head); // empty map
			while (minNode->left)
				minNode = minNode->left;
			return ConstIterator(minNode);
		}

		const_iterator cend() const
		{
			return ConstIterator(head);
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
	class TreeMap<KeyType, ValueType>::ConstIterator
	{
	public:
		using reference = typename TreeMap::const_reference;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename TreeMap::value_type;
		using pointer = const typename TreeMap::value_type*;
		friend class TreeMap;
	private:
		Node* node;

	public:


		explicit ConstIterator()
		{}
		
		ConstIterator(Node* node): node(node)
		{}

		ConstIterator(const ConstIterator& other): node(other.node)
		{
		}

		ConstIterator& operator++()
		{
			if (!node->parent)
				throw std::out_of_range("can't increment end");
			if (node->right)
			{
				node = node->right;
				while (node->left)
					node = node->left;
				return *this;
			}
			else
			{
				Node* tmp = node->parent;
				while (tmp && node == tmp->right)
				{
					node = tmp;
					tmp = tmp->parent;
				}
				node = tmp;
				return *this;
			}
		}

		ConstIterator operator++(int)
		{
			ConstIterator temp(*this);
			operator++();
			return temp;
		}

		ConstIterator& operator--()
		{
			if(node ->right == node)
				throw std::out_of_range("Cannot decrement begin, empty map");
			if (node->left) 
			{
				node = node->left;
				while (node->right)
					node = node->right;
				return *this;
			}

			Node* tmp = node->parent;
			while (tmp && tmp->right != node)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			node = tmp;
			return *this;
		}

		ConstIterator operator--(int)
		{
			ConstIterator temp(*this);
			operator--();
			return temp;
		}

		reference operator*() const
		{
			if(node->parent == nullptr)
				throw std::out_of_range("Cannot dereference end");
			return node->data;
		}

		pointer operator->() const
		{
			return &this->operator*();
		}

		bool operator==(const ConstIterator& other) const
		{
			return node == other.node;
		}

		bool operator!=(const ConstIterator& other) const
		{
			return !(*this == other);
		}

	
	};

	template <typename KeyType, typename ValueType>
	class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
	{
	public:
		using reference = typename TreeMap::reference;
		using pointer = typename TreeMap::value_type*;

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

#endif /* AISDI_MAPS_MAP_H */
