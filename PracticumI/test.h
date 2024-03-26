#pragma once

#define ERROR(info, ...)                                                   \
    {                                                                      \
        fprintf(stderr, "ERROR: %s,%d,%s ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, info, ##__VA_ARGS__);                              \
        fprintf(stderr, "\n");                                             \
    }
