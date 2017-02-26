#ifndef PVECTOR_HPP_
#define PVECTOR_HPP_

constexpr unsigned int pvectorSizeChange = 32;

template<class T>
class pvector {
private:
	T* items;
	unsigned int _size;
	unsigned int capacity;

public:
	class iterator {
	private:
		T* pos;
	public:
		iterator(T* p)
			: pos(p) {}

		T operator*(void) const {
			return *pos;
		}

		template<typename N>
		iterator operator+(N inc) const {
			return pos + inc;	
		}

		iterator& operator++(void) {
			++pos;
			return *this;
		}

		template<typename N>
		iterator operator-(N inc) const {
			return pos - inc;
		}

		bool operator>(const iterator& i) const {
			return pos > i.pos;
		}

		bool operator<(const iterator& i) const {
			return pos < i.pos;
		}

		bool operator!=(const iterator& i) const {
			return pos != i.pos;
		}
	};

	pvector(void)
		: items(new T[pvectorSizeChange]), _size(0),
		capacity(pvectorSizeChange) {}

	~pvector(void) {
		delete items;
	}

	inline iterator begin(void) {
		return iterator(items);
	}

	inline iterator end(void) {
		return iterator(items + _size);
	}

	inline unsigned int size(void) const {
		return _size;
	}

	T& push_back(T& t) {
		if (_size >= capacity) {
			auto nItems = new T[capacity + pvectorSizeChange];
			for (unsigned int i = 0; i < capacity; i++)
				nItems[i] = items[i];
			delete items;
			items = nItems;
		}

		auto& r = items[_size++] = t;
		return r;
	}

	template<typename... Args>
	T& emplace_back(Args... args) {
		if (_size >= capacity) {
			auto nItems = new T[capacity + pvectorSizeChange];
			for (unsigned int i = 0; i < capacity; i++)
				nItems[i] = items[i];
			delete items;
			items = nItems;
		}
		auto& r = items[_size++] = T(args...);
		return r;
	}

	void erase(iterator it) {
		for (unsigned int i = it - begin(); i < capacity; i++)
			items[i] = items[i + 1];
		_size--;
	}

	void clear(void) {
		delete items;
		items = new T[pvectorSizeChange];
		_size = 0;
		capacity = pvectorSizeChange;
	}
};

#endif // PVECTOR_HPP_
