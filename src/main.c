#include "ls.h"

LsOptions opts;
int exit_status = 0;

int main(int argc, char *argv[]) {
    int ch;
    memset(&opts, 0, sizeof(opts));

    /* Root tự động bật -A nếu không truyền -a */
    if (geteuid() == 0) {
        opts.opt_A = true;
    }

    while ((ch = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (ch) {
            case 'A': opts.opt_A = true; break;
            case 'a': opts.opt_a = true; break;
            case 'c': opts.opt_c = true; opts.opt_u = false; break;
            case 'd': opts.opt_d = true; break;
            case 'F': opts.opt_F = true; break;
            case 'f': opts.opt_f = true; opts.opt_a = true; break;
            case 'h': opts.opt_h = true; opts.opt_k = false; break;
            case 'i': opts.opt_i = true; break;
            case 'k': opts.opt_k = true; opts.opt_h = false; break;
            case 'l': opts.opt_l = true; opts.opt_n = false; break;
            case 'n': opts.opt_n = true; opts.opt_l = false; break;
            case 'q': opts.opt_q = true; opts.opt_w = false; break;
            case 'R': opts.opt_R = true; break;
            case 'r': opts.opt_r = true; break;
            case 'S': opts.opt_S = true; break;
            case 's': opts.opt_s = true; break;
            case 't': opts.opt_t = true; break;
            case 'u': opts.opt_u = true; opts.opt_c = false; break;
            case 'w': opts.opt_w = true; opts.opt_q = false; break;
            default:
                fprintf(stderr, "usage: ls [-AacdFfhiklnqRrSstuw] [file ...]\n");
                exit(EXIT_FAILURE);
        }
    }

    argc -= optind;
    argv += optind;

    process_operands(argc, argv);

    return exit_status;
}