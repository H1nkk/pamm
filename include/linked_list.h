#pragma once

#include <initializer_list>
#include <stdexcept>

template<typename T>
class LinkedList
{
private:
    struct Node
    {
        T data{};
        Node* next;
    };

    Node* mFirst;
    Node* mLast;
    size_t mSize;

public:
    struct Iterator
    {
    private:
        Node* mpCurrent;
        Node* mpNext;

        Iterator() noexcept : mpCurrent(nullptr), mpNext(nullptr) {}
        Iterator(Node* cur, Node* next) noexcept : mpCurrent(cur), mpNext(next) {}
        Iterator(Node* cur) noexcept : mpCurrent(cur), mpNext(cur != nullptr ? cur->next : nullptr) {}
    public:
        Iterator& operator++()
        {
            if (mpCurrent == nullptr && mpNext == nullptr)
            {
                throw std::out_of_range(__FUNCTION__ ": can't increment end() iterator.");
            }

            mpCurrent = mpNext != nullptr ? mpNext : mpCurrent->next;
            mpNext = mpCurrent != nullptr ? mpCurrent->next : nullptr;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const noexcept
        {
            return mpCurrent == other.mpCurrent;
        }

        bool operator!=(const Iterator& other) const noexcept
        {
            return !(*this == other);
        }

        const T& operator*() const
        {
            if (mpCurrent == nullptr)
            {
                throw std::out_of_range(__FUNCTION__ ": can't dereference end() or before_begin() iterator.");
            }

            return mpCurrent->data;
        }

        friend class LinkedList;
    };

    LinkedList() : mSize(0), mFirst(nullptr), mLast(nullptr) {}
    LinkedList(const std::initializer_list<T>& elems) : mSize(0), mFirst(nullptr), mLast(nullptr)
    {
        for (const auto& elem : elems)
        {
            pushBack(elem);
        }
    }

    LinkedList(const LinkedList& other) : mSize(0), mFirst(nullptr), mLast(nullptr)
    {
        for (const auto& elem : other)
        {
            pushBack(elem);
        }
    }

    LinkedList(LinkedList&& other) noexcept : mSize(other.mSize), mFirst(other.mFirst), mLast(other.mLast)
    {
        other.mSize = 0;
        other.mFirst = nullptr;
        other.mLast = nullptr;
    }

    ~LinkedList()
    {
        clear();
    }

    LinkedList& operator=(const LinkedList& other)
    {
        if (mFirst == other.mFirst)
        {
            return *this;
        }

        clear();
        for (const auto& elem : other)
        {
            pushBack(elem);
        }
    }

    LinkedList& operator=(LinkedList&& other)
    {
        if (mFirst == other.mFirst)
        {
            return *this;
        }

        clear();
        mFirst = other.mFirst;
        mLast = other.mLast;
        mSize = other.mSize;
        other.mFirst = nullptr;
        other.mLast = nullptr;
        other.mSize = 0;
    }

    size_t size() const noexcept { return mSize; }
    bool empty() const noexcept { return size() == 0; }

    void pushFront(const T& elem)
    {
        Node* nw = new Node{ elem, mFirst };
        mFirst = nw;
        if (mLast == nullptr)
        {
            mLast = nw;
        }

        ++mSize;
    }

    void pushBack(const T& elem)
    {
        Node* nw = new Node{ elem, nullptr };
        if (mLast != nullptr)
        {
            mLast->next = nw;
        }
        mLast = nw;
        if (mFirst == nullptr)
        {
            mFirst = nw;
        }

        ++mSize;
    }

    void popFront()
    {
        if (empty())
        {
            throw std::out_of_range(__FUNCTION__ ": can't pop front element of empty list.");
        }

        if (mFirst == mLast)
        {
            delete mFirst;
            mFirst = nullptr;
            mLast = nullptr;
        } else
        {
            Node* tmp = mFirst;
            mFirst = mFirst->next;
            delete tmp;
        }

        --mSize;
    }

    void popBack()
    {
        if (empty())
        {
            throw std::out_of_range(__FUNCTION__ ": can't pop back element from empty list.");
        }

        if (mLast == mFirst)
        {
            delete mLast;
            mFirst = nullptr;
            mLast = nullptr;
        } else
        {
            Node* cur = mFirst;
            for (; cur->next != mLast; cur = cur->next) {}
            cur->next = nullptr;
            Node* tmp = mLast;
            mLast = cur;
            delete tmp;
        }

        --mSize;
    }

    void clear()
    {
        while (!empty())
        {
            popFront();
        }
    }

    const T& back()
    {
        if (empty())
        {
            throw std::out_of_range(__FUNCTION__ ": can't get back from empty list.");
        }

        return mLast->data;
    }

    const T& front()
    {
        if (empty())
        {
            throw std::out_of_range(__FUNCTION__ ": can't get front from empty list.");
        }

        return mFirst->data;
    }

    Iterator insertAfter(const Iterator& pos, const T& data)
    {
        if (pos.mpNext == nullptr && pos.mpCurrent == nullptr)
        {
            throw std::out_of_range(__FUNCTION__": can't insert elements after end() iterator.");
        }

        if (pos.mpNext == nullptr)
        {
            pushBack(data);
            return Iterator(mLast);
        }

        if (pos.mpCurrent == nullptr)
        {
            pushFront(data);
            return Iterator(mFirst);
        }

        Node* nw = new Node{ data, pos.mpNext };
        pos.mpCurrent->next = nw;

        ++mSize;

        return Iterator(nw);
    }

    void eraseAfter(const Iterator& pos)
    {
        if (pos.mpNext == nullptr)
        {
            throw std::out_of_range(__FUNCTION__ ": can't erase after end of list.");
        }

        if (pos.mpCurrent == nullptr)
        {
            popFront();
            return;
        }

        if (pos.mpNext->next == nullptr)
        {
            popBack();
            return;
        }

        pos.mpCurrent->next = pos.mpNext->next;
        delete pos.mpNext;
        --mSize;
    }

    Iterator begin() const noexcept { return Iterator(mFirst); }
    Iterator before_begin() const noexcept { return Iterator(nullptr, mFirst); }
    Iterator before_end() const noexcept { return Iterator(mLast, nullptr); }
    Iterator end() const noexcept { return Iterator(); }
};