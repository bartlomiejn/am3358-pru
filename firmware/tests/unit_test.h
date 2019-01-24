#ifndef _UNIT_TEST_H
#define _UNIT_TEST_H

#include <stdio.h>
    
#define test_assert(test) \
    if (!(test)) \
    { \
        return #test; \
    }

#define test_setup_start(test) \
    static void setup_test(void) \
    {

#define test_setup_end() \
    }
    
#define test_teardown_start(test) \
    static void setup_test(void) \
    {

#define test_teardown_end() \
    }
    
#define test_function_start(test) \
    static char *test(void) \
    {

#define test_function_end() \
        return 0; \
    }
    
#define test_case(name) void name(void)

#define test_case_start(name) \
    void name(void) { \
        printf("%s: Started\n", #name); \
        void (*setup)(void) = NULL; \
        void (*teardown)(void) = NULL;

#define test_case_end() \
        printf("\n"); \
    }
    
#define add_setup(test) \
    setup = setup_test

#define add_teardown(test) \
    teardown = teardown_test
    
#define add_test_function(test) \
    { \
        if (setup) setup(); \
        char *message = test(); \
        tests_ran++; \
        if (teardown) teardown(); \
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
