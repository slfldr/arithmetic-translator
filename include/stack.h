#pragma once

#include <iostream>
#include <string>
#include <map>

template<typename T>
class sqVector
{
protected:
    size_t size;
    size_t capacity;

    T* data;

public:
    sqVector(size_t sz = 0) : size(sz), capacity(sz * 2 + 2)
    {
        data = new T[capacity];

        if (data == nullptr)
        {
            throw std::bad_alloc();
        }
    }

    sqVector(T* arr, size_t sz) : size(sz), capacity(sz * 2 + 2)
    {
        data = new T[capacity];

        if (data == nullptr)
        {
            throw std::bad_alloc();
        }
        else
        {
            std::copy(arr, arr + sz, data);
        }
    }

    sqVector(const sqVector& t) : size(t.size), capacity(t.capacity)
    {
        data = new T[capacity];

        if (data == nullptr)
        {
            throw std::bad_alloc();
        }
        else
        {
            std::copy(t.data, t.data + t.size, data);
        }
    }

    sqVector(sqVector&& t) noexcept
    {
        data = nullptr;

        swap(*this, t);
    }

    ~sqVector()
    {
        delete[] data;

        data = nullptr;
    }

    sqVector& operator=(const sqVector& v)
    {
        if (&v == this)
        {
            return *this;
        }

        if (size != v.size)
        {
            T* p = new T[v.capacity];

            if (p == nullptr)
            {
                throw std::bad_alloc();
            }
            else
            {
                delete[] data;

                data = nullptr;
                data = p;
                size = v.size;
                capacity = v.capacity;

                p = nullptr;
            }
        }

        std::copy(v.data, v.data + v.size, data);

        return *this;
    }

    sqVector& operator=(sqVector&& v) noexcept
    {
        delete[] data;

        data = nullptr;

        swap(*this, v);

        return *this;
    }

    friend void swap(sqVector& lhs, sqVector& rhs)
    {
        std::swap(lhs.size, rhs.size);
        std::swap(lhs.capacity, rhs.capacity);
        std::swap(lhs.data, rhs.data);
    }

    size_t size_() const noexcept
    {
        return size;
    };
    size_t capacity_() const noexcept
    {
        return capacity;
    };

    bool isEmpty() const noexcept
    {
        return (size == 0);
    };
    bool isFull() const noexcept
    {
        return (size == capacity);
    };

    bool operator==(const sqVector& t) const noexcept
    {
        if (size != t.size)
        {
            return 0;
        }
        else
        {
            for (std::size_t i = 0; i < size; i++)
            {
                if (data[i] != t.data[i])
                {
                    return 0;
                }
            }
        }

        return 1;
    }
    bool operator!=(const sqVector& t) const noexcept
    {
        return (!(*this == t));
    }

    T operator[](std::size_t ind)
    {
        return data[ind];
    }
    const T operator[](std::size_t ind) const
    {
        return data[ind];
    }

    T back()
    {
        if (!(this -> isEmpty()))
        {
            return data[size - 1];
        }
        else
        {
            throw std::logic_error("Vector is empty");
        }
    };

    void resize(int new_size)
    {
        T* mem = new T[new_size * 2 + 2];

        if (mem != nullptr)
        {
            std::copy(data, data + capacity, mem);

            delete[] data;

            capacity = new_size * 2 + 2;
            data = mem;

            mem = nullptr;
        }
    }
    void push_back(T elem)
    {
        insert(elem, size);
    }
    void push_front(T elem)
    {
        insert(elem, 0);
    }
    void pop_back()
    {
        if (size != 0)
        {
            size--;
        }
        else
        {
            throw std::logic_error("Vector is empty");
        }
    }
    void pop_front()
    {
        erase(0);
    }
    void insert(T elem, std::size_t ind)
    {
        if ((this -> isFull()))
        {
            this -> resize(size_t(size_t((int)size + 1)));
        }

        for (std::size_t i = size; i > ind; i--)
        {
            data[i] = data[i - 1];
        }

        data[ind] = elem;

        size++;
    }
    void erase(std::size_t ind)
    {
        if (!(this -> isEmpty()))
        {
            for (std::size_t i = ind; i < size; i++)
            {
                data[i] = data[i + 1];
            }

            size--;
        }
        else
        {
            throw std::logic_error("Vector is empty");
        }
    }
};

template<typename T> class mStack : private sqVector<T>
{
    using sqVector<T>::size;
    using sqVector<T>::capacity;
    using sqVector<T>::data;
    using sqVector<T>::pop_back;
    using sqVector<T>::push_back;
    using sqVector<T>::back;
    using sqVector<T>::resize;

public:
    mStack() : sqVector<T>(0) {}

    using sqVector<T>::isFull;
    using sqVector<T>::isEmpty;
    using sqVector<T>::size_;
    using sqVector<T>::capacity_;
    using sqVector<T>::operator[];

    void push(T elem)
    {
        this -> push_back(elem);
    }
    void pop()
    {
        this -> pop_back();
    }
    T top()
    {
        return this -> back();
    }
};

template<typename T> class mQueue : private sqVector<T>
{
    using sqVector<T>::size;
    using sqVector<T>::capacity;
    using sqVector<T>::data;
    using sqVector<T>::resize;

    std::size_t front, back;

public:
    mQueue() : sqVector<T>(0), front(0), back(0) {}

    using sqVector<T>::isFull;
    using sqVector<T>::isEmpty;
    using sqVector<T>::size_;
    using sqVector<T>::capacity_;

    void push(T elem)
    {
        if (this -> isFull())
        {
            this -> resize(size_t(size_t((int)size + 1)));
        }

        data[back] = elem;

        size++;

        if (back == capacity - 1)
        {
            back = 0;
        }
        else
        {
            back++;
        }
    }
    void pop()
    {
        if (!this -> isEmpty())
        {
            if (front == capacity - 1)
            {
                front = 0;
            }
            else
            {
                front++;
            }

            size--;
        }
    }

    T get_front()
    {
        return data[front];
    }
    T get_back()
    {
        if (back == 0)
        {
            return data[capacity - 1];
        }
        else
        {
            return data[back - 1];
        }
    }
};

template<typename T> class mQueue2Stacks : private sqVector<mStack<T>>
{
private:
    using::sqVector<mStack<T>>::data;

    std::size_t size, capacity;

public:
    mQueue2Stacks() : size(data[0].size_() + data[1].size_()), capacity(data[0].capacity_() + data[1].capacity_()) {}

    size_t size_() const noexcept
    {
        return size;
    };
    size_t capacity_() const noexcept
    {
        return capacity;
    };

    bool isEmpty() const noexcept
    {
        return (data[0].isEmpty() && data[1].isEmpty());
    };
    bool isFull() const noexcept
    {
        return (data[0].isFull() && data[1].isFull());
    };

    void push(T elem)
    {
        data[0].push(elem);

        size++;
    }
    void pop()
    {
        if (data[1].isEmpty())
        {
            while (!data[0].isEmpty())
            {
                T mem = data[0].top();

                data[1].push(mem);
                data[0].pop();
            }
        }
        data[1].pop();

        size--;
    }

    T back()
    {
        if (!data[1].isEmpty())
        {
            return data[1][0];
        }
        if (!data[0].isEmpty())
        {
            return data[0][size];
        }
        else
        {
            return 0;
        }
    }
    T front()
    {
        if (!data[1].isEmpty())
        {
            return data[1][size];
        }
        if (!data[0].isEmpty())
        {
            return data[0][0];
        }
        else
        {
            return 0;
        }
    }
};