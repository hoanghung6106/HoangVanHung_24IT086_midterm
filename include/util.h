#ifndef UTIL_H
#define UTIL_H

#include "ls.h"

time_t get_entry_time(const FileEntry *fe);
int compare_entries(const void *a, const void *b);
void print_sanitized_name(const char *name);

#endif