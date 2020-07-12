#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// error utility macro
#define ASSERT_EXIT(cond, message) \
do { \
    if((!cond)) { \
        printf("error at: %s, line: %d\n", __func__, __LINE__); \
        perror((message)); \
        exit(1); \
    } \
} while (0)

static inline bool str_end_with(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

#endif