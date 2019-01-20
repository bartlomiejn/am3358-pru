#ifndef _UNIT_TEST_H
#define _UNIT_TEST_H

#include <stdio.h>

#define test_case(name) void name(void)

#define test_case_start(name) \
    void name(void) { \
        printf("%s: Started\n", #name);

#define test_case_end() \
        printf("\n"); \
    }
    
#define test_assert(test) \
    if (!(test)) \
    { \
        return #test; \
    }

#define add_test(test) \
    { \
        setup(); \
        char *message = test(); \
        tests_ran++; \
        teardown(); \
        if (message != 0) \
        { \
            printf("%s: Assertion failed on `%s`\n", #test, message); \
            tests_failed++; \
        } \
        else \
        { \
           printf("%s: Success\n", #test); \
        } \
    }

extern int tests_ran;
extern int tests_failed;

#endif
