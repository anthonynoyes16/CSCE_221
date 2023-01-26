#include <cstddef>    // size_t
#include <functional> // std::hash
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"

template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    HashNode **_buckets;
    size_type _size;
    size_type _bucket_count;

    HashNode _head;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const key_type, mapped_type>;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        HashNode * _node;

        explicit iterator(HashNode *ptr) noexcept { _node = ptr; }

    public:
        iterator() { _node = nullptr; };
        iterator(const iterator &) = default;
        iterator(iterator &&) = default;
        ~iterator() = default;
        iterator &operator=(const iterator &) = default;
        iterator &operator=(iterator &&) = default;
        reference operator*() const { return _node->val; }
        pointer operator->() const { return &(_node->val); }
        iterator &operator++() {
            _node = _node->next;
            return *this;
        }
        iterator operator++(int) {
            iterator hold = iterator(_node);
            _node = _node->next;
            return hold;
        }
        bool operator==(const iterator &other) const noexcept {
            if (_node == other._node) {
                return true;
            }
            return false;
        }
        bool operator!=(const iterator &other) const noexcept {
            if (_node == other._node) {
                return false;
            }
            return true;
        }
    };

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            UnorderedMap * _map;
            HashNode * _node;
            size_type _bucket;

            explicit local_iterator(UnorderedMap * map, HashNode *ptr, size_type bucket) noexcept { _map = map, _node = ptr, _bucket = bucket; }

        public:
            local_iterator() { 
                _map = nullptr;
                _node = nullptr;
                _bucket = 0;
            }
            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const { return _node->val; }
            pointer operator->() const { return &(_node->val); }
            local_iterator & operator++() {
                if (_node->next && _bucket == _map->_bucket(_node->next->val.first)) {
                    _node = _node->next;
                    return *this;
                }
                _node = nullptr;
                return *this;
            }
            local_iterator operator++(int) {
                local_iterator hold = local_iterator(_map, _node, _bucket);
                if (_node->next && _bucket == _map->_bucket(_node->next->val.first)) {
                    _node = _node->next;
                    return hold;
                }
                _node = nullptr;
                return hold;
            }
            bool operator==(const local_iterator &other) const noexcept {
                if (_node == other._node) {
                    return true;
                }
                return false;
            }
            bool operator!=(const local_iterator &other) const noexcept {
                if (_node == other._node) {
                    return false;
                }
                return true;
            }
    };

private:

    size_type _bucket(size_t code) const { return _range_hash(code, _bucket_count); }
    size_type _bucket(const Key &key) const { return _bucket(_hash(key)); }

    void _insert_before(size_type bucket, HashNode *node) {
        HashNode*& hold = _buckets[bucket];
        if (hold == nullptr) {
            node->next = _head.next;
            if (_head.next != nullptr) {
                _buckets[_bucket(_head.next->val.first)] = node;
            }
            _head.next = node;
            hold = &_head;
        }
        else {
            node->next = hold->next;
            hold->next = node;
        }
        _size++;
    }

    HashNode*& _bucket_begin(size_type bucket) {
        if (_buckets[bucket] == nullptr) {
            return _buckets[bucket];
        }
        return _buckets[bucket]->next;
    }

    HashNode* _find_prev(size_type code, size_type bucket, const Key & key) {
        if (_buckets[bucket] == nullptr || _buckets[bucket]->next == nullptr) {
            return nullptr;
        }
        HashNode* hold = _buckets[bucket];
        while (hold && (hold->next != nullptr)) {
            if (code != _hash(hold->next->val.first)) {
                hold = hold->next;
                continue;
            }
            if (_equal(hold->next->val.first, key)) {
                return hold;
            }
            hold = hold->next;
        }
        return nullptr;
    }

    HashNode* _find_prev(const Key & key) {
        return _find_prev(_hash(key), _bucket(_hash(key)), key);
    }

    void _erase_after(HashNode * prev) {
        if (prev == nullptr) {
            return;
        }
        HashNode* hold = prev->next;
        if (hold == nullptr) {
            return;
        }
        HashNode* _next = hold->next;
        size_type bucket_hold = _bucket(hold->val.first);
        size_type bucket_next;
        prev->next = _next;
        _size--;
        delete hold;
        if (_next) {
            bucket_next = _bucket(_next->val.first);
        }
        else {
            bucket_next = -1;
        }
        if (prev->next && _buckets[bucket_next] == hold) {
            _buckets[bucket_next] = prev;
        }
        if (_buckets[bucket_hold] == prev && bucket_hold != bucket_next) {
            _buckets[bucket_hold] = nullptr;
        }
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
                const key_equal & equal = key_equal { }): _head() {
                    _hash = hash;
                    _equal = equal;
                    _bucket_count = next_greater_prime(bucket_count);
                    _buckets = new HashNode *[_bucket_count]();
                    _size = 0;
    }

    ~UnorderedMap() {
        clear();
    }

    UnorderedMap(const UnorderedMap & other) {
        _buckets = new HashNode*[other._bucket_count]{};
        _hash = other._hash;
        _size = 0;
        _bucket_count = other._bucket_count;
        _equal = other._equal;
        HashNode* hold = other._head.next;
        while (hold != nullptr) {
            insert(hold->val);
            hold = hold->next;
        }
    }

    UnorderedMap(UnorderedMap && other) {
        _buckets = other._buckets;
        _size = other._size;
        _bucket_count = other._bucket_count;
        _hash = other._hash;
        _equal = other._equal;
        _head.next = other._head.next;
        other._size = 0;
        other._hash = Hash{};
        other._equal = key_equal{};
        other._buckets = new HashNode* [other._bucket_count]{};
        other._head.next = nullptr;
        if (_head.next != nullptr) {
            _buckets[_bucket(_head.next->val.first)] = &_head;
        }
    }

    UnorderedMap & operator=(const UnorderedMap & other) {
        if (_equal(_head.next->val.first, other._head.next->val.first)) {
            return *this;
        }
        this->clear();
        delete[] _buckets;
        _buckets = new HashNode*[other._bucket_count]{};
        _hash = other._hash;
        _size = 0;
        _bucket_count = other._bucket_count;
        _equal = other._equal;
        HashNode* hold = other._head.next;
        while (hold != nullptr) {
            insert(hold->val);
            hold = hold->next;
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) {
        if (_equal(_head.next->val.first, other._head.next->val.first)) {
            return *this;
        }
        
        this->clear();
        delete[] _buckets;
        _buckets = other._buckets;
        _size = other._size;
        _bucket_count = other._bucket_count;
        _hash = other._hash;
        _equal = other._equal;
        _head.next = other._head.next;
        other._size = 0;
        other._hash = Hash{};
        other._equal = key_equal{};
        other._buckets = new HashNode* [other._bucket_count]{};
        other._head.next = nullptr;
        if (_head.next != nullptr) {
            _buckets[_bucket(_head.next->val.first)] = &_head;
        }
        return *this;
    }

    void clear() noexcept {
        iterator start = begin();
        while (start != end()) {
            start = erase(start);
        }
    }

    size_type size() const noexcept { return _size; }

    bool empty() const noexcept { return _size == 0; }

    size_type bucket_count() const noexcept { return _bucket_count; }

    iterator begin() { return iterator(_head.next); }

    iterator end() { return iterator(nullptr); }

    local_iterator begin(size_type n) { return local_iterator(this, _bucket_begin(n), n); }

    local_iterator end(size_type n) { return local_iterator(this, nullptr, n);}

    size_type bucket_size(size_type n) {
        size_type count = 0;
        HashNode* hold = _bucket_begin(n);
        if (hold == nullptr) {
            return 0;
        }
        while (hold->next && _bucket(hold->val.first) == _bucket(hold->next->val.first)) {
            hold = hold->next;
            count++;
        }
        return count+1;
    }

    float load_factor() const { return float(_size)/float(_bucket_count); }

    size_type bucket(const Key & key) const { return _bucket(key); }

    std::pair<iterator, bool> insert(value_type && value) {
        size_t _code = _hash((value.first));
        size_t __bucket = _bucket(_code);
        HashNode* _temp = _find_prev(_code, __bucket, value.first);
        if (_temp) {
            return std::make_pair(iterator(_temp->next), false);
        }
        HashNode* temp = new HashNode(std::move(value));
        _insert_before(__bucket, temp);
        iterator hold = iterator(temp);
        return std::make_pair(hold, true);
    }

    std::pair<iterator, bool> insert(const value_type & value) {
        size_t _code = _hash(value.first);
        size_t __bucket = _bucket(_code);
        HashNode* _temp = _find_prev(_code, __bucket, value.first);
        if (_temp) {
            return std::make_pair(iterator(_temp->next), false);
        }
        HashNode* temp = new HashNode(value);
        _insert_before(__bucket, temp);
        iterator hold = iterator(temp);
        return std::make_pair(hold, true);
    }

    iterator find(const Key & key) {
        if (_find_prev(key)) {
            return iterator(_find_prev(key)->next);
        }
        return end();
    }

    T& operator[](const Key & key) {
        if (find(key) == end()) {
            HashNode* temp = new HashNode(std::make_pair(key, mapped_type()));
            _insert_before(_bucket(key), temp);
        }
        return find(key)._node->val.second;
    }

    iterator erase(iterator pos) {
        HashNode* prev = _find_prev(pos._node->val.first);
        if (prev == nullptr) {
            return iterator(nullptr);
        }
        _erase_after(prev);
        return iterator(prev->next);
    }

    size_type erase(const Key & key) {
        iterator hold = find(key);
        if (hold == iterator(nullptr)) {
            return 0;
        }
        else {
            erase(hold);
            return 1;
        }
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    HashNode const * node = map._head.next;
    os << "List: ";
    do {
        if(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
        } else {
            os << "(nullptr)";
            break;
        }

        node = node->next;
    } while(true);
    os << std::endl;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        if(!node) {
            os << "(nullptr)";
        } else {
            while((node = node->next) && map.bucket(node->val.first) == bucket) {
                os << "(" << node->val.first << ", " << node->val.second << ") ";
            }
        }
        os << std::endl;
    }
}