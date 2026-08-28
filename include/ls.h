#ifndef LS_H
#define LS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

typedef struct {
    bool opt_A;
    bool opt_a;
    bool opt_c;
    bool opt_d;
    bool opt_F;
    bool opt_f;
    bool opt_h;
    bool opt_i;
    bool opt_k;
    bool opt_l;
    bool opt_n;
    bool opt_q;
    bool opt_R;
    bool opt_r;
    bool opt_S;
    bool opt_s;
    bool opt_t;
    bool opt_u;
    bool opt_w;
} LsOptions;

typedef struct {
    char *name;
    char *path;
    struct stat sb;
    bool stat_ok;
} FileEntry;

extern LsOptions opts;
extern int exit_status;

void process_operands(int argc, char *argv[]);
void list_directory(const char *dirpath, bool print_header);

#endif
