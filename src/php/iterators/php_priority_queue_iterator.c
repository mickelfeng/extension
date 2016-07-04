#include "php.h"
#include "zend_exceptions.h"

#include "../../common.h"
#include "../../ds/ds_priority_queue.h"
#include "php_priority_queue_iterator.h"

static void priority_queue_iterator_dtor(zend_object_iterator *iter)
{
    DTOR_AND_UNDEF(&iter->data);
}

static int priority_queue_iterator_valid(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? FAILURE : SUCCESS;
}

static zval *priority_queue_iterator_get_current_data(zend_object_iterator *iter)
{
    return &iter->data;
}

static void priority_queue_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((PriorityQueueIterator *) iter)->position);
}

static void priority_queue_iterator_move_forward(zend_object_iterator *iter)
{
    PriorityQueueIterator *iterator = (PriorityQueueIterator *) iter;

    if ( ! PRIORITY_QUEUE_IS_EMPTY(iterator->queue)) {
        priority_queue_pop(iterator->queue, &iter->data);
        iterator->position++;
    } else {
        ZVAL_UNDEF(&iter->data);
    }
}

static void priority_queue_iterator_rewind(zend_object_iterator *iter)
{
    PriorityQueueIterator *iterator = (PriorityQueueIterator *) iter;

    if ( ! PRIORITY_QUEUE_IS_EMPTY(iterator->queue)) {
        priority_queue_pop(iterator->queue, &iter->data);
    }

    iterator->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
    priority_queue_iterator_dtor,
    priority_queue_iterator_valid,
    priority_queue_iterator_get_current_data,
    priority_queue_iterator_get_current_key,
    priority_queue_iterator_move_forward,
    priority_queue_iterator_rewind
};

zend_object_iterator *priority_queue_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    PriorityQueueIterator *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(PriorityQueueIterator));
    zend_iterator_init((zend_object_iterator*) iterator);

    ZVAL_UNDEF(&iterator->intern.data);

    iterator->intern.funcs = &iterator_funcs;
    iterator->queue        = Z_PRIORITY_QUEUE_P(object);
    iterator->position     = 0;

    return (zend_object_iterator *) iterator;
}
