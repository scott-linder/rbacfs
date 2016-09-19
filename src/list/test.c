#include <stdio.h>
#include <assert.h>
#include "lib.h"

int main(void) {
    char *vals[] = { "foo", "bar", "baz" };
    size_t len = sizeof(vals) / sizeof(vals[0]);
    struct list *list = LIST_INIT;
    for (size_t i = 0; i < len; i++) {
        list_append(&list, vals[i]);
    }
    size_t i = 0;
    for (struct list *cur = list; cur; cur = list_next(cur)) {
        assert(list_value(cur) == vals[i++]);
    }

    char *test_vals[] = { "test1", "testing", "tested" };
    size_t test_len = sizeof(test_vals) / sizeof(test_vals[0]);
    struct list *test_list = LIST_INIT;
    for (size_t i = 0; i < test_len; i++) {
        list_append(&test_list, test_vals[i]);
    }
    i = 0;
    for (struct list *test_cur = test_list; test_cur; test_cur = list_next(test_cur)) {
        assert(list_value(test_cur) == test_vals[i++]);
    }  

    char *test_vals1[] = { "", " ", "...", "!@/*" };
    size_t test_len1 = sizeof(test_vals1) / sizeof(test_vals1[0]);
    struct list *test_list1 = LIST_INIT;
    for (size_t i = 0; i < test_len1; i++) {
        list_append(&test_list1, test_vals1[i]);
    }
    i = 0;
    for (struct list *test_cur1 = test_list1; test_cur1; test_cur1 = list_next(test_cur1)) {
        assert(list_value(test_cur1) == test_vals1[i++]);
    }

    char *test_vals2[] = { "123", "------", "1-2" };
    size_t test_len2 = sizeof(test_vals2) / sizeof(test_vals2[0]);
    struct list *test_list2 = LIST_INIT;
    for (size_t i = 0; i < test_len2; i++) {
        list_append(&test_list2, test_vals2[i]);
    }
    i = 0;
    for (struct list *test_cur2 = test_list2; test_cur2; test_cur2 = list_next(test_cur2)) {
        assert(list_value(test_cur2) == test_vals2[i++]);
    }  

}
