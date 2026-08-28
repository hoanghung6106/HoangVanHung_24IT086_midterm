#include "ls.h"
#include "display.h"
#include "util.h"

void list_directory(const char *dirpath, bool print_header) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        fprintf(stderr, "ls: %s: %s\n", dirpath, strerror(errno));
        exit_status = 1;
        return;
    }

    if (print_header) {
        printf("\n%s:\n", dirpath);
    }

    struct dirent *dp;
    FileEntry *entries = NULL;
    size_t count = 0, capacity = 0;
    long total_blocks = 0;

    while ((dp = readdir(dir)) != NULL) {
        /* Lọc -a và -A */
        if (!opts.opt_a && !opts.opt_f) {
            if (dp->d_name[0] == '.') {
                if (!opts.opt_A) continue;
                if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
            }
        }

        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 64;
            entries = realloc(entries, capacity * sizeof(FileEntry));
        }

        FileEntry *fe = &entries[count++];
        fe->name = strdup(dp->d_name);
        
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, dp->d_name);
        fe->path = strdup(fullpath);

        if (lstat(fe->path, &fe->sb) == 0) {
            fe->stat_ok = true;
            total_blocks += fe->sb.st_blocks;
        } else {
            fe->stat_ok = false;
        }
    }
    closedir(dir);

    /* Sắp xếp */
    if (!opts.opt_f) {
        qsort(entries, count, sizeof(FileEntry), compare_entries);
    }

    /* In dòng total blocks theo chuẩn POSIX */
    if ((opts.opt_l || opts.opt_n || opts.opt_s) && count > 0) {
        if (opts.opt_k) total_blocks = (total_blocks + 1) / 2;
        printf("total %ld\n", total_blocks);
    }

    /* Hiển thị */
    for (size_t i = 0; i < count; i++) {
        print_entry(&entries[i]);
    }

    /* Đệ quy -R nếu được bật */
    if (opts.opt_R) {
        for (size_t i = 0; i < count; i++) {
            if (entries[i].stat_ok && S_ISDIR(entries[i].sb.st_mode)) {
                if (strcmp(entries[i].name, ".") != 0 && strcmp(entries[i].name, "..") != 0) {
                    list_directory(entries[i].path, true);
                }
            }
        }
    }

    /* Dọn dẹp bộ nhớ */
    for (size_t i = 0; i < count; i++) {
        free(entries[i].name);
        free(entries[i].path);
    }
    free(entries);
}

void process_operands(int argc, char *argv[]) {
    if (argc == 0) {
        if (opts.opt_d) {
            FileEntry fe;
            fe.name = ".";
            fe.path = ".";
            lstat(".", &fe.sb);
            print_entry(&fe);
        } else {
            list_directory(".", false);
        }
        return;
    }

    FileEntry *files = NULL;
    FileEntry *dirs = NULL;
    size_t file_cnt = 0, dir_cnt = 0;

    for (int i = 0; i < argc; i++) {
        FileEntry fe;
        fe.name = argv[i];
        fe.path = argv[i];
        if (lstat(fe.path, &fe.sb) != 0) {
            fprintf(stderr, "ls: %s: %s\n", fe.name, strerror(errno));
            exit_status = 1;
            continue;
        }
        fe.stat_ok = true;

        if (opts.opt_d || !S_ISDIR(fe.sb.st_mode)) {
            files = realloc(files, (file_cnt + 1) * sizeof(FileEntry));
            files[file_cnt++] = fe;
        } else {
            dirs = realloc(dirs, (dir_cnt + 1) * sizeof(FileEntry));
            dirs[dir_cnt++] = fe;
        }
    }

    if (!opts.opt_f) {
        if (file_cnt > 0) qsort(files, file_cnt, sizeof(FileEntry), compare_entries);
        if (dir_cnt > 0) qsort(dirs, dir_cnt, sizeof(FileEntry), compare_entries);
    }

    /* Liệt kê các non-directory trước */
    for (size_t i = 0; i < file_cnt; i++) {
        print_entry(&files[i]);
    }

    /* Liệt kê các thư mục */
    for (size_t i = 0; i < dir_cnt; i++) {
        bool show_header = (argc > 1 || opts.opt_R);
        list_directory(dirs[i].path, show_header);
    }

    free(files);
    free(dirs);
}