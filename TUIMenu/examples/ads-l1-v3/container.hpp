#pragma once

#include <cstddef>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <ostream>


template<typename T>
class list_v3
{
private:
    struct list_item; // объявление структуры элемента

public:
    struct list_iterator; // объявление структуры итераторы

    using value_type             = T;
    using size_type              = size_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = list_iterator;
    using const_iterator         = const list_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    struct list_iterator
    {
        friend list_v3;

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = ptrdiff_t;
        using pointer           = value_type*;
        using const_pointer     = const pointer;

        list_iterator() = default;

        list_iterator(const list_v3* const owner, list_item* ptr)
            : _owner(owner), _ptr(ptr) {}

        iterator&
        operator++()
        {
            // проверка на конец контейнера
            if (_ptr->_next < 0)
                _ptr = _owner->_border;
            else
                _ptr = _owner->_items[_ptr->_next];

            return *this;
        }

        iterator
        operator++(int)
        {
            // сохранение состояния итератора
            auto old = *this;
            // итерирование
            ++(*this);
            return old;
        }

        reference
        operator*() // получение ссылки на значение итератора
        { return _ptr->_value; }

        const_reference
        operator*() const
        { return _ptr->_value; }

        pointer
        operator->() // получение указателя на значение итератора
        { return &_ptr->_value; }

        const_pointer
        operator->() const
        { return &_ptr->_value; }

        bool
        operator==(const_iterator& rhs) const
        { return _ptr == rhs._ptr; }

        bool
        operator!=(const_iterator& rhs) const
        { return !(*this== rhs); }

    private:
        const list_v3* const _owner;
        list_item*  _ptr;
    };

    list_v3() = default;

    // резервирующий конструктор
    list_v3(size_t cap)
    { reserve(cap); }

    // конструктор копирования
    list_v3(const list_v3<T>& src)
    { *this = src; }

    ~list_v3()
    { clear(); }

    reference
    operator[](size_t pos)
    {
        if (pos >= _capacity)
            std::out_of_range("pos is out of range!");

        list_item*& item_ptr = _items[pos];

        // создание элемента, если в ячейке nullptr
        if (!item_ptr) {
            item_ptr = replace(pos);
            _size++;
        }

        return _items[pos]->_value;
    }

    const_reference
    operator[](size_t pos) const
    {
        // снятие константности с указателя на объект
        list_v3<value_type>& tmp = *const_cast<list_v3<value_type>*>(this);
        // вызов неконстантной версии оператора
        return tmp[pos];
    }

    // копирующий оператор присваивания
    list_v3<T>&
    operator=(const list_v3<T>& rhs)
    {
        // удаление старых элементов
        clear();
        // выделение памяти под новые
        allocate(rhs._capacity);

        _size = rhs._size;

        // цикл копирования
        for (size_t n = 0; n < _capacity; n++) {
            list_item* item_ptr = rhs._items[n];

            if (item_ptr)
                _items[n] = new list_item(*item_ptr);
        }

        return *this;
    }

    // оператор вывода в поток
    friend std::ostream&
    operator<<(std::ostream& os, const list_v3<value_type>& rhs)
    {
        if (rhs.empty())
            return os << "[]";

        size_t item_n = 0;
        os << "[ ";

        for (const auto& item : rhs) {
            os << item;
            if (++item_n < rhs.size())
                os << ", ";    
        }

        os << " ]";
        return os;
    }

    size_type
    size() const
    { return _size; }

    size_type
    capacity() const
    { return _capacity; }

    void
    clear()
    {
        if (_size == 0 && _capacity == 0)
            return;

        for (size_t n = 0; n < _size; n++)
            delete _items[n];

        delete[] _items;

        _size      = 0;
        _capacity  = 0;
    }

    bool
    empty() const
    { return !_size; }

    void
    reserve(size_t new_cap)
    {
        if (new_cap <= _capacity)
            return;

        if (!_items) // == nullptr
            return allocate(new_cap);

        size_t first_index  = get_index(_first);
        size_t old_capacity = _capacity;
        list_item** old_ptr = _items;
        allocate(new_cap);

        // копирование указателей на элементы
        for (size_t n = 0; n < old_capacity; n++)
            _items[n] = old_ptr[n];

        _first = _items[first_index];
        _last  = _first + (_size > 0 ? _size - 1 : 0);
        delete[] old_ptr;
    }

    iterator
    beging()
    { return iterator(this, _first); }

    iterator
    end()
    { return iterator(this, _border); }

    const_iterator
    begin() const
    { return iterator(this, _first); }

    const_iterator
    end() const
    { return iterator(this, _border); }

    reference
    front()
    { return _first->_value; }

    reference
    back()
    { return _last->_value; }

    const_reference
    front() const
    { return _first->_value; }

    const_reference
    back() const
    { return _last->_value; }

    void
    shift(ssize_t pos_from, ssize_t pos_to, ssize_t offset)
    {
        if (pos_from < 0 || pos_to > _capacity)
            throw std::out_of_range("pos is out of range!");

        if (offset == 0)
            return;
        else if (offset > 0)
            shift_front(pos_from, pos_to, offset);
        else if (offset < 0)
            shift_back(pos_from, pos_to, -offset);
    }

    iterator
    insert(iterator pos, const_reference value)
    { return insert(get_index(pos._ptr), value); }

    // вставка после позиции
    iterator
    insert(ssize_t pos, const_reference value)
    {
        if (pos + 1 > _capacity)
            throw std::out_of_range("pos is out of range!");

        reserve(_capacity + 1);
        shift(pos + 1, _size, 1);

        list_item* item_ptr = replace(pos + 1, &value);
        _size++;

        if (item_ptr->_next < 0)
            _last = item_ptr;

        return iterator(this, item_ptr);
    }

    list_item*
    replace(size_t pos, const value_type* value = nullptr)
    {
        if (pos >= _capacity)
            throw std::out_of_range("pos is out of range!");

        delete _items[pos];

        int nearest_prev = nearest_index(pos, false);
        auto item_ptr    = new list_item();
        item_ptr->_next  = nearest_index(pos, true);
        
        if (nearest_prev < 0)
            _first = item_ptr;
        else
            _items[nearest_prev]->_next = pos;

        if (item_ptr->_next < 0)
            _last = item_ptr;

        if (value)
            item_ptr->_value = *value;;

        _items[pos] = item_ptr;

        return item_ptr;
    }

    void
    push_back(const_reference value)
    {
        if (!_first) {
            insert(-1, value);
            return;
        }

        reserve(_capacity + 1);
        replace(_capacity - 1, &value);
        _size++;
        _last = _items[_size - 1];
    }

    void
    push_front(const_reference value)
    {
        reserve(_capacity + 1);
        shift(0, _capacity, 1);
        replace(0, &value);
        _size++;
        _first = _items[0];
    }

    iterator
    erase(iterator pos)
    {
        if (pos == end())
            return end();

        return erase(get_index(pos._ptr));
    }

    iterator
    erase(size_t pos)
    {
        if (pos >= _capacity)
            throw std::out_of_range("pos is out of range!");

        int next = delete_item(pos);
        shift(pos + 1, _capacity, -1);

        _size--;

        return (next > 0 ? iterator(this, _items[next - 1]) : end());
    }

    iterator
    find(const_reference value) const
    {
        iterator iter = begin();

        for (; iter != end(); ++iter) {
            if (*iter == value)
                return iter;
        }

        return iter; // end()
    }

private:
    struct list_item
    {
        friend list_v3;

    public:
        list_item() = default;

        list_item(const_reference value, int next)
            : _next(next), _value(value) {}

        list_item(const list_item& item) // конструктор копирования
            : list_item(item._value, item._next) {}

    private:
        int _next = -1;
        value_type _value;
    };

    size_t _size       = 0;
    size_t _capacity   = 0;
    list_item* _first  = nullptr;
    list_item* _last   = nullptr;
    list_item* _border = nullptr;
    list_item** _items = nullptr;


    // выделение памяти под хранение элементов контейнера
    void
    allocate(size_t cap)
    {
        _items    = new list_item*[cap]();
        _capacity = cap;
    }

    int
    nearest_index(size_t pos, bool forward)
    {
        if (forward)
            return nearest_next(pos);
        
        return nearest_prev(pos);
    }

    int
    nearest_next(size_t pos)
    {
        if (pos + 1 == _capacity)
            return -1;

        for (size_t n = pos + 1; n < _capacity; n++) {
            if (_items[n]) // если элемент не nullptr
                return n;
        }

        return -1;
    }

    int
    nearest_prev(size_t pos)
    {
        if (pos == 0)
            return -1;

        for (size_t n = pos - 1; n >= 0; n--) {
            if (_items[n]) // если элемент не nullptr
                return n;
        }

        return -1;
    }

    int
    nearest_remap(size_t pos)
    {
        int prev = nearest_prev(pos);
        int next = nearest_next(pos);
        
        if (prev >= 0)
            _items[prev]->_next = next;

        return next;
    }

    void
    shift_front(ssize_t pos_from, ssize_t pos_to, ssize_t offset)
    {
        for (ssize_t n = pos_to - 1; n >= pos_from; n--) {
            // ежели сдвиг уместен
            if (n + offset < _capacity) {
                // ежели сдвиг _next уместен
                if (_items[n]->_next + offset < _capacity)
                    _items[n]->_next += offset;
                // иначе - текущий элемент последний в контейнере
                else
                    _items[n]->_next = -1;

                // сдвиг
                _items[n + offset] = _items[n];
                _items[n]          = nullptr;
            }
            // если с учётом сдвига элемент выходит
            // за границы контейнера, то он удаляется
            else if (_items[n]) {
                delete_item(n);
                _size--;
            }
        }
    }

    void
    shift_back(ssize_t pos_from, ssize_t pos_to, ssize_t offset)
    {
        for (ssize_t n = pos_from; n < pos_to; n++) {
            // ежели сдвиг не выходит за границы
            if (n - offset >= 0) {
                _items[n - offset] = _items[n];
                _items[n]          = nullptr;
            }
            // иначе удаляем элемент
            else if (_items[n]) {
                delete_item(n);
                _size--;
            }
        }
    }

    size_t
    get_index(const list_item* item_ptr)
    {
        size_t pos = 0;

        for (; pos < _capacity; pos++) {
            if (item_ptr == _items[pos])
                break;
        }

        return pos;
    }

    int
    delete_item(size_t pos)
    {
        delete _items[pos];

        _items[pos] = nullptr;
        return nearest_remap(pos);
    }
};



































































template <typename T>
using list_V3 = std::vector<T>;

