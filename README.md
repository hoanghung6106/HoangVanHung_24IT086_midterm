# Midterm Project: Implementation of UNIX ls(1)

- **Student Name:** Hoàng Văn Hưng
- **Student ID:** 24IT086
- **Repository:** https://github.com/hoangvanhung/HoangVanHung_24IT86_midterm

## 1. Project Overview
This project develops a modular, robust clone of the NetBSD `ls(1)` command written in C, adhering strictly to the manual page specifications.

## 2. Implemented Features
- **File Entry Filtering:** Supports `-a` (include dots), `-A` (exclude `.` and `..`), and super-user default behaviors.
- **Sorting Logic:** Default lexicographical, `-t` (modification time), `-S` (file size), `-r` (reverse sort), and `-f` (unsorted).
- **Time Representation:** Toggles between mtime, `-c` (status change time), and `-u` (access time).
- **Listing Modes:** Standard format, `-d` (directories as files), and `-R` (recursive traversal).
- **Long Format (`-l`, `-n`):** Parses `struct stat` to display permissions (`strmode` equivalent), hard links, numeric/name UID and GID, byte counts, major/minor device numbers for character/block special files, and symlink targets (`->`).
- **Additional Formatting:** `-F` (type indicators `/`, `*`, `@`, `=`, `|`), `-h` / `-k` (humanized and KB sizes), `-s` (allocated blocks), `-i` (inode numbers), and `-q` / `-w` (non-printable character sanitization).

## 3. How to Build and Run
```bash
# Build the project
make

# Run examples
./ls -la
./ls -lF /dev
./ls -lRSh include/

# Clean build artifacts
make clean