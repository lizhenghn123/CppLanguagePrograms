// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Thread safety
// -------------
//
// Writes require external synchronization, most likely a mutex.
// Reads require a guarantee that the SkipList will not be destroyed
// while the read is in progress.  Apart from that, reads progress
// without any internal locking or synchronization.
//
// Invariants:
//
// (1) Allocated nodes are never deleted until the SkipList is
// destroyed.  This is trivially guaranteed by the code since we
// never delete any skip list nodes.
//
// (2) The contents of a Node except for the next/prev pointers are
// immutable after the Node has been linked into the SkipList.
// Only Insert() modifies the list, and it is careful to initialize
// a node and use release-stores to publish the nodes in one or
// more lists.

//! NOT THREAD SAFETY! lizhenghn@gmail.com !!!

#ifndef ZL_SKIPLIST_H
#define ZL_SKIPLIST_H
#include <string.h>
#include <assert.h>
#include <sstream>
#include <string>

namespace zl { namespace stl {

template <typename Key, class Comparator>
class SkipList
{
    struct Node;
public:
    // Create a new SkipList object that will use "cmp" for comparing keys,
    explicit SkipList(Comparator cmp);

    // insert key_ into the list.
    void insert(const Key& key);

    // Returns true iff an entry that compares equal to key_ is in the list.
    bool contains(const Key& key) const;

    // Delete the key iff exist
    void remove(const Key& key);

    // dump skiplist
    std::string dump() const;

    // Iteration over the contents of a skip list
    class Iterator
    {
    public:
        // Initialize an iterator over the specified list.
        // The returned iterator is not valid.
        explicit Iterator(const SkipList* list);

        // Returns true iff the iterator is positioned at a valid node.
        bool valid() const;

        // Returns the key_ at the current position.
        // REQUIRES: valid()
        const Key& key() const;

        // Advances to the next position.
        // REQUIRES: valid()
        void next();

        // Advances to the previous position.
        // REQUIRES: valid()
        void prev();

        // Advance to the first entry with a key_ >= target
        void Seek(const Key& target);

        // Position at the first entry in list.
        // Final state of iterator is valid() iff list is not empty.
        void seekToFirst();

        // Position at the last entry in list.
        // Final state of iterator is valid() iff list is not empty.
        void seekToLast();

    private:
        const SkipList* list_;
        Node* node_;
        // Intentionally copyable
    };

private:
    inline int getMaxHeight() const
    {
        return max_height_;
    }

    inline bool equal(const Key& a, const Key& b) const
    {
        return (compare_(a, b) == 0);
    }

    Node* newNode(const Key& key, int height);

    void deleteNode(Node* x, Node **update);

    int randomHeight();

    // Return true if key_ is greater than the data stored in "n"
    bool keyIsAfterNode(const Key& key, Node* n) const;

    // Return the earliest node that comes at or after key_.
    // Return NULL if there is no such node.
    //
    // If prev is non-NULL, fills prev[level] with pointer to previous
    // node at "level" for every level in [0..max_height_-1].
    Node* findGreaterOrEqual(const Key& key, Node** prev) const;

    // Return the latest node with a key_ < key_.
    // Return head_ if there is no such node.
    Node* findLessThan(const Key& key) const;

    // Return the last node in the list.
    // Return head_ if list is empty.
    Node* findLast() const;

    // No copying allowed
    SkipList(const SkipList&);
    void operator=(const SkipList&);

private:
    enum { kMaxHeight = 17 };

    // Immutable after construction
    Comparator const compare_;

    Node* const head_;

    int max_height_;   // Height of the entire list
};

// Implementation details follow
template<typename Key, class Comparator>
struct SkipList<Key, Comparator>::Node
{
    explicit Node(const Key& k)
        : key_(k)
    { }

    ~Node()
    { }

    Node* next(int n)
    {
        assert(n >= 0);
        return next_[n];
    }

    void setNext(int n, Node* x)
    {
        assert(n >= 0);
        next_[n] = x;
    }

public:
    Key const key_;

    // Array of length equal to the node height.  next_[0] is lowest level link.
    Node* next_[1];
};

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node*
SkipList<Key, Comparator>::newNode(const Key& key, int height)
{
    char* mem = (char*)malloc(sizeof(Node) + sizeof(Node*) * (height - 1));
    return new (mem)Node(key);
}

template<typename Key, class Comparator>
inline SkipList<Key, Comparator>::Iterator::Iterator(const SkipList* list)
{
    list_ = list;
    node_ = NULL;
}

template<typename Key, class Comparator>
inline bool SkipList<Key, Comparator>::Iterator::valid() const
{
    return node_ != NULL;
}

template<typename Key, class Comparator>
inline const Key& SkipList<Key, Comparator>::Iterator::key() const
{
    assert(valid());
    return node_->key_;
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::next()
{
    assert(valid());
    node_ = node_->next(0);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::prev()
{
    // Instead of using explicit "prev" links, we just search for the
    // last node that falls before key_.
    assert(valid());
    node_ = list_->findLessThan(node_->key_);
    if (node_ == list_->head_)
    {
        node_ = NULL;
    }
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Seek(const Key& target)
{
    node_ = list_->findGreaterOrEqual(target, NULL);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::seekToFirst()
{
    node_ = list_->head_->next(0);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::seekToLast()
{
    node_ = list_->findLast();
    if (node_ == list_->head_) {
        node_ = NULL;
    }
}

template<typename Key, class Comparator>
int SkipList<Key, Comparator>::randomHeight()
{
    // Increase height with probability 1 in kBranching
    static const unsigned int kBranching = 4;
    int height = 1;
    while (height < kMaxHeight && ((rand() % kBranching) == 0))
    {
        height++;
    }
    assert(height > 0);
    assert(height <= kMaxHeight);
    return height;
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::keyIsAfterNode(const Key& key, Node* n) const
{
    // NULL n is considered infinite
    return (n != NULL) && (compare_(n->key_, key) < 0);
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::findGreaterOrEqual(const Key& key, Node** prev) const
{
    Node* x = head_;
    int level = getMaxHeight() - 1;
    while (true)
    {
        Node* next = x->next(level);
        if (keyIsAfterNode(key, next))
        {
            // Keep searching in this list
            x = next;
        }
        else
        {
            if (prev != NULL)
                prev[level] = x;
            if (level == 0)
            {
                return next;
            }
            else
            {
                // Switch to next list
                level--;
            }
        }
    }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node*
SkipList<Key, Comparator>::findLessThan(const Key& key) const
{
    Node* x = head_;
    int level = getMaxHeight() - 1;
    while (true)
    {
        assert(x == head_ || compare_(x->key_, key) < 0);
        Node* next = x->next(level);
        if (next == NULL || compare_(next->key_, key) >= 0)
        {
            if (level == 0)
            {
                return x;
            }
            else
            {
                // Switch to next list
                level--;
            }
        }
        else
        {
            x = next;
        }
    }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::findLast() const
{
    Node* x = head_;
    int level = getMaxHeight() - 1;
    while (true)
    {
        Node* next = x->next(level);
        if (next == NULL)
        {
            if (level == 0)
            {
                return x;
            }
            else
            {
                // Switch to next list
                level--;
            }
        }
        else
        {
            x = next;
        }
    }
}

template<typename Key, class Comparator>
SkipList<Key, Comparator>::SkipList(Comparator cmp)
    : compare_(cmp)
    , head_(newNode(0 /* any key_ will do */, kMaxHeight))
    , max_height_(1)
{
    for (int i = 0; i < kMaxHeight; i++)
    {
        head_->setNext(i, NULL);
    }
}

template<typename Key, class Comparator>
void SkipList<Key, Comparator>::insert(const Key& key)
{
    // TODO(opt): We can use a barrier-free variant of findGreaterOrEqual()
    // here since insert() is externally synchronized.
    Node* prev[kMaxHeight];
    Node* x = findGreaterOrEqual(key, prev);

    // Our data structure does not allow duplicate insertion
    assert(x == NULL || !equal(key, x->key_));

    int height = randomHeight();
    if (height > getMaxHeight())
    {
        for (int i = getMaxHeight(); i < height; i++)
        {
            prev[i] = head_;
        }
        //fprintf(stderr, "Change height from %d to %d\n", max_height_, height);

        max_height_ = height;
    }

    x = newNode(key, height);
    for (int i = 0; i < height; i++)
    {
        x->setNext(i, prev[i]->next(i));
        prev[i]->setNext(i, x);
    }
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::contains(const Key& key) const
{
    Node* x = findGreaterOrEqual(key, NULL);
    if (x != NULL && equal(key, x->key_))
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<typename Key, class Comparator>
void SkipList<Key, Comparator>::remove(const Key& key)
{
    Node* prev[kMaxHeight];
    Node* x = findGreaterOrEqual(key, prev);
    if (x == NULL || !equal(key, x->key_)) // not found
        return;

    deleteNode(x, prev);
}

template<typename Key, class Comparator>
void SkipList<Key, Comparator>::deleteNode(Node* x, Node **update)
{
    for (int i = 0; i < getMaxHeight(); i++)
    {
        if (update[i]->next_[i] == x)
        {
            update[i]->next_[i] = x->next_[i];
        }
    }

    while (max_height_ > 1 && head_->next_[max_height_ - 1] == NULL)
        max_height_--;

    x->~Node();
    free(x);
}

template<typename Key, class Comparator>
std::string SkipList<Key, Comparator>::dump() const
{
    std::ostringstream oss;

    Node* node;
    for (int i = 0; i < kMaxHeight; i++)
    {
        oss << "Height[" << i << "]: ";
        node = head_->next_[i];
        while (node)
        {
            oss << node->key_ << " -> ";
            node = node->next_[i];
        }
        oss << "NULL\n";

    }
    return oss.str();
}

} }  // namespace zl { namespace stl {
#endif  /* ZL_SKIPLIST_H */
