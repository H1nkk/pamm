#pragma once
#include <vector>
#include <optional>

template <typename K, typename V>
class RedBlackTree
{
private:
    struct Node
    {
        enum Color { RED, BLACK };

        Node* pParent;
        Node* pLeft;
        Node* pRight;

        Color color;

        K key;
        V value;

        bool isLeaf;

        Node(Node* pParent_, Node* pLeft_, Node* pRight_, Color color_, K key_, V value_, bool isLeaf_) :
            pParent(pParent_), pLeft(pLeft_), pRight(pRight_), color(color_), key(key_), value(value_), isLeaf(isLeaf_)
        {
            if (pLeft != nullptr) pLeft->pParent = this;
            if (pRight != nullptr) pRight->pParent = this;
        }

        static Node* createLeave()
        {
            return new Node(nullptr, nullptr, nullptr, BLACK, {}, {}, true);
        }
    };

    Node* mpRoot;
    size_t mSize;

    bool isRightChild(const Node* pNode)
    {
        if (pNode->pParent == nullptr)
        {
            return false;
        }

        return pNode->pParent->pRight == pNode;
    }

    bool isLeftChild(const Node* pNode)
    {
        if (pNode->pParent == nullptr)
        {
            return false;
        }

        return pNode->pParent->pLeft == pNode;
    }

    Node* getGrandparent(const Node* pNode)
    {
        if (pNode != nullptr && pNode->pParent != nullptr)
        {
            return pNode->pParent->pParent;
        }

        return nullptr;
    }

    Node* getUncle(const Node* pNode)
    {
        Node* g = getGrandparent(pNode);
        if (g == nullptr)
        {
            return nullptr;
        }

        if (pNode->pParent == g->pLeft)
        {
            return g->pRight;
        } else
        {
            return g->pLeft;
        }
    }

    Node* getSibling(const Node* pNode)
    {
        if (isLeftChild(pNode))
        {
            return pNode->pParent->pRight;
        } else
        {
            return pNode->pParent->pLeft;
        }
    }

    void replaceNode(Node* pNode, Node* pChild)
    {
        pChild->pParent = pNode->pParent;

        if (pChild->pParent == nullptr)
        {
            mpRoot = pChild;
        } else
        {
            if (isLeftChild(pNode))
            {
                pNode->pParent->pLeft = pChild;
            } else
            {
                pNode->pParent->pRight = pChild;
            }
        }
    }

    void rotateLeft(Node* pNode)
    {
        Node* newp = pNode->pRight;

        newp->pParent = pNode->pParent;
        if (pNode->pParent != nullptr)
        {
            if (pNode == pNode->pParent->pLeft)
            {
                pNode->pParent->pLeft = newp;
            } else
            {
                pNode->pParent->pRight = newp;
            }
        } else
        {
            mpRoot = newp;
        }

        pNode->pRight = newp->pLeft;
        if (newp->pLeft != nullptr)
        {
            newp->pLeft->pParent = pNode;
        }

        pNode->pParent = newp;
        newp->pLeft = pNode;
    }

    void rotateRight(Node* pNode)
    {
        Node* newp = pNode->pLeft;

        newp->pParent = pNode->pParent;
        if (pNode->pParent != nullptr)
        {
            if (pNode->pParent->pLeft == pNode)
            {
                pNode->pParent->pLeft = newp;
            } else
            {
                pNode->pParent->pRight = newp;
            }
        } else
        {
            mpRoot = newp;
        }

        pNode->pLeft = newp->pRight;
        if (newp->pRight != nullptr)
        {
            newp->pRight->pParent = pNode;
        }

        pNode->pParent = newp;
        newp->pRight = pNode;
    }

    void fixInsert(Node* pNode)
    {
        if (pNode->pParent == nullptr) // pNode корень
        {
            // красим корень в красный
            pNode->color = Node::BLACK;
            return;
        }

        while (pNode->pParent != nullptr && pNode->pParent->color == Node::RED)
        {
            if (isLeftChild(pNode->pParent))
            {
                Node* pUncle = getUncle(pNode);
                Node* pGrand = getGrandparent(pNode);
                if (pUncle != nullptr && pUncle->color == Node::RED)
                {
                    pNode->pParent->color = Node::BLACK;
                    pUncle->color = Node::BLACK;
                    pGrand->color = Node::RED;
                    pNode = pGrand;
                } else
                {
                    if (isRightChild(pNode))
                    {
                        pNode = pNode->pParent;
                        rotateLeft(pNode);
                    }

                    pNode->pParent->color = Node::BLACK;
                    pGrand->color = Node::RED;
                    rotateRight(pGrand);
                }
            } else
            {
                Node* pUncle = getUncle(pNode);
                Node* pGrand = getGrandparent(pNode);
                if (pUncle != nullptr && pUncle->color == Node::RED)
                {
                    pNode->pParent->color = Node::BLACK;
                    pUncle->color = Node::BLACK;
                    pGrand->color = Node::RED;
                    pNode = pGrand;
                } else
                {
                    if (isLeftChild(pNode))
                    {
                        pNode = pNode->pParent;
                        rotateRight(pNode);
                    }
                    pNode->pParent->color = Node::BLACK;
                    pGrand->color = Node::RED;
                    rotateLeft(pGrand);
                }
            }
        }

        mpRoot->color = Node::BLACK;
    }

    void fixDelete(Node* pNode)
    {
        // pNode корень, дерево не сломано
        if (pNode->pParent == nullptr)
        {
            return;
        }

        Node* pSibling = getSibling(pNode);
        if (pSibling->color == Node::RED)
        {
            pNode->pParent->color = Node::RED;
            pSibling->color = Node::BLACK;
            if (isLeftChild(pNode))
            {
                rotateLeft(pNode->pParent);
            } else
            {
                rotateRight(pNode->pParent);
            }
        }

        pSibling = getSibling(pNode);

        // отец, брат и дети брата черные, можно просто поменять цвет брата, потом исправлять у отца
        if (pNode->pParent->color == Node::BLACK &&
            pSibling->color == Node::BLACK &&
            pSibling->pLeft->color == Node::BLACK &&
            pSibling->pRight->color == Node::BLACK)
        {
            pSibling->color = Node::RED;
            fixDelete(pNode->pParent);
            return;
        }

        // тоже самое, но отец красный, его красим в черный, а брата в красный
        if (pNode->pParent->color == Node::RED &&
            pSibling->color == Node::BLACK &&
            pSibling->pLeft->color == Node::BLACK &&
            pSibling->pRight->color == Node::BLACK)
        {
            pSibling->color = Node::RED;
            pNode->pParent->color = Node::BLACK;
            return;
        }


        if (isLeftChild(pNode) &&
            pSibling->pLeft->color == Node::RED &&
            pSibling->pRight->color == Node::BLACK)
        {
            pSibling->color = Node::RED;
            pSibling->pLeft->color = Node::BLACK;
            rotateRight(pSibling);
        } else if (isRightChild(pNode) &&
            pSibling->pLeft->color == Node::BLACK &&
            pSibling->pRight->color == Node::RED)
        {
            pSibling->color = Node::RED;
            pSibling->pRight->color = Node::BLACK;
            rotateLeft(pSibling);
        }

        pSibling = getSibling(pNode);

        pSibling->color = pNode->pParent->color;
        pNode->pParent->color = Node::BLACK;

        if (isLeftChild(pNode))
        {
            pSibling->pRight->color = Node::BLACK;
            rotateLeft(pNode->pParent);
        } else
        {
            pSibling->pLeft->color = Node::BLACK;
            rotateRight(pNode->pParent);
        }

        mpRoot->color = Node::BLACK;
    }

    void deleteZeroOneChild(Node* pNode)
    {
        Node* pChild = pNode->pRight->isLeaf ? pNode->pLeft : pNode->pRight;
        Node* pOtherChild = pNode->pRight->isLeaf ? pNode->pRight : pNode->pLeft;

        replaceNode(pNode, pChild);

        if (pNode->color == Node::BLACK)
        {
            if (pChild->color == Node::RED)
            {
                pChild->color = Node::BLACK;
            } else
            {
                fixDelete(pChild);
            }
        }

        delete pOtherChild;
        delete pNode;

        if (pChild->isLeaf && pChild == mpRoot)
        {
            mpRoot = nullptr;
            delete pChild;
        }
    }

    Node* getNext(Node* pNode)
    {
        pNode = pNode->pRight;

        while (!pNode->pLeft->isLeaf)
        {
            pNode = pNode->pLeft;
        }

        return pNode;
    }

    void traverse(const Node* pNode, std::vector<std::pair<K, V>>& dat)
    {
        if (pNode->isLeaf)
        {
            return;
        }

        traverse(pNode->pLeft, dat);
        dat.push_back({ pNode->key, pNode->value });
        traverse(pNode->pRight, dat);
    }

    std::pair<bool, int> checkNode(const Node* pNode)
    {
        if (pNode->isLeaf)
        {
            return { true, 1 };
        }

        auto res1 = checkNode(pNode->pLeft);
        auto res2 = checkNode(pNode->pRight);

        int h = res1.second;
        if (pNode->color == Node::BLACK) h++;

        if (pNode->color == Node::RED)
        {
            if (pNode->pLeft->color == Node::RED ||
                pNode->pRight->color == Node::RED)
            {
                return { false, h };
            }
        }

        if (!res1.first || !res2.first)
        {
            return { false, h };
        }

        if (res1.second != res2.second)
        {
            return { false, h };
        }

        return { true, h };
    }

public:
    RedBlackTree() : mpRoot(nullptr), mSize(0) {}

    size_t size() const noexcept { return mSize; }
    bool empty() const noexcept { return mSize == 0; }

    std::optional<V> find(const K& key) const
    {
        if (mpRoot == nullptr)
        {
            return std::nullopt;
        }

        Node* pCur = mpRoot;

        while (!pCur->isLeaf && pCur->key != key)
        {
            if (pCur->key < key)
            {
                pCur = pCur->pRight;
            } else
            {
                pCur = pCur->pLeft;
            }
        }

        if (pCur->isLeaf)
        {
            return std::nullopt;
        } else
        {
            return pCur->value;
        }
    }

    void insert(const K& key, const V& value)
    {
        if (mpRoot == nullptr)
        {
            mpRoot = new Node(nullptr, Node::createLeave(), Node::createLeave(), Node::RED, key, value, false);
            fixInsert(mpRoot);
            mSize++;
            return;
        }

        Node* pCur = mpRoot;

        while (!pCur->isLeaf && pCur->key != key)
        {
            if (pCur->key < key)
            {
                pCur = pCur->pRight;
            } else
            {
                pCur = pCur->pLeft;
            }
        }

        if (pCur->isLeaf)
        {
            Node* pNew;
            if (isLeftChild(pCur))
            {
                pNew = new Node(pCur->pParent, pCur, Node::createLeave(), Node::RED, key, value, false);
                pNew->pParent->pLeft = pNew;
            } else
            {
                pNew = new Node(pCur->pParent, pCur, Node::createLeave(), Node::RED, key, value, false);
                pNew->pParent->pRight = pNew;
            }
            fixInsert(pNew);
            mSize++;
        } else
        {
            pCur->value = value;
        }
    }

    bool erase(const K& key)
    {
        if (mpRoot == nullptr)
        {
            return false;
        }

        Node* pCur = mpRoot;

        while (!pCur->isLeaf && pCur->key != key)
        {
            if (pCur->key < key)
            {
                pCur = pCur->pRight;
            } else
            {
                pCur = pCur->pLeft;
            }
        }

        if (pCur->isLeaf)
        {
            return false;
        }

        // ноль или один ребенок
        if (pCur->pLeft->isLeaf ||
            pCur->pRight->isLeaf)
        {
            deleteZeroOneChild(pCur);
            mSize--;
            return true;
        }

        // два ребенка
        Node* pNext = getNext(pCur);

        pCur->key = pNext->key;
        pCur->value = pNext->value;

        deleteZeroOneChild(pNext);
        mSize--;

        return true;
    }

    bool isValidTree()
    {
        if (mpRoot == nullptr)
        {
            return true;
        }

        return checkNode(mpRoot).first;
    }

    std::vector<std::pair<K, V>> toVector()
    {
        std::vector<std::pair<K, V>> ans;

        if (mpRoot != nullptr)
        {
            traverse(mpRoot, ans);
        }

        return ans;
    }
};