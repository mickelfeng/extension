#include "../../common.h"
#include "../../ds/ds_vector.h"
#include "../../ds/ds_deque.h"
#include "../../ds/ds_stack.h"
#include "../../ds/ds_queue.h"
#include "../../ds/ds_priority_queue.h"
#include "../../ds/ds_map.h"
#include "../../ds/ds_set.h"
#include "../../ds/ds_pair.h"

#include "../objects/php_ds_vector.h"
#include "../objects/php_ds_deque.h"

/**
 *
 */
#define METHOD(name) PHP_METHOD(ds, name)

/**
 *
 */
#define DS_ME(name) \
    PHP_ME(ds, name, arginfo##_##name, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)

zend_class_entry *ds_ce;

/**
 * ds::vector(...)
 */
ARGINFO_OPTIONAL_ZVAL_RETURN_COLLECTION(vector, values, Vector);
METHOD(vector)
{
    ds_vector_t *vector;
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            vector = ds_vector_ex(Z_LVAL_P(values));
        } else {
            vector = ds_vector();
            ds_vector_push_all(vector, values);
        }
    } else {
        vector = ds_vector();
    }

    RETURN_DS_VECTOR(vector);
}

/**
 * ds::deque(...)
 */
ARGINFO_OPTIONAL_ZVAL_RETURN_COLLECTION(deque, values, Deque);
METHOD(deque)
{
    ds_deque_t *deque;
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            deque = ds_deque_ex(Z_LVAL_P(values));
        } else {
            deque = ds_deque();
            ds_deque_push_all(deque, values);
        }
    } else {
        deque = ds_deque();
    }

    RETURN_DEQUE(deque);
}

/**
 * ds::stack(...)
 */
ARGINFO_OPTIONAL_ZVAL_RETURN_DS(stack, values, Stack);
METHOD(stack)
{
    php_ds_stack_t *stack = php_ds_stack_init();
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            php_ds_stack_user_allocate(stack, Z_LVAL_P(values));
        } else {
            php_ds_stack_push_all(stack, values);
        }
    }

    RETURN_DS_STACK(stack);
}

/**
 * ds::queue(...)
 */
ARGINFO_OPTIONAL_ZVAL_RETURN_DS(queue, values, Queue);
METHOD(queue)
{
    Queue *queue = queue_init();
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            queue_user_allocate(queue, Z_LVAL_P(values));
        } else {
            queue_push_all(queue, values);
        }
    }

    RETURN_QUEUE(queue);
}

/**
 * ds::priority_queue()
 */
ARGINFO_NONE_RETURN_DS(priority_queue, PriorityQueue);
METHOD(priority_queue)
{
    RETURN_PRIORITY_QUEUE(priority_queue_init());
}

/**
 * ds::map(...)
 */
ARGINFO_OPTIONAL_ZVAL_RETURN_DS(map, values, Map);
METHOD(map)
{
    Map *map = map_create();
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            map_user_allocate(map, Z_LVAL_P(values));
        } else {
            map_put_all(map, values);
        }
    }

    RETURN_MAP(map);
}

/**
 * ds::set(...)
 */
ARGINFO_OPTIONAL_ZVAL_RETURN_DS(set, values, Set);
METHOD(set)
{
    Set *set = set_init();
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            set_user_allocate(set, Z_LVAL_P(values));
        } else {
            set_add_all(set, values);
        }
    }

    RETURN_SET(set);
}

/**
 * ds::pair(...)
 */
ARGINFO_ZVAL_ZVAL(pair, key, value)
METHOD(pair)
{
    PARSE_ZVAL_ZVAL(key, value);
    pair_create_as_zval(key, value, return_value);
}

void register_ds()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        DS_ME(vector)
        DS_ME(deque)
        DS_ME(stack)
        DS_ME(queue)
        DS_ME(priority_queue)
        DS_ME(map)
        DS_ME(set)
        DS_ME(pair)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, "ds", methods);

    ds_ce = zend_register_internal_class(&ce);
    ds_ce->ce_flags      |= ZEND_ACC_FINAL;    // Can't extend
    ds_ce->ce_flags      |= ZEND_ACC_ABSTRACT; // Can't construct
}

