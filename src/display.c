#include "display.h"
#include "util.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>

void format_mode(mode_t mode, char *out) {
    if (S_ISDIR(mode)) out[0] = 'd';
    else if (S_ISLNK(mode)) out[0] = 'l';
    else if (S_ISCHR(mode)) out[0] = 'c';
    else if (S_ISBLK(mode)) out[0] = 'b';
    else if (S_ISFIFO(mode)) out[0] = 'p';
    else if (S_ISSOCK(mode)) out[0] = 's';
    else out[0] = '-';

    out[1] = (mode & S_IRUSR) ? 'r' : '-';
    out[2] = (mode & S_IWUSR) ? 'w' : '-';
    if (mode & S_ISUID) out[3] = (mode & S_IXUSR) ? 's' : 'S';
    else out[3] = (mode & S_IXUSR) ? 'x' : '-';

    out[4] = (mode & S_IRGRP) ? 'r' : '-';
    out[5] = (mode & S_IWGRP) ? 'w' : '-';
    if (mode & S_ISGID) out[6] = (mode & S_IXGRP) ? 's' : 'S';
    else out[6] = (mode & S_IXGRP) ? 'x' : '-';

    out[7] = (mode & S_IROTH) ? 'r' : '-';
    out[8] = (mode & S_IWOTH) ? 'w' : '-';
    if (mode & S_ISVTX) out[9] = (mode & S_IXOTH) ? 't' : 'T';
    else out[9] = (mode & S_IXOTH) ? 'x' : '-';

    out[10] = '\0';
}

void format_size(off_t size, char *out, size_t max_len) {
    if (opts.opt_h) {
        const char *units[] = {"B", "K", "M", "G", "T"};
        double s = (double)size;
        int i = 0;
        while (s >= 1024.0 && i < 4) {
            s /= 1024.0;
            i++;
        }
        if (i == 0) snprintf(out, max_len, "%lld%s", (long long)size, units[i]);
        else snprintf(out, max_len, "%.1f%s", s, units[i]);
    } else {
        snprintf(out, max_len, "%lld", (long long)size);
    }
}

char get_type_suffix(mode_t mode) {
    if (!opts.opt_F) return '\0';
    if (S_ISDIR(mode)) return '/';
    if (S_ISLNK(mode)) return '@';
    if (S_ISFIFO(mode)) return '|';
    if (S_ISSOCK(mode)) return '=';
    if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) return '*';
    return '\0';
}

void print_long_format(const FileEntry *fe) {
    char modestr[12];
    format_mode(fe->sb.st_mode, modestr);

    if (opts.opt_i) printf("%llu ", (unsigned long long)fe->sb.st_ino);

    if (opts.opt_s) {
        long blk = fe->sb.st_blocks;
        if (opts.opt_k) blk = (blk + 1) / 2;
        printf("%ld ", blk);
    }

    printf("%s %2u ", modestr, (unsigned int)fe->sb.st_nlink);

    if (opts.opt_n) {
        printf("%-8u %-8u ", (unsigned int)fe->sb.st_uid, (unsigned int)fe->sb.st_gid);
    } else {
        struct passwd *pw = getpwuid(fe->sb.st_uid);
        struct group *gr = getgrgid(fe->sb.st_gid);
        if (pw) printf("%-8s ", pw->pw_name);
        else printf("%-8u ", (unsigned int)fe->sb.st_uid);

        if (gr) printf("%-8s ", gr->gr_name);
        else printf("%-8u ", (unsigned int)fe->sb.st_gid);
    }

    if (S_ISCHR(fe->sb.st_mode) || S_ISBLK(fe->sb.st_mode)) {
        printf("%3d, %3d ", major(fe->sb.st_rdev), minor(fe->sb.st_rdev));
    } else {
        char sbuf[32];
        format_size(fe->sb.st_size, sbuf, sizeof(sbuf));
        printf("%8s ", sbuf);
    }

    time_t ftime = get_entry_time(fe);
    time_t now = time(NULL);
    struct tm *tm = localtime(&ftime);
    char timebuf[64];
    if (now - ftime > 180 * 86400 || ftime > now) {
        strftime(timebuf, sizeof(timebuf), "%b %e  %Y", tm);
    } else {
        strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm);
    }
    printf("%s ", timebuf);

    print_sanitized_name(fe->name);
    char suf = get_type_suffix(fe->sb.st_mode);
    if (suf) putchar(suf);

    if (S_ISLNK(fe->sb.st_mode)) {
        char linktarget[PATH_MAX];
        ssize_t len = readlink(fe->path, linktarget, sizeof(linktarget) - 1);
        if (len != -1) {
            linktarget[len] = '\0';
            printf(" -> %s", linktarget);
        }
    }
    putchar('\n');
}

void print_entry(const FileEntry *fe) {
    if (opts.opt_l || opts.opt_n) {
        print_long_format(fe);
        return;
    }

    if (opts.opt_i) printf("%llu ", (unsigned long long)fe->sb.st_ino);
    if (opts.opt_s) {
        long blk = fe->sb.st_blocks;
        if (opts.opt_k) blk = (blk + 1) / 2;
        printf("%ld ", blk);
    }

    print_sanitized_name(fe->name);
    char suf = get_type_suffix(fe->sb.st_mode);
    if (suf) putchar(suf);
    putchar('\n');
}
