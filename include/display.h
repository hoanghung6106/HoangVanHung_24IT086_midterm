#ifndef DISPLAY_H
#define DISPLAY_H

#include "ls.h"

void print_entry(const FileEntry *fe);
void print_long_format(const FileEntry *fe);
void format_mode(mode_t mode, char *out_str);
void format_size(off_t size, char *out_str, size_t max_len);
char get_type_suffix(mode_t mode);

#endif