/**=========================================================**
  * SPDX-License-Identifier: GPL-3.0-or-later               *
  * Copyright (c) 2025- The Nordix Authors                  *
  * Part of Yggdrasil - Nordix desktop environment          *
 **=========================================================*/
/*
Nordix paru search
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#define SEPARATOR_NORDIX SLATEGRAY_1"𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹"
#define SEPARATOR SLATEGRAY_1 "▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔"      

#define SLATEGRAY_1 "\033[38;2;94;104;109m"


#define YELLOW "\033[33m"
#define RED "\033[38;2;255;0;0m"
#define WHITE "\033[38;2;255;255;255m"
#define MAGENTA "\033[35m"
#define NARTIC "\033[38;2;156;230;255m"
#define NVFROST "\033[38;2;210;255;255m"
#define NCYAN "\033[38;2;0;255;255m"
#define NVBLUE "\033[38;2;175;251;253m"
#define NGLACIER "\033[38;2;80;200;255m"

#define UNDERLINE "\033[4m"
#define RESET     "\033[0m"

#define SCROLL_OFF 0
#define SCROLL_UP 1
#define SCROLL_DOWN 2
#define MAX_PACKAGES 3048
#define MAX_PKG_NAME 356

void scroll(int direction) {
    if (direction == SCROLL_OFF) return;

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
        if (fd < 0) return;
    }

    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_WHEEL);
    ioctl(fd, UI_SET_RELBIT, REL_WHEEL_HI_RES);

    struct uinput_setup usetup = {0};
    snprintf(usetup.name, UINPUT_MAX_NAME_SIZE, "nordix-scroll-%d", getpid());
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    usetup.id.version = 1;

    if (ioctl(fd, UI_DEV_SETUP, &usetup) < 0) {
        close(fd);
        return;
    }

    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        close(fd);
        return;
    }

    usleep(200000);  // Sleep

    struct input_event ev;
    int wheel_value = (direction == SCROLL_UP) ? 18000 : -18000;
    int hi_res_value = (direction == SCROLL_UP) ? 280000 : -280000;

    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_REL;
    ev.code = REL_WHEEL;
    ev.value = wheel_value;
    write(fd, &ev, sizeof(ev));

    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_REL;
    ev.code = REL_WHEEL_HI_RES;
    ev.value = hi_res_value;
    write(fd, &ev, sizeof(ev));

    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    write(fd, &ev, sizeof(ev));
     memset(&ev, 0, sizeof(ev));

         usleep(100000);
    gettimeofday(&ev.time, NULL);
    ev.type = EV_REL;
    ev.code = REL_WHEEL;
    ev.value = wheel_value;
    write(fd, &ev, sizeof(ev));

    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_REL;
    ev.code = REL_WHEEL_HI_RES;
    ev.value = hi_res_value;
    write(fd, &ev, sizeof(ev));

    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    write(fd, &ev, sizeof(ev));
            usleep(100000);
    gettimeofday(&ev.time, NULL);
    ev.type = EV_REL;
    ev.code = REL_WHEEL;
    ev.value = wheel_value;
    write(fd, &ev, sizeof(ev));

    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_REL;
    ev.code = REL_WHEEL_HI_RES;
    ev.value = hi_res_value;
    write(fd, &ev, sizeof(ev));

    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    write(fd, &ev, sizeof(ev));
    usleep(100000);  // Sleep

    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

void print_help(const char *progname) {
    printf("%s\n", SEPARATOR_NORDIX);
    printf(UNDERLINE NARTIC "Nordix package search - Help" RESET "\n\n");
    printf(WHITE "Usage: %s " NARTIC "[options] " NCYAN "<search term>\n\n" RESET, progname);
    printf(NVBLUE "Options:\n" RESET);
    printf("  --scroll-0    " WHITE "No auto scroll\n" RESET);
    printf("  --scroll-1    " WHITE "Scroll up after search (default)\n" RESET);
    printf("  --scroll-2    " WHITE "Scroll down after search\n" RESET);
    printf("  --no-size     " WHITE "Skip size lookup (faster)\n" RESET);
    printf("  --help, -h    " WHITE "Show this help message\n\n" RESET);
    printf(NGLACIER "Description:\n" RESET);
    printf("  " WHITE "Nice aur search with auto scroll.\n\n" RESET);
    printf("%s\n", SEPARATOR_NORDIX);
}

void strip_colors(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src == '\033') {
            while (*src && *src != 'm') src++;
            if (*src) src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

// Quick hashmap for sizes
typedef struct {
    char name[MAX_PKG_NAME];
    char size[64];
} PkgSize;

PkgSize *size_map = NULL;
int size_map_count = 0;

const char* get_size(const char *pkg_name) {
    for (int i = 0; i < size_map_count; i++) {
        if (strcmp(size_map[i].name, pkg_name) == 0) {
            return size_map[i].size;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    int scroll_mode = SCROLL_UP;
    int skip_sizes = 0;
    char *search_terms[256];
    int search_count = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "--scroll-0") == 0) scroll_mode = SCROLL_OFF;
        else if (strcmp(argv[i], "--scroll-1") == 0) scroll_mode = SCROLL_UP;
        else if (strcmp(argv[i], "--scroll-2") == 0) scroll_mode = SCROLL_DOWN;
        else if (strcmp(argv[i], "--no-size") == 0) skip_sizes = 1;
        else search_terms[search_count++] = argv[i];
    }

    if (search_count == 0) {
        fprintf(stderr, RED "Error: " WHITE "No search term provided\n" RESET);
        print_help(argv[0]);
        return 1;
    }

    // Build the paru command
    char cmd[1024] = "PAGER=cat paru -Ss --color=always ";
    for (int i = 0; i < search_count; i++) {
        strcat(cmd, search_terms[i]);
        if (i < search_count - 1) strcat(cmd, " ");
    }

    char tmpfile[256];
    snprintf(tmpfile, sizeof(tmpfile), "/tmp/nordix_paru_%d.txt", getpid());

    char full_cmd[2048];
    snprintf(full_cmd, sizeof(full_cmd), "%s > %s 2>&1", cmd, tmpfile);
    system(full_cmd);

    // First pass: collect package names and repos
    FILE *fp = fopen(tmpfile, "r");
    if (!fp) {
        perror("fopen failed");
        return 1;
    }

    char line[4096];
    char (*pkg_names)[MAX_PKG_NAME] = malloc(MAX_PACKAGES * MAX_PKG_NAME);
    int *is_aur = malloc(MAX_PACKAGES * sizeof(int));

    if (!pkg_names || !is_aur) {
        perror("malloc failed");
        if (fp) fclose(fp);
        return 1;
    }

    int pkg_count = 0;
    int installed_pkgs = 0;
    int aur_count = 0;
    int repo_count = 0;

    while (fgets(line, sizeof(line), fp) && pkg_count < MAX_PACKAGES) {
        char clean_line[4096];
        strcpy(clean_line, line);
        strip_colors(clean_line);

        if (clean_line[0] != ' ' && clean_line[0] != '\t' && clean_line[0] != '\n') {
            char *space = strchr(clean_line, ' ');
            if (space) {
                int len = space - clean_line;
                char pkg_full[256];
                strncpy(pkg_full, clean_line, len);
                pkg_full[len] = 0;

                // Check if it is AUR
                is_aur[pkg_count] = (strncmp(pkg_full, "aur/", 4) == 0);
                if (is_aur[pkg_count]) aur_count++;
                else repo_count++;

                char *slash = strchr(pkg_full, '/');
                if (slash) {
                    strncpy(pkg_names[pkg_count], slash + 1, MAX_PKG_NAME - 1);
                } else {
                    strncpy(pkg_names[pkg_count], pkg_full, MAX_PKG_NAME - 1);
                }
                pkg_names[pkg_count][MAX_PKG_NAME - 1] = 0;

                if (strstr(line, "[Installed") != NULL) {
                    installed_pkgs++;
                }
                pkg_count++;
            }
        }
    }
    fclose(fp);

    // Get disk space
    char diskspace[32] = "?";
    FILE *df = popen("df -h / | awk 'NR==2 {print $4}' | sed 's/G/ GB/'", "r");
    if (df) {
        if (fgets(diskspace, sizeof(diskspace), df)) {
            diskspace[strcspn(diskspace, "\n")] = 0;
        }
        pclose(df);
    }

    // Get sizes ONLY for repo packages, in ONE single run
    size_map = malloc(MAX_PACKAGES * sizeof(PkgSize));
    size_map_count = 0;

    if (!skip_sizes && repo_count > 0) {
        char size_file[256];
        snprintf(size_file, sizeof(size_file), "/tmp/nordix_sizes_%d.txt", getpid());

        char pkg_list_file[256];
        snprintf(pkg_list_file, sizeof(pkg_list_file), "/tmp/nordix_pkglist_%d.txt", getpid());
        FILE *pkg_fp = fopen(pkg_list_file, "w");
        if (pkg_fp) {
            for (int i = 0; i < pkg_count; i++) {
                if (!is_aur[i]) {
                    fprintf(pkg_fp, "%s\n", pkg_names[i]);
                }
            }
            fclose(pkg_fp);

            char expac_cmd[512];
            snprintf(expac_cmd, sizeof(expac_cmd),
                    "cat %s | xargs expac -H M -S '%%n %%m' 2>/dev/null > %s",
                    pkg_list_file, size_file);
            system(expac_cmd);

            FILE *sfp = fopen(size_file, "r");
            if (sfp) {
                char size_line[256];
                while (fgets(size_line, sizeof(size_line), sfp) && size_map_count < MAX_PACKAGES) {
                    size_line[strcspn(size_line, "\n")] = 0;
                    char *space = strchr(size_line, ' ');
                    if (space) {
                        *space = '\0';
                        strncpy(size_map[size_map_count].name, size_line, MAX_PKG_NAME - 1);
                        strncpy(size_map[size_map_count].size, space + 1, 63);
                        size_map_count++;
                    }
                }
                fclose(sfp);
            }

            remove(pkg_list_file);
            remove(size_file);
        }
    }

    // Clear and show header
    system("clear");
    printf("%s\n", SEPARATOR_NORDIX);
    printf("🧊 " UNDERLINE NARTIC "Nordix package search" RESET " " YELLOW " Paru" WHITE "/" NCYAN "aur" WHITE ": ");
    for (int i = 0; i < search_count; i++) {
        printf("%s", search_terms[i]);
        if (i < search_count - 1) printf(" ");
    }
    printf("\n");

    printf(NARTIC "Scroll mode:" WHITE " ");
    switch(scroll_mode) {
        case SCROLL_OFF: printf("Off\n"); break;
        case SCROLL_UP: printf("Up (default)\n"); break;
        case SCROLL_DOWN: printf("Down\n"); break;
    }

    printf(NVFROST "Found:" WHITE " %d " NARTIC "(" NCYAN "AUR:" WHITE " %d" NARTIC ", " YELLOW "Repo:" WHITE " %d" NARTIC ")"
           NVFROST " Installed:" WHITE " %d" NVFROST " Diskspace:" WHITE " %s\n",
           pkg_count, aur_count, repo_count, installed_pkgs, diskspace);
    printf("%s\n", SEPARATOR_NORDIX);
    printf("\n");

    // Second pass: print results
    fp = fopen(tmpfile, "r");
    if (!fp) {
        perror("fopen failed");
        return 1;
    }

    int first = 1;
    int pkg_index = -1;
    int in_package = 0;

    while (fgets(line, sizeof(line), fp)) {
        char clean_line[4096];
        strcpy(clean_line, line);
        strip_colors(clean_line);

        if (clean_line[0] != ' ' && clean_line[0] != '\t' && clean_line[0] != '\n') {
            if (!first) printf("\n%s\n", SEPARATOR);
            else first = 0;

            fputs(line, stdout);
            pkg_index++;
            in_package = 1;
        } else {
            fputs(line, stdout);

            if (in_package && pkg_index >= 0 && pkg_index < pkg_count) {
                if (is_aur[pkg_index]) {
                    printf("  " MAGENTA "AUR package" RESET " (size unknown until build)\n");
                } else if (!skip_sizes) {
                    const char *size = get_size(pkg_names[pkg_index]);
                    if (size) {
                        printf("  " YELLOW "Installed Size:" RESET " %s\n", size);
                    }
                }
                in_package = 0;
            }
        }
    }
    fclose(fp);

    printf("\n%s\n", SEPARATOR);
usleep(100000);
    // SCROLL


    // Clean up
    free(pkg_names);
    free(is_aur);
    free(size_map);
    remove(tmpfile);

    if (scroll_mode != SCROLL_OFF) {
        scroll(scroll_mode);
    }
    usleep(100000);
    return 0;
}
