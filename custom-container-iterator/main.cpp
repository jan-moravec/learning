#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

template <typename T>
class MyArray
{
public:
    typedef std::size_t size_type;

    /// Iterator inner class
    class iterator
    {
    public:
        /// Helper type to itself
        using self_type = iterator;
        /// Types needed for iterator
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        /// Define the category. Each category needs some functions
        /// http://www.cplusplus.com/reference/iterator/
        using iterator_category = std::forward_iterator_tag;

        /// Constructors, copy and move operators
        explicit iterator(pointer ptr = nullptr) : _ptr(ptr) {}
        iterator(const self_type &) = default;
        self_type &operator=(const self_type&) = default;
        iterator(self_type &&) = default;
        self_type &operator=(self_type&&) = default;

        /// Operators
        self_type &operator++() { _ptr++; return *this; }
        self_type operator++(int) { self_type i = *this; _ptr++; return i; }
        reference &operator*() { return *_ptr; }
        pointer operator->() { return _ptr; }
        bool operator==(const self_type& rhs) const { return _ptr == rhs._ptr; }
        bool operator!=(const self_type& rhs) const { return _ptr != rhs._ptr; }
    private:
        pointer _ptr;
    };

    /// Constructors, destructor, copy and move operators
    explicit MyArray(size_type size = 0) : _size(size) {
        if (_size) {
            _data = new T[_size];
        } else {
            _data = nullptr;
        }
    }
    MyArray(const MyArray &other): _size(other.size) {
        delete[] _data;
        if (_size) {
            _data = new T[_size];
            std::copy(&other._data[0], &other._data[0] + _size, &_data[0]);
        } else {
            _data = nullptr;
        }
    }
    MyArray &operator=(const MyArray &other) {
        if (&other == this) {
            return *this;
        }
        delete[] _data;
        if (_size) {
            _data = new T[_size];
            std::copy(&other._data[0], &other._data[0] + _size, &_data[0]);
        } else {
            _data = nullptr;
        }
        return *this;
    }
    MyArray(MyArray &&other) {
        delete[] _data;
        _data = other._data;
        _size = other._size;
        other._data = nullptr;
        other._size = 0;
    }
    MyArray &operator=(MyArray &&other) {
        if (&other == this) {
            return *this;
        }
        delete[] _data;
        _data = other._data;
        _size = other._size;
        other._data = nullptr;
        other._size = 0;
        return *this;
    }
    ~MyArray() {
        _size = 0;
        delete[] _data;
    }

    /// Get size of container
    size_type size() const { return _size; }

    T& operator[](size_type index)
    {
        assert(index < _size);
        return _data[index];
    }

    const T& operator[](size_type index) const
    {
        assert(index < _size);
        return _data[index];
    }

    iterator begin()
    {
        return iterator(_data);
    }

    iterator end()
    {
        return iterator(_data + _size);
    }

private:
    T* _data;
    size_type _size;
};


int main()
{
    MyArray<double> point3d(3);
    point3d[0] = 3.3;
    point3d[1] = 2.2;
    point3d[2] = 4.1;

    std::cout << "Test iterator for loop" << std::endl;
    for (MyArray<double>::iterator i = point3d.begin(); i != point3d.end(); ++i) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;

    std::cout << "Test range based for loop" << std::endl;
    for (auto &i : point3d) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Test std::copy into std::vector" << std::endl;
    std::vector<double> vec;
    std::copy(point3d.begin(), point3d.end(), std::inserter(vec, vec.begin()));
    for(std::vector<double>::iterator i = vec.begin(); i != vec.end(); ++i) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;

    // We cannot use sort, since random acces iterator would be needed. This means more operators support (+, -, >, <, ...)
    //std::cout << "Test std::sort" << std::endl;
    //std::sort(point3d.begin(), point3d.end());
    return 0;
}
