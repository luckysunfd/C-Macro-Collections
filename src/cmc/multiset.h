/**
 * multiset.h
 *
 * Creation Date: 10/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * MultiSet
 *
 * In mathematics, a multiset is a modification of the concept of a set that,
 * unlike a set, allows for multiple instances for each of its elements. The
 * positive integer number of instances, given for each element is called the
 * multiplicity of this element in the multiset. A MultiSet also has a
 * cardinality which equals the sum of the multiplicities of its elements.
 */

#ifndef CMC_MULTISET_H
#define CMC_MULTISET_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utl/cmc_string.h"

/* to_string format */
static const char *cmc_string_fmt_multiset = "%s at %p { buffer:%p, capacity:%" PRIuMAX ", count:%" PRIuMAX ", cardinality:%" PRIuMAX ", load:%lf, cmp:%p, hash:%p }";

#ifndef CMC_IMPL_HASHTABLE_STATE
#define CMC_IMPL_HASHTABLE_STATE

enum cmc_entry_state_e
{
    CMC_ES_DELETED = -1,
    CMC_ES_EMPTY = 0,
    CMC_ES_FILLED = 1
};

#endif /* CMC_IMPL_HASHTABLE_STATE */

#ifndef CMC_IMPL_HASHTABLE_SETUP
#define CMC_IMPL_HASHTABLE_SETUP

static const size_t cmc_hashtable_primes[] = {53, 97, 191, 383, 769, 1531,
                                              3067, 6143, 12289, 24571, 49157,
                                              98299, 196613, 393209, 786431,
                                              1572869, 3145721, 6291449,
                                              12582917, 25165813, 50331653,
                                              100663291, 201326611, 402653189,
                                              805306357, 1610612741,
                                              3221225473, 6442450939,
                                              12884901893, 25769803799,
                                              51539607551, 103079215111,
                                              206158430209, 412316860441,
                                              824633720831, 1649267441651,
                                              3298534883309, 6597069766657,
                                              13194139533299, 26388279066623,
                                              52776558133303, 105553116266489,
                                              211106232532969, 422212465066001,
                                              844424930131963,
                                              1688849860263953,
                                              3377699720527861,
                                              6755399441055731,
                                              13510798882111483,
                                              27021597764222939,
                                              54043195528445957,
                                              108086391056891903,
                                              216172782113783773,
                                              432345564227567621,
                                              864691128455135207,
                                              1729382256910270481,
                                              3458764513820540933,
                                              6917529027641081903,
                                              13835058055282163729llu};

#endif /* CMC_IMPL_HASHTABLE_SETUP */

#define CMC_GENERATE_MULTISET(PFX, SNAME, V)    \
    CMC_GENERATE_MULTISET_HEADER(PFX, SNAME, V) \
    CMC_GENERATE_MULTISET_SOURCE(PFX, SNAME, V)

#define CMC_WRAPGEN_MULTISET_HEADER(PFX, SNAME, K, V) \
    CMC_GENERATE_MULTISET_HEADER(PFX, SNAME, V)

#define CMC_WRAPGEN_MULTISET_SOURCE(PFX, SNAME, K, V) \
    CMC_GENERATE_MULTISET_SOURCE(PFX, SNAME, V)

/* HEADER ********************************************************************/
#define CMC_GENERATE_MULTISET_HEADER(PFX, SNAME, V)                                          \
                                                                                             \
    /* Hashset Structure */                                                                  \
    typedef struct SNAME##_s                                                                 \
    {                                                                                        \
        /* Array of Entries */                                                               \
        struct SNAME##_entry_s *buffer;                                                      \
                                                                                             \
        /* Current Array Capcity */                                                          \
        size_t capacity;                                                                     \
                                                                                             \
        /* Current amount of unique elements */                                              \
        size_t count;                                                                        \
                                                                                             \
        /* Total amount of elements taking into account their multiplicity */                \
        size_t cardinality;                                                                  \
                                                                                             \
        /* Load factor in range (0.0, 1.0) */                                                \
        double load;                                                                         \
                                                                                             \
        /* Element comparison function */                                                    \
        int (*cmp)(V, V);                                                                    \
                                                                                             \
        /* Element hash function */                                                          \
        size_t (*hash)(V);                                                                   \
                                                                                             \
        /* Function that returns an iterator to the start of the hashset */                  \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                               \
                                                                                             \
        /* Function that returns an iterator to the end of the hashset */                    \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                                 \
                                                                                             \
    } SNAME, *SNAME##_ptr;                                                                   \
                                                                                             \
    typedef struct SNAME##_entry_s                                                           \
    {                                                                                        \
        /* Entry element */                                                                  \
        V value;                                                                             \
                                                                                             \
        /* The element's multiplicity */                                                     \
        size_t multiplicity;                                                                 \
                                                                                             \
        /* The distance of this node to its original position, used by robin-hood hashing */ \
        size_t dist;                                                                         \
                                                                                             \
        /* The sate of this node (DELETED, EMPTY, FILLED) */                                 \
        enum cmc_entry_state_e state;                                                        \
                                                                                             \
    } SNAME##_entry, *SNAME##_entry_ptr;                                                     \
                                                                                             \
    /* Hashset Iterator */                                                                   \
    typedef struct SNAME##_iter_s                                                            \
    {                                                                                        \
        /* Target Hashset */                                                                 \
        struct SNAME##_s *target;                                                            \
                                                                                             \
        /* Cursor's position (index) */                                                      \
        size_t cursor;                                                                       \
                                                                                             \
        /* Keeps track of relative index to the iteration of elements */                     \
        size_t index;                                                                        \
                                                                                             \
        /* The index of the first element */                                                 \
        size_t first;                                                                        \
                                                                                             \
        /* The index of the last element */                                                  \
        size_t last;                                                                         \
                                                                                             \
        /* If the iterator has reached the start of the iteration */                         \
        bool start;                                                                          \
                                                                                             \
        /* If the iterator has reached the end of the iteration */                           \
        bool end;                                                                            \
                                                                                             \
    } SNAME##_iter, *SNAME##_iter_ptr;                                                       \
                                                                                             \
    /* Collection Functions */                                                               \
    /* Collection Allocation and Deallocation */                                             \
    SNAME *PFX##_new(size_t capacity, double load, int (*compare)(V, V), size_t (*hash)(V)); \
    void PFX##_clear(SNAME *_set_, void (*deallocator)(V));                                  \
    void PFX##_free(SNAME *_set_, void (*deallocator)(V));                                   \
    /* Collection Input and Output */                                                        \
    bool PFX##_insert(SNAME *_set_, V element);                                              \
    bool PFX##_insert_many(SNAME *_set_, V element, size_t count);                           \
    bool PFX##_update(SNAME *_set_, V element, size_t multiplicity);                         \
    bool PFX##_remove(SNAME *_set_, V element);                                              \
    size_t PFX##_remove_all(SNAME *_set_, V element);                                        \
    /* Element Access */                                                                     \
    bool PFX##_max(SNAME *_set_, V *value);                                                  \
    bool PFX##_min(SNAME *_set_, V *value);                                                  \
    size_t PFX##_multiplicity_of(SNAME *_set_, V element);                                   \
    /* Collection State */                                                                   \
    bool PFX##_contains(SNAME *_set_, V element);                                            \
    bool PFX##_empty(SNAME *_set_);                                                          \
    bool PFX##_full(SNAME *_set_);                                                           \
    size_t PFX##_count(SNAME *_set_);                                                        \
    size_t PFX##_cardinality(SNAME *_set_);                                                  \
    size_t PFX##_capacity(SNAME *_set_);                                                     \
    double PFX##_load(SNAME *_set_);                                                         \
    /* Collection Utility */                                                                 \
    bool PFX##_resize(SNAME *_set_, size_t capacity);                                        \
    SNAME *PFX##_copy_of(SNAME *_set_, V (*copy_func)(V));                                   \
    bool PFX##_equals(SNAME *_set1_, SNAME *_set2_, bool ignore_multiplicity);               \
    cmc_string PFX##_to_string(SNAME *_set_);                                                \
                                                                                             \
    /* Set Operations */                                                                     \
    SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_);                                        \
    SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_);                                 \
    SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_);                                   \
    SNAME *PFX##_summation(SNAME *_set1_, SNAME *_set2_);                                    \
    SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_);                         \
    bool PFX##_is_subset(SNAME *_set1_, SNAME *_set2_);                                      \
    bool PFX##_is_superset(SNAME *_set1_, SNAME *_set2_);                                    \
    bool PFX##_is_proper_subset(SNAME *_set1_, SNAME *_set2_);                               \
    bool PFX##_is_proper_superset(SNAME *_set1_, SNAME *_set2_);                             \
    bool PFX##_is_disjointset(SNAME *_set1_, SNAME *_set2_);                                 \
                                                                                             \
    /* Iterator Functions */                                                                 \
    /* Iterator Allocation and Deallocation */                                               \
    SNAME##_iter *PFX##_iter_new(SNAME *target);                                             \
    void PFX##_iter_free(SNAME##_iter *iter);                                                \
    /* Iterator Initialization */                                                            \
    void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                                 \
    /* Iterator State */                                                                     \
    bool PFX##_iter_start(SNAME##_iter *iter);                                               \
    bool PFX##_iter_end(SNAME##_iter *iter);                                                 \
    /* Iterator Movement */                                                                  \
    void PFX##_iter_to_start(SNAME##_iter *iter);                                            \
    void PFX##_iter_to_end(SNAME##_iter *iter);                                              \
    bool PFX##_iter_next(SNAME##_iter *iter);                                                \
    bool PFX##_iter_prev(SNAME##_iter *iter);                                                \
    bool PFX##_iter_advance(SNAME##_iter *iter, size_t steps);                               \
    bool PFX##_iter_rewind(SNAME##_iter *iter, size_t steps);                                \
    bool PFX##_iter_go_to(SNAME##_iter *iter, size_t index);                                 \
    /* Iterator Access */                                                                    \
    V PFX##_iter_value(SNAME##_iter *iter);                                                  \
    size_t PFX##_iter_multiplicity(SNAME##_iter *iter);                                      \
    size_t PFX##_iter_index(SNAME##_iter *iter);                                             \
                                                                                             \
/* SOURCE ********************************************************************/
#define CMC_GENERATE_MULTISET_SOURCE(PFX, SNAME, V)                                                \
                                                                                                   \
    /* Implementation Detail Functions */                                                          \
    static SNAME##_entry *PFX##_impl_insert_and_return(SNAME *_set_, V element, bool *new_node);   \
    static SNAME##_entry *PFX##_impl_get_entry(SNAME *_set_, V element);                           \
    static size_t PFX##_impl_calculate_size(size_t required);                                      \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_set_);                                         \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_set_);                                           \
                                                                                                   \
    SNAME *PFX##_new(size_t capacity, double load, int (*compare)(V, V), size_t (*hash)(V))        \
    {                                                                                              \
        if (capacity == 0 || load <= 0 || load >= 1)                                               \
            return NULL;                                                                           \
                                                                                                   \
        /* Prevent integer overflow */                                                             \
        if (capacity >= UINTMAX_MAX * load)                                                        \
            return NULL;                                                                           \
                                                                                                   \
        size_t real_capacity = PFX##_impl_calculate_size(capacity / load);                         \
                                                                                                   \
        SNAME *_set_ = malloc(sizeof(SNAME));                                                      \
                                                                                                   \
        if (!_set_)                                                                                \
            return NULL;                                                                           \
                                                                                                   \
        _set_->buffer = calloc(real_capacity, sizeof(SNAME##_entry));                              \
                                                                                                   \
        if (!_set_->buffer)                                                                        \
        {                                                                                          \
            free(_set_);                                                                           \
            return NULL;                                                                           \
        }                                                                                          \
                                                                                                   \
        _set_->count = 0;                                                                          \
        _set_->cardinality = 0;                                                                    \
        _set_->capacity = real_capacity;                                                           \
        _set_->load = load;                                                                        \
        _set_->cmp = compare;                                                                      \
        _set_->hash = hash;                                                                        \
                                                                                                   \
        _set_->it_start = PFX##_impl_it_start;                                                     \
        _set_->it_end = PFX##_impl_it_end;                                                         \
                                                                                                   \
        return _set_;                                                                              \
    }                                                                                              \
                                                                                                   \
    void PFX##_clear(SNAME *_set_, void (*deallocator)(V))                                         \
    {                                                                                              \
        if (deallocator)                                                                           \
        {                                                                                          \
            for (size_t i = 0; i < _set_->capacity; i++)                                           \
            {                                                                                      \
                SNAME##_entry *entry = &(_set_->buffer[i]);                                        \
                                                                                                   \
                if (entry->state == CMC_ES_FILLED)                                                 \
                {                                                                                  \
                    deallocator(entry->value);                                                     \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
                                                                                                   \
        memset(_set_->buffer, 0, sizeof(SNAME##_entry) * _set_->capacity);                         \
                                                                                                   \
        _set_->count = 0;                                                                          \
    }                                                                                              \
                                                                                                   \
    void PFX##_free(SNAME *_set_, void (*deallocator)(V))                                          \
    {                                                                                              \
        if (deallocator)                                                                           \
        {                                                                                          \
            for (size_t i = 0; i < _set_->capacity; i++)                                           \
            {                                                                                      \
                SNAME##_entry *entry = &(_set_->buffer[i]);                                        \
                                                                                                   \
                if (entry->state == CMC_ES_FILLED)                                                 \
                {                                                                                  \
                    deallocator(entry->value);                                                     \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
                                                                                                   \
        free(_set_->buffer);                                                                       \
        free(_set_);                                                                               \
    }                                                                                              \
                                                                                                   \
    bool PFX##_insert(SNAME *_set_, V element)                                                     \
    {                                                                                              \
        bool new_node;                                                                             \
                                                                                                   \
        SNAME##_entry *entry = PFX##_impl_insert_and_return(_set_, element, &new_node);            \
                                                                                                   \
        if (!entry)                                                                                \
            return false;                                                                          \
                                                                                                   \
        if (!new_node)                                                                             \
            entry->multiplicity++;                                                                 \
                                                                                                   \
        _set_->cardinality++;                                                                      \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    bool PFX##_insert_many(SNAME *_set_, V element, size_t count)                                  \
    {                                                                                              \
        if (count == 0)                                                                            \
            return true;                                                                           \
                                                                                                   \
        bool new_node;                                                                             \
                                                                                                   \
        SNAME##_entry *entry = PFX##_impl_insert_and_return(_set_, element, &new_node);            \
                                                                                                   \
        if (!entry)                                                                                \
            return false;                                                                          \
                                                                                                   \
        if (new_node)                                                                              \
            entry->multiplicity = count;                                                           \
        else                                                                                       \
            entry->multiplicity += count;                                                          \
                                                                                                   \
        _set_->cardinality += count;                                                               \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    bool PFX##_update(SNAME *_set_, V element, size_t multiplicity)                                \
    {                                                                                              \
        if (multiplicity == 0)                                                                     \
            return PFX##_remove_all(_set_, element);                                               \
                                                                                                   \
        bool new_node;                                                                             \
                                                                                                   \
        SNAME##_entry *entry = PFX##_impl_insert_and_return(_set_, element, &new_node);            \
                                                                                                   \
        if (!entry)                                                                                \
            return false;                                                                          \
                                                                                                   \
        if (new_node)                                                                              \
            _set_->cardinality++;                                                                  \
                                                                                                   \
        _set_->cardinality = (_set_->cardinality - entry->multiplicity) + multiplicity;            \
                                                                                                   \
        entry->multiplicity = multiplicity;                                                        \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    bool PFX##_remove(SNAME *_set_, V element)                                                     \
    {                                                                                              \
        SNAME##_entry *result = PFX##_impl_get_entry(_set_, element);                              \
                                                                                                   \
        if (result == NULL)                                                                        \
            return false;                                                                          \
                                                                                                   \
        if (result->multiplicity > 1)                                                              \
            result->multiplicity--;                                                                \
        else                                                                                       \
        {                                                                                          \
            result->value = (V){0};                                                                \
            result->multiplicity = 0;                                                              \
            result->dist = 0;                                                                      \
            result->state = CMC_ES_DELETED;                                                        \
                                                                                                   \
            _set_->count--;                                                                        \
        }                                                                                          \
                                                                                                   \
        _set_->cardinality--;                                                                      \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    size_t PFX##_remove_all(SNAME *_set_, V element)                                               \
    {                                                                                              \
        SNAME##_entry *result = PFX##_impl_get_entry(_set_, element);                              \
                                                                                                   \
        if (result == NULL)                                                                        \
            return 0;                                                                              \
                                                                                                   \
        size_t removed = result->multiplicity;                                                     \
                                                                                                   \
        result->value = (V){0};                                                                    \
        result->multiplicity = 0;                                                                  \
        result->dist = 0;                                                                          \
        result->state = CMC_ES_DELETED;                                                            \
                                                                                                   \
        _set_->count--;                                                                            \
        _set_->cardinality -= removed;                                                             \
                                                                                                   \
        return removed;                                                                            \
    }                                                                                              \
                                                                                                   \
    bool PFX##_max(SNAME *_set_, V *value)                                                         \
    {                                                                                              \
        if (PFX##_empty(_set_))                                                                    \
            return false;                                                                          \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
                                                                                                   \
        for (PFX##_iter_init(&iter, _set_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))        \
        {                                                                                          \
            V result = PFX##_iter_value(&iter);                                                    \
            size_t index = PFX##_iter_index(&iter);                                                \
                                                                                                   \
            if (index == 0)                                                                        \
                *value = result;                                                                   \
            else if (_set_->cmp(result, *value) > 0)                                               \
                *value = result;                                                                   \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    bool PFX##_min(SNAME *_set_, V *value)                                                         \
    {                                                                                              \
        if (PFX##_empty(_set_))                                                                    \
            return false;                                                                          \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
                                                                                                   \
        for (PFX##_iter_init(&iter, _set_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))        \
        {                                                                                          \
            V result = PFX##_iter_value(&iter);                                                    \
            size_t index = PFX##_iter_index(&iter);                                                \
                                                                                                   \
            if (index == 0)                                                                        \
                *value = result;                                                                   \
            else if (_set_->cmp(result, *value) < 0)                                               \
                *value = result;                                                                   \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    size_t PFX##_multiplicity_of(SNAME *_set_, V element)                                          \
    {                                                                                              \
        SNAME##_entry *entry = PFX##_impl_get_entry(_set_, element);                               \
                                                                                                   \
        if (!entry)                                                                                \
            return 0;                                                                              \
                                                                                                   \
        return entry->multiplicity;                                                                \
    }                                                                                              \
                                                                                                   \
    bool PFX##_contains(SNAME *_set_, V element)                                                   \
    {                                                                                              \
        return PFX##_impl_get_entry(_set_, element) != NULL;                                       \
    }                                                                                              \
                                                                                                   \
    bool PFX##_empty(SNAME *_set_)                                                                 \
    {                                                                                              \
        return _set_->count == 0;                                                                  \
    }                                                                                              \
                                                                                                   \
    bool PFX##_full(SNAME *_set_)                                                                  \
    {                                                                                              \
        return (double)PFX##_capacity(_set_) * PFX##_load(_set_) <= (double)PFX##_count(_set_);    \
    }                                                                                              \
                                                                                                   \
    size_t PFX##_count(SNAME *_set_)                                                               \
    {                                                                                              \
        return _set_->count;                                                                       \
    }                                                                                              \
                                                                                                   \
    size_t PFX##_cardinality(SNAME *_set_)                                                         \
    {                                                                                              \
        return _set_->cardinality;                                                                 \
    }                                                                                              \
                                                                                                   \
    size_t PFX##_capacity(SNAME *_set_)                                                            \
    {                                                                                              \
        return _set_->capacity;                                                                    \
    }                                                                                              \
                                                                                                   \
    double PFX##_load(SNAME *_set_)                                                                \
    {                                                                                              \
        return _set_->load;                                                                        \
    }                                                                                              \
                                                                                                   \
    bool PFX##_resize(SNAME *_set_, size_t capacity)                                               \
    {                                                                                              \
        if (PFX##_capacity(_set_) == capacity)                                                     \
            return true;                                                                           \
                                                                                                   \
        if (PFX##_capacity(_set_) > capacity / PFX##_load(_set_))                                  \
            return true;                                                                           \
                                                                                                   \
        /* Prevent integer overflow */                                                             \
        if (capacity >= UINTMAX_MAX * PFX##_load(_set_))                                           \
            return false;                                                                          \
                                                                                                   \
        /* Calculate required capacity based on the prime numbers */                               \
        size_t theoretical_size = PFX##_impl_calculate_size(capacity);                             \
                                                                                                   \
        /* Not possible to shrink with current available prime numbers */                          \
        if (theoretical_size < PFX##_count(_set_) / PFX##_load(_set_))                             \
            return false;                                                                          \
                                                                                                   \
        SNAME *_new_set_ = PFX##_new(capacity, PFX##_load(_set_), _set_->cmp, _set_->hash);        \
                                                                                                   \
        if (!_new_set_)                                                                            \
            return false;                                                                          \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
                                                                                                   \
        for (PFX##_iter_init(&iter, _set_); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))        \
        {                                                                                          \
            PFX##_insert_many(_new_set_, PFX##_iter_value(&iter), PFX##_iter_multiplicity(&iter)); \
        }                                                                                          \
                                                                                                   \
        if (PFX##_count(_set_) != PFX##_count(_new_set_))                                          \
        {                                                                                          \
            PFX##_free(_new_set_, NULL);                                                           \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        SNAME##_entry *tmp_b = _set_->buffer;                                                      \
        _set_->buffer = _new_set_->buffer;                                                         \
        _new_set_->buffer = tmp_b;                                                                 \
                                                                                                   \
        size_t tmp_c = _set_->capacity;                                                            \
        _set_->capacity = _new_set_->capacity;                                                     \
        _new_set_->capacity = tmp_c;                                                               \
                                                                                                   \
        PFX##_free(_new_set_, NULL);                                                               \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    SNAME *PFX##_copy_of(SNAME *_set_, V (*copy_func)(V))                                          \
    {                                                                                              \
        SNAME *result = PFX##_new(_set_->capacity, _set_->load, _set_->cmp, _set_->hash);          \
                                                                                                   \
        if (!result)                                                                               \
            return NULL;                                                                           \
                                                                                                   \
        if (copy_func)                                                                             \
        {                                                                                          \
            for (size_t i = 0; i < _set_->capacity; i++)                                           \
            {                                                                                      \
                SNAME##_entry *scan = &(_set_->buffer[i]);                                         \
                                                                                                   \
                if (scan->state != CMC_ES_EMPTY)                                                   \
                {                                                                                  \
                    SNAME##_entry *target = &(result->buffer[i]);                                  \
                                                                                                   \
                    if (scan->state == CMC_ES_DELETED)                                             \
                        target->state = CMC_ES_DELETED;                                            \
                    else                                                                           \
                    {                                                                              \
                        target->state = scan->state;                                               \
                        target->dist = scan->dist;                                                 \
                        target->multiplicity = scan->multiplicity;                                 \
                                                                                                   \
                        target->value = copy_func(scan->value);                                    \
                    }                                                                              \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
        else                                                                                       \
            memcpy(result->buffer, _set_->buffer, sizeof(SNAME##_entry) * _set_->capacity);        \
                                                                                                   \
        result->count = _set_->count;                                                              \
        result->cardinality = _set_->cardinality;                                                  \
                                                                                                   \
        return result;                                                                             \
    }                                                                                              \
                                                                                                   \
    bool PFX##_equals(SNAME *_set1_, SNAME *_set2_, bool ignore_multiplicity)                      \
    {                                                                                              \
        if (PFX##_count(_set1_) != PFX##_count(_set2_))                                            \
            return false;                                                                          \
                                                                                                   \
        if (!ignore_multiplicity && PFX##_cardinality(_set1_) != PFX##_cardinality(_set2_))        \
            return false;                                                                          \
                                                                                                   \
        if (PFX##_count(_set1_) == 0)                                                              \
            return true;                                                                           \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
        PFX##_iter_init(&iter, _set1_);                                                            \
                                                                                                   \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))           \
        {                                                                                          \
            SNAME##_entry *entry = PFX##_impl_get_entry(_set2_, PFX##_iter_value(&iter));          \
                                                                                                   \
            if (entry == NULL)                                                                     \
                return false;                                                                      \
                                                                                                   \
            if (!ignore_multiplicity && entry->multiplicity != PFX##_iter_multiplicity(&iter))     \
                return false;                                                                      \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    cmc_string PFX##_to_string(SNAME *_set_)                                                       \
    {                                                                                              \
        cmc_string str;                                                                            \
        SNAME *s_ = _set_;                                                                         \
        const char *name = #SNAME;                                                                 \
                                                                                                   \
        snprintf(str.s, cmc_string_len, cmc_string_fmt_multiset,                                   \
                 name, s_, s_->buffer, s_->capacity, s_->count,                                    \
                 s_->cardinality, s_->load, s_->cmp, s_->hash);                                    \
                                                                                                   \
        return str;                                                                                \
    }                                                                                              \
                                                                                                   \
    SNAME *PFX##_union(SNAME *_set1_, SNAME *_set2_)                                               \
    {                                                                                              \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);     \
                                                                                                   \
        if (!_set_r_)                                                                              \
            return NULL;                                                                           \
                                                                                                   \
        SNAME##_iter iter1, iter2;                                                                 \
        PFX##_iter_init(&iter1, _set1_);                                                           \
        PFX##_iter_init(&iter2, _set2_);                                                           \
                                                                                                   \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1); PFX##_iter_next(&iter1))        \
        {                                                                                          \
            V value = PFX##_iter_value(&iter1);                                                    \
                                                                                                   \
            size_t m1 = PFX##_iter_multiplicity(&iter1);                                           \
            size_t m2 = PFX##_multiplicity_of(_set2_, value);                                      \
            size_t max_ = m1 > m2 ? m1 : m2;                                                       \
                                                                                                   \
            PFX##_update(_set_r_, value, max_);                                                    \
        }                                                                                          \
                                                                                                   \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2); PFX##_iter_next(&iter2))        \
        {                                                                                          \
            V value = PFX##_iter_value(&iter2);                                                    \
                                                                                                   \
            size_t m1 = PFX##_multiplicity_of(_set1_, value);                                      \
            size_t m2 = PFX##_iter_multiplicity(&iter2);                                           \
            size_t max_ = m1 > m2 ? m1 : m2;                                                       \
                                                                                                   \
            PFX##_update(_set_r_, value, max_);                                                    \
        }                                                                                          \
                                                                                                   \
        return _set_r_;                                                                            \
    }                                                                                              \
                                                                                                   \
    SNAME *PFX##_intersection(SNAME *_set1_, SNAME *_set2_)                                        \
    {                                                                                              \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);     \
                                                                                                   \
        if (!_set_r_)                                                                              \
            return NULL;                                                                           \
                                                                                                   \
        SNAME *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;                          \
        SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;                                      \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
        PFX##_iter_init(&iter, _set_A_);                                                           \
                                                                                                   \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))           \
        {                                                                                          \
            V value = PFX##_iter_value(&iter);                                                     \
                                                                                                   \
            size_t m1 = PFX##_iter_multiplicity(&iter);                                            \
            size_t m2 = PFX##_multiplicity_of(_set_B_, value);                                     \
            size_t max_ = m1 < m2 ? m1 : m2;                                                       \
                                                                                                   \
            PFX##_update(_set_r_, value, max_);                                                    \
        }                                                                                          \
                                                                                                   \
        return _set_r_;                                                                            \
    }                                                                                              \
                                                                                                   \
    SNAME *PFX##_difference(SNAME *_set1_, SNAME *_set2_)                                          \
    {                                                                                              \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);     \
                                                                                                   \
        if (!_set_r_)                                                                              \
            return NULL;                                                                           \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
        PFX##_iter_init(&iter, _set1_);                                                            \
                                                                                                   \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))           \
        {                                                                                          \
            V value = PFX##_iter_value(&iter);                                                     \
                                                                                                   \
            size_t m1 = PFX##_iter_multiplicity(&iter);                                            \
            size_t m2 = PFX##_multiplicity_of(_set2_, value);                                      \
                                                                                                   \
            if (m1 > m2)                                                                           \
                PFX##_update(_set_r_, value, m1 - m2);                                             \
        }                                                                                          \
                                                                                                   \
        return _set_r_;                                                                            \
    }                                                                                              \
                                                                                                   \
    SNAME *PFX##_summation(SNAME *_set1_, SNAME *_set2_)                                           \
    {                                                                                              \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);     \
                                                                                                   \
        if (!_set_r_)                                                                              \
            return NULL;                                                                           \
                                                                                                   \
        SNAME##_iter iter1, iter2;                                                                 \
        PFX##_iter_init(&iter1, _set1_);                                                           \
        PFX##_iter_init(&iter2, _set2_);                                                           \
                                                                                                   \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1); PFX##_iter_next(&iter1))        \
        {                                                                                          \
            PFX##_insert_many(_set_r_, PFX##_iter_value(&iter1), PFX##_iter_multiplicity(&iter1)); \
        }                                                                                          \
                                                                                                   \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2); PFX##_iter_next(&iter2))        \
        {                                                                                          \
            PFX##_insert_many(_set_r_, PFX##_iter_value(&iter2), PFX##_iter_multiplicity(&iter2)); \
        }                                                                                          \
                                                                                                   \
        return _set_r_;                                                                            \
    }                                                                                              \
                                                                                                   \
    SNAME *PFX##_symmetric_difference(SNAME *_set1_, SNAME *_set2_)                                \
    {                                                                                              \
        SNAME##_iter iter1, iter2;                                                                 \
                                                                                                   \
        SNAME *_set_r_ = PFX##_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);     \
                                                                                                   \
        if (!_set_r_)                                                                              \
            return NULL;                                                                           \
                                                                                                   \
        PFX##_iter_init(&iter1, _set1_);                                                           \
        PFX##_iter_init(&iter2, _set2_);                                                           \
                                                                                                   \
        for (PFX##_iter_to_start(&iter1); !PFX##_iter_end(&iter1); PFX##_iter_next(&iter1))        \
        {                                                                                          \
            V value = PFX##_iter_value(&iter1);                                                    \
                                                                                                   \
            size_t m1 = PFX##_iter_multiplicity(&iter1);                                           \
            size_t m2 = PFX##_multiplicity_of(_set2_, value);                                      \
                                                                                                   \
            if (m1 != m2)                                                                          \
            {                                                                                      \
                if (m1 > m2)                                                                       \
                    PFX##_update(_set_r_, value, m1 - m2);                                         \
                else                                                                               \
                    PFX##_update(_set_r_, value, m2 - m1);                                         \
            }                                                                                      \
        }                                                                                          \
                                                                                                   \
        for (PFX##_iter_to_start(&iter2); !PFX##_iter_end(&iter2); PFX##_iter_next(&iter2))        \
        {                                                                                          \
            V value = PFX##_iter_value(&iter2);                                                    \
                                                                                                   \
            size_t m1 = PFX##_multiplicity_of(_set1_, value);                                      \
            size_t m2 = PFX##_iter_multiplicity(&iter2);                                           \
                                                                                                   \
            if (m1 != m2)                                                                          \
            {                                                                                      \
                if (m1 > m2)                                                                       \
                    PFX##_update(_set_r_, value, m1 - m2);                                         \
                else                                                                               \
                    PFX##_update(_set_r_, value, m2 - m1);                                         \
            }                                                                                      \
        }                                                                                          \
                                                                                                   \
        return _set_r_;                                                                            \
    }                                                                                              \
                                                                                                   \
    bool PFX##_is_subset(SNAME *_set1_, SNAME *_set2_)                                             \
    {                                                                                              \
        if (PFX##_count(_set1_) > PFX##_count(_set2_))                                             \
            return false;                                                                          \
                                                                                                   \
        if (PFX##_empty(_set1_))                                                                   \
            return true;                                                                           \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
                                                                                                   \
        PFX##_iter_init(&iter, _set1_);                                                            \
                                                                                                   \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))           \
        {                                                                                          \
            V value = PFX##_iter_value(&iter);                                                     \
                                                                                                   \
            size_t m1 = PFX##_iter_multiplicity(&iter);                                            \
            size_t m2 = PFX##_multiplicity_of(_set2_, value);                                      \
                                                                                                   \
            if (m1 > m2)                                                                           \
                return false;                                                                      \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    bool PFX##_is_superset(SNAME *_set1_, SNAME *_set2_)                                           \
    {                                                                                              \
        return PFX##_is_subset(_set2_, _set1_);                                                    \
    }                                                                                              \
                                                                                                   \
    bool PFX##_is_proper_subset(SNAME *_set1_, SNAME *_set2_)                                      \
    {                                                                                              \
        if (PFX##_count(_set1_) >= PFX##_count(_set2_))                                            \
            return false;                                                                          \
                                                                                                   \
        if (PFX##_empty(_set1_))                                                                   \
        {                                                                                          \
            if (!PFX##_empty(_set2_))                                                              \
                return true;                                                                       \
            else                                                                                   \
                return false;                                                                      \
        }                                                                                          \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
                                                                                                   \
        PFX##_iter_init(&iter, _set1_);                                                            \
                                                                                                   \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))           \
        {                                                                                          \
            V value = PFX##_iter_value(&iter);                                                     \
                                                                                                   \
            size_t m1 = PFX##_iter_multiplicity(&iter);                                            \
            size_t m2 = PFX##_multiplicity_of(_set2_, value);                                      \
                                                                                                   \
            if (m1 >= m2)                                                                          \
                return false;                                                                      \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    bool PFX##_is_proper_superset(SNAME *_set1_, SNAME *_set2_)                                    \
    {                                                                                              \
        return PFX##_is_proper_subset(_set2_, _set1_);                                             \
    }                                                                                              \
                                                                                                   \
    bool PFX##_is_disjointset(SNAME *_set1_, SNAME *_set2_)                                        \
    {                                                                                              \
        if (PFX##_empty(_set1_))                                                                   \
            return true;                                                                           \
                                                                                                   \
        SNAME##_iter iter;                                                                         \
                                                                                                   \
        PFX##_iter_init(&iter, _set1_);                                                            \
                                                                                                   \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))           \
        {                                                                                          \
            V value = PFX##_iter_value(&iter);                                                     \
                                                                                                   \
            if (PFX##_contains(_set2_, value))                                                     \
                return false;                                                                      \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    SNAME##_iter *PFX##_iter_new(SNAME *target)                                                    \
    {                                                                                              \
        SNAME##_iter *iter = malloc(sizeof(SNAME##_iter));                                         \
                                                                                                   \
        if (!iter)                                                                                 \
            return NULL;                                                                           \
                                                                                                   \
        PFX##_iter_init(iter, target);                                                             \
                                                                                                   \
        return iter;                                                                               \
    }                                                                                              \
                                                                                                   \
    void PFX##_iter_free(SNAME##_iter *iter)                                                       \
    {                                                                                              \
        free(iter);                                                                                \
    }                                                                                              \
                                                                                                   \
    void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                                        \
    {                                                                                              \
        memset(iter, 0, sizeof(SNAME##_iter));                                                     \
                                                                                                   \
        iter->target = target;                                                                     \
        iter->start = true;                                                                        \
        iter->end = PFX##_empty(target);                                                           \
                                                                                                   \
        if (!PFX##_empty(target))                                                                  \
        {                                                                                          \
            for (size_t i = 0; i < target->capacity; i++)                                          \
            {                                                                                      \
                if (target->buffer[i].state == CMC_ES_FILLED)                                      \
                {                                                                                  \
                    iter->first = i;                                                               \
                    break;                                                                         \
                }                                                                                  \
            }                                                                                      \
                                                                                                   \
            iter->cursor = iter->first;                                                            \
                                                                                                   \
            for (size_t i = target->capacity; i > 0; i--)                                          \
            {                                                                                      \
                if (target->buffer[i - 1].state == CMC_ES_FILLED)                                  \
                {                                                                                  \
                    iter->last = i - 1;                                                            \
                    break;                                                                         \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    bool PFX##_iter_start(SNAME##_iter *iter)                                                      \
    {                                                                                              \
        return PFX##_empty(iter->target) || iter->start;                                           \
    }                                                                                              \
                                                                                                   \
    bool PFX##_iter_end(SNAME##_iter *iter)                                                        \
    {                                                                                              \
        return PFX##_empty(iter->target) || iter->end;                                             \
    }                                                                                              \
                                                                                                   \
    void PFX##_iter_to_start(SNAME##_iter *iter)                                                   \
    {                                                                                              \
        if (!PFX##_empty(iter->target))                                                            \
        {                                                                                          \
            iter->cursor = iter->first;                                                            \
            iter->index = 0;                                                                       \
            iter->start = true;                                                                    \
            iter->end = PFX##_empty(iter->target);                                                 \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    void PFX##_iter_to_end(SNAME##_iter *iter)                                                     \
    {                                                                                              \
        if (!PFX##_empty(iter->target))                                                            \
        {                                                                                          \
            iter->cursor = iter->last;                                                             \
            iter->index = iter->target->count - 1;                                                 \
            iter->start = PFX##_empty(iter->target);                                               \
            iter->end = true;                                                                      \
        }                                                                                          \
    }                                                                                              \
                                                                                                   \
    bool PFX##_iter_next(SNAME##_iter *iter)                                                       \
    {                                                                                              \
        if (iter->end)                                                                             \
            return false;                                                                          \
                                                                                                   \
        if (iter->index + 1 == PFX##_count(iter->target))                                          \
        {                                                                                          \
            iter->end = true;                                                                      \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        iter->start = PFX##_empty(iter->target);                                                   \
                                                                                                   \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                               \
                                                                                                   \
        iter->index++;                                                                             \
                                                                                                   \
        while (1)                                                                                  \
        {                                                                                          \
            iter->cursor++;                                                                        \
            scan = &(iter->target->buffer[iter->cursor]);                                          \
                                                                                                   \
            if (scan->state == CMC_ES_FILLED)                                                      \
                break;                                                                             \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    bool PFX##_iter_prev(SNAME##_iter *iter)                                                       \
    {                                                                                              \
        if (iter->start)                                                                           \
            return false;                                                                          \
                                                                                                   \
        if (iter->index == 0)                                                                      \
        {                                                                                          \
            iter->start = true;                                                                    \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        iter->end = PFX##_empty(iter->target);                                                     \
                                                                                                   \
        SNAME##_entry *scan = &(iter->target->buffer[iter->cursor]);                               \
                                                                                                   \
        iter->index--;                                                                             \
                                                                                                   \
        while (1)                                                                                  \
        {                                                                                          \
            iter->cursor--;                                                                        \
            scan = &(iter->target->buffer[iter->cursor]);                                          \
                                                                                                   \
            if (scan->state == CMC_ES_FILLED)                                                      \
                break;                                                                             \
        }                                                                                          \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* Returns true only if the iterator moved */                                                  \
    bool PFX##_iter_advance(SNAME##_iter *iter, size_t steps)                                      \
    {                                                                                              \
        if (iter->end)                                                                             \
            return false;                                                                          \
                                                                                                   \
        if (iter->index + 1 == PFX##_count(iter->target))                                          \
        {                                                                                          \
            iter->end = true;                                                                      \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        if (steps == 0 || iter->index + steps >= PFX##_count(iter->target))                        \
            return false;                                                                          \
                                                                                                   \
        for (size_t i = 0; i < steps; i++)                                                         \
            PFX##_iter_next(iter);                                                                 \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* Returns true only if the iterator moved */                                                  \
    bool PFX##_iter_rewind(SNAME##_iter *iter, size_t steps)                                       \
    {                                                                                              \
        if (iter->start)                                                                           \
            return false;                                                                          \
                                                                                                   \
        if (iter->index == 0)                                                                      \
        {                                                                                          \
            iter->start = true;                                                                    \
            return false;                                                                          \
        }                                                                                          \
                                                                                                   \
        if (steps == 0 || iter->index < steps)                                                     \
            return false;                                                                          \
                                                                                                   \
        for (size_t i = 0; i < steps; i++)                                                         \
            PFX##_iter_prev(iter);                                                                 \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* Returns true only if the iterator was able to be positioned at the given index */           \
    bool PFX##_iter_go_to(SNAME##_iter *iter, size_t index)                                        \
    {                                                                                              \
        if (index >= PFX##_count(iter->target))                                                    \
            return false;                                                                          \
                                                                                                   \
        if (iter->index > index)                                                                   \
            return PFX##_iter_rewind(iter, iter->index - index);                                   \
        else if (iter->index < index)                                                              \
            return PFX##_iter_advance(iter, index - iter->index);                                  \
                                                                                                   \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
    V PFX##_iter_value(SNAME##_iter *iter)                                                         \
    {                                                                                              \
        if (PFX##_empty(iter->target))                                                             \
            return (V){0};                                                                         \
                                                                                                   \
        return iter->target->buffer[iter->cursor].value;                                           \
    }                                                                                              \
                                                                                                   \
    size_t PFX##_iter_multiplicity(SNAME##_iter *iter)                                             \
    {                                                                                              \
        if (PFX##_empty(iter->target))                                                             \
            return 0;                                                                              \
                                                                                                   \
        return iter->target->buffer[iter->cursor].multiplicity;                                    \
    }                                                                                              \
                                                                                                   \
    size_t PFX##_iter_index(SNAME##_iter *iter)                                                    \
    {                                                                                              \
        return iter->index;                                                                        \
    }                                                                                              \
                                                                                                   \
    static SNAME##_entry *PFX##_impl_insert_and_return(SNAME *_set_, V element, bool *new_node)    \
    {                                                                                              \
        /* If the entry already exists simply return it as we might do something with it */        \
        /* This function only guarantees that there is a valid entry for a given element */        \
                                                                                                   \
        *new_node = false;                                                                         \
                                                                                                   \
        SNAME##_entry *entry = PFX##_impl_get_entry(_set_, element);                               \
                                                                                                   \
        if (entry != NULL)                                                                         \
            return entry;                                                                          \
                                                                                                   \
        *new_node = true;                                                                          \
                                                                                                   \
        if (PFX##_full(_set_))                                                                     \
        {                                                                                          \
            if (!PFX##_resize(_set_, PFX##_capacity(_set_) + 1))                                   \
                return NULL;                                                                       \
        }                                                                                          \
                                                                                                   \
        size_t hash = _set_->hash(element);                                                        \
        size_t original_pos = hash % _set_->capacity;                                              \
        size_t pos = original_pos;                                                                 \
        /* Current multiplicity. Might change due to robin hood hashing */                         \
        size_t curr_mul = 1;                                                                       \
                                                                                                   \
        SNAME##_entry *target = &(_set_->buffer[pos]);                                             \
                                                                                                   \
        if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)                      \
        {                                                                                          \
            target->value = element;                                                               \
            target->multiplicity = curr_mul;                                                       \
            target->dist = pos - original_pos;                                                     \
            target->state = CMC_ES_FILLED;                                                         \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            while (true)                                                                           \
            {                                                                                      \
                pos++;                                                                             \
                target = &(_set_->buffer[pos % _set_->capacity]);                                  \
                                                                                                   \
                if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)              \
                {                                                                                  \
                    target->value = element;                                                       \
                    target->multiplicity = curr_mul;                                               \
                    target->dist = pos - original_pos;                                             \
                    target->state = CMC_ES_FILLED;                                                 \
                                                                                                   \
                    break;                                                                         \
                }                                                                                  \
                else if (target->dist < pos - original_pos)                                        \
                {                                                                                  \
                    /* Swap everything */                                                          \
                    V tmp = target->value;                                                         \
                    size_t tmp_dist = target->dist;                                                \
                    size_t tmp_mul = target->multiplicity;                                         \
                                                                                                   \
                    target->value = element;                                                       \
                    target->dist = pos - original_pos;                                             \
                    target->multiplicity = curr_mul;                                               \
                                                                                                   \
                    element = tmp;                                                                 \
                    original_pos = pos - tmp_dist;                                                 \
                    curr_mul = tmp_mul;                                                            \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
                                                                                                   \
        _set_->count++;                                                                            \
                                                                                                   \
        return target;                                                                             \
    }                                                                                              \
                                                                                                   \
    static SNAME##_entry *PFX##_impl_get_entry(SNAME *_set_, V element)                            \
    {                                                                                              \
        size_t hash = _set_->hash(element);                                                        \
        size_t pos = hash % _set_->capacity;                                                       \
                                                                                                   \
        SNAME##_entry *target = &(_set_->buffer[pos]);                                             \
                                                                                                   \
        while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)                  \
        {                                                                                          \
            if (_set_->cmp(target->value, element) == 0)                                           \
                return target;                                                                     \
                                                                                                   \
            pos++;                                                                                 \
            target = &(_set_->buffer[pos % _set_->capacity]);                                      \
        }                                                                                          \
                                                                                                   \
        return NULL;                                                                               \
    }                                                                                              \
                                                                                                   \
    static size_t PFX##_impl_calculate_size(size_t required)                                       \
    {                                                                                              \
        const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);       \
                                                                                                   \
        if (cmc_hashtable_primes[count - 1] < required)                                            \
            return required;                                                                       \
                                                                                                   \
        size_t i = 0;                                                                              \
        while (cmc_hashtable_primes[i] < required)                                                 \
            i++;                                                                                   \
                                                                                                   \
        return cmc_hashtable_primes[i];                                                            \
    }                                                                                              \
                                                                                                   \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_set_)                                          \
    {                                                                                              \
        SNAME##_iter iter;                                                                         \
                                                                                                   \
        PFX##_iter_init(&iter, _set_);                                                             \
                                                                                                   \
        return iter;                                                                               \
    }                                                                                              \
                                                                                                   \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_set_)                                            \
    {                                                                                              \
        SNAME##_iter iter;                                                                         \
                                                                                                   \
        PFX##_iter_init(&iter, _set_);                                                             \
        PFX##_iter_to_end(&iter);                                                                  \
                                                                                                   \
        return iter;                                                                               \
    }

#endif /* CMC_MULTISET_H */