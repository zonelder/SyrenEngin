#pragma once

#include <vector>

template<typename Entity,unsigned N>
class SparseSet
{
public:


	using key_type = Entity;
	using densed_container_type = std::vector<key_type>;
	using sparse_container_type = std::vector<key_type>;

	static constexpr key_type tombstone = N-1;


	class reverse_iterator
	{
	public:

		reverse_iterator(densed_container_type::iterator it):_curIt(it)

		auto operator++(int) const noexcept
		{
			auto copy = *this;
			++(*this);
			return copy;
		}

		auto operator++() noexcept
		{
			--_curIt;
			return *this;
		}

		bool operator==(const reverse_iterator& other) const noexcept
		{
			return _curIt == other._curIt;
		}

		bool operator!=(const reverse_iterator& other) const noexcept
		{
			return _curIt != other._curIt;
		}

		auto& operator*() const noexcept
		{
			return *_curIt;
		}

	private:
		densed_container_type::iterator _curIt;
	};

	SparseSet() noexcept : _sparse(N, tombstone),_densed(N,tombstone)
	{
		//reserve(N);
		//_data.reserve(capacity);
	}

	auto begin() noexcept
	{
		// using reverse_iterator to enshure that modification of SparseArray
		// during iteration will not trigger any exeptions
		return _densed.begin();//reverse_iterator(densed.begin() + _data.size()); 
	}

	auto end() noexcept
	{
		return _densed.end();
	}

	bool contains(const key_type& key) const noexcept
	{
		return _sparse[key] != tombstone;
	}


	key_type& operator[](const key_type& key) const noexcept
	{
		return _sparse[key];
	}

	void add(const key_type& key) noexcept
	{
		if (contains(key))
			return this->operator[key];

		auto pos = _densed.size();
		_densed.push_back(key);
		_sparse[key] = pos;
	}

	bool remove(const key_type& key) noexcept
	{
		auto& pos = _sparse[key];
		if (pos == tombstone)// Data not exist
			return false;

		const auto last = _densed.back();
		_sparse[last] = pos;
		pos = tombstone;
		std::swap(_densed[pos],_densed.back());
		_densed.pop_back();
		return true;
	}

	void clear() noexcept
	{
		_sparse.clear();
		_densed.clear();
	}

	virtual ~SparseSet() {};
		 
private:

	sparse_container_type _sparse;
	densed_container_type _densed;

};