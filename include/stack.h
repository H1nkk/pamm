#pragma once

#include <stdexcept>
#include <iostream>

#define STR1(x)  #x
#define STR(x)  STR1(x)

const int MaxStackSize = 100000000;

template <typename T>
class Stack
{
public:
    Stack(size_t sizeToReserve = 0) : mCapacity(0), mSize(0), mpMemory(nullptr)
    {
        reserve(sizeToReserve);
    }

    Stack(const Stack& other)
    {
        mCapacity = other.mCapacity;
        mSize = other.mSize;
        mpMemory = new T[mSize];
        std::copy(other.mpMemory, other.mpMemory + mSize, mpMemory);
    }

    Stack(Stack&& other) noexcept
    {
        mCapacity = other.mCapacity;
        mSize = other.mSize;
        mpMemory = other.mpMemory;

        other.mSize = 0;
        other.mCapacity = 0;
        other.mpMemory = nullptr;
    }

    ~Stack()
    {
        delete[] mpMemory;
    }

    Stack& operator=(const Stack& other)
    {
        if (other.mCapacity > mCapacity)
        {
            T* tmp = new T[other.mCapacity];
            delete[] mpMemory;
            mpMemory = tmp;
            mCapacity = other.mCapacity;
        }

        mSize = other.mSize;
        std::copy(other.mpMemory, other.mpMemory + mSize, mpMemory);

        return *this;
    }

    Stack& operator=(Stack&& other) noexcept
    {
        if (mpMemory == other.mpMemory)
        {
            return *this;
        }

        mCapacity = other.mCapacity;
        mSize = other.mSize;
        mpMemory = other.mpMemory;

        other.mSize = 0;
        other.mCapacity = 0;
        other.mpMemory = nullptr;


        return *this;
    }

    size_t size() const noexcept
    {
        return mSize;
    }

    size_t capacity() const noexcept
    {
        return mCapacity;
    }

    bool empty() const noexcept
    {
        return size() == 0;
    }

    const T& top() const
    {
        if (empty())
        {
            throw std::logic_error("Stack is empty");
        }

        return mpMemory[size() - 1];
    }

    void pop()
    {
        if (empty())
        {
            throw std::logic_error("Can't pop from empty stack");
        }

        --mSize;
    }

    void push(const T& elem)
    {
        if (mSize == mCapacity)
        {
            expand();
        }

        mpMemory[mSize++] = elem;
    }

    void push(T&& elem)
    {
        if (mSize == mCapacity)
        {
            expand();
        }

        mpMemory[mSize++] = std::move(elem);
    }

    void reserve(size_t newCapacity)
    {
        if (newCapacity > MaxStackSize)
        {
            throw std::length_error("Maximum stack length is " STR(MaxStackSize));
        }

        if (newCapacity <= capacity())
        {
            return;
        }

        T* tmp = new T[newCapacity];

        if (mpMemory != nullptr)
        {
            std::copy(mpMemory, mpMemory + mSize, tmp);
            delete[] mpMemory;
        }

        mCapacity = newCapacity;
        mpMemory = tmp;
    }

    friend void swap(Stack& lhs, Stack& rhs) noexcept
    {
        std::swap(lhs.mSize, rhs.mSize);
        std::swap(lhs.mCapacity, rhs.mCapacity);
        std::swap(lhs.mpMemory, rhs.mpMemory);
    }

    friend std::ostream& operator<<(std::ostream& os, const Stack& s)
    {
        for (size_t i = 0; i < s.size(); ++i)
        {
            os << s.mpMemory[i] << ' ';
        }

        return os;
    }

private:
    T* mpMemory;
    size_t mCapacity;
    size_t mSize;

    void expand()
    {
        const size_t reallocationFactor = 2;

        size_t newCapacity = std::max((size_t)1, mCapacity * reallocationFactor);
        reserve(newCapacity);
    }
};