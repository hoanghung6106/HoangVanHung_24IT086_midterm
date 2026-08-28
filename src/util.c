#include "util.h"
#include <ctype.h>

time_t get_entry_time(const FileEntry *fe) {
    if (opts.opt_c) return fe->sb.st_ctime;
    if (opts.opt_u) return fe->sb.st_atime;
    return fe->sb.st_mtime;
}

int compare_entries(const void *a, const void *b) {
    const FileEntry *fa = (const FileEntry *)a;
    const FileEntry *fb = (const FileEntry *)b;
    int res = 0;

    if (opts.opt_f) return 0; /* -f: không sắp xếp */

    if (opts.opt_S) {
        if (fa->sb.st_size < fb->sb.st_size) res = 1;
        else if (fa->sb.st_size > fb->sb.st_size) res = -1;
        else res = strcmp(fa->name, fb->name);
    } else if (opts.opt_t) {
        time_t ta = get_entry_time(fa);
        time_t tb = get_entry_time(fb);
        if (ta < tb) res = 1;
        else if (ta > tb) res = -1;
        else res = strcmp(fa->name, fb->name);
    } else {
        res = strcmp(fa->name, fb->name);
    }

    return opts.opt_r ? -res : res;
}

void print_sanitized_name(const char *name) {
    bool term = isatty(STDOUT_FILENO);
    bool force_q = opts.opt_q || (!opts.opt_w && term);

    for (const char *p = name; *p != '\0'; p++) {
        if (!isprint((unsigned char)*p) && force_q) {
            putchar('?');
        } else {
            putchar(*p);
        }
    }
}