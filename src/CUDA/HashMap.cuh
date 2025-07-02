#include "CUDACommon.cuh"

#define EMPTY_KEY -1
#define VALID_KEY(k) ((k) != EMPTY_KEY)

template<typename Key, typename Value>
struct HashEntry
{
    Key key;
    Value value;
};

template<typename Key, typename Value>
struct HashMap
{
    HashEntry<Key, Value>* entries;
    size_t capacity = 1024 * 1024 * 100;
    uint8_t maxProbe = 64;

    __device__ __host__
        inline size_t hash(Key key) const
    {
        return static_cast<size_t>(key) % capacity;
    }

    __device__
        bool insert(Key key, Value value)
    {
        size_t idx = hash(key);

        for (int i = 0; i < maxProbe; ++i)
        {
            size_t slot = (idx + i) % capacity;
            Key* slot_key = &entries[slot].key;
            Key prev_key = atomicCAS(slot_key, EMPTY_KEY, key);

            if (prev_key == EMPTY_KEY || prev_key == key)
            {
                entries[slot].value = value;
                return true;
            }
        }
        return false;
    }

    __device__
        bool find(Key key, Value* outValue)
    {
        size_t idx = hash(key);

        for (int i = 0; i < maxProbe; ++i)
        {
            size_t slot = (idx + i) % capacity;
            Key k = entries[slot].key;

            if (k == key)
            {
                *outValue = entries[slot].value;
                return true;
            }
            if (k == EMPTY_KEY)
            {
                return false;
            }
        }
        return false;
    }
};
