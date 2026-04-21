/**=========================================================**
  * SPDX-License-Identifier: GPL-3.0-or-later               *
  * Copyright (c) 2025- The Nordix Authors                  *
  * Part of Yggdrasil - Nordix desktop environment          *
 **=========================================================*/
/*
Nordix pacman search
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

#define SEPARATOR_NORDIX SLATEGRAY_1"𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹𝈸𝈹"
#define SEPARATOR SLATEGRAY_1 "▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔"
#define SLATEGRAY_1 "\033[38;2;94;104;109m"
#define NVFROST "\033[38;2;210;255;255m"
#define YELLOW "\033[33m"
#define WHITE "\033[38;2;255;255;255m"
#define NARTIC "\033[38;2;156;230;255m"
#define NCYAN "\033[38;2;0;255;255m"
#define RED "\033[38;2;255;0;0m"
#define UNDERLINE "\033[4m"
#define RESET "\033[0m"

#define SCROLL_OFF 0
#define SCROLL_UP 1
#define SCROLL_DOWN 2
#define MAX_PACKAGES 1024

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

    usleep(200000);  // 200ms

    struct input_event ev;
    int wheel_value = (direction == SCROLL_UP) ? 18000 : -18000;
    int hi_res_value = (direction == SCROLL_UP) ? 260000 : -260000;

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

    usleep(100000);  // 200ms

    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

void print_help(const char *progname) {
    printf("\n%s\n", SEPARATOR);
    printf(UNDERLINE NARTIC "Nordix package search - Help (Pacman)" RESET "\n\n");
    printf(WHITE "Usage: %s " NARTIC "[options] " NCYAN "<search term>\n\n" RESET, progname);
    printf(NARTIC "Options:\n" RESET);
    printf("  --scroll-0    " WHITE "No auto scroll\n" RESET);
    printf("  --scroll-1    " WHITE "Scroll up after search (default)\n" RESET);
    printf("  --scroll-2    " WHITE "Scroll down after search\n" RESET);
    printf("  --help, -h    " WHITE "Show this help message\n\n" RESET);
    printf("%s\n", SEPARATOR);
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    int scroll_mode = SCROLL_UP;
    char *search_terms[256];
    int search_count = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "--scroll-0") == 0) {
            scroll_mode = SCROLL_OFF;
        }
        else if (strcmp(argv[i], "--scroll-1") == 0) {
            scroll_mode = SCROLL_UP;
        }
        else if (strcmp(argv[i], "--scroll-2") == 0) {
            scroll_mode = SCROLL_DOWN;
        }
        else {
            search_terms[search_count++] = argv[i];
        }
    }

    if (search_count == 0) {
        fprintf(stderr, RED "Error: " WHITE "No search term provided\n" RESET);
        print_help(argv[0]);
        return 1;
    }

    // Build the pacman command
    char cmd[1024] = "pacman -Ss --color=always ";
    for (int i = 0; i < search_count; i++) {
        strcat(cmd, search_terms[i]);
        if (i < search_count - 1) strcat(cmd, " ");
    }

    char tmpfile[256];
    snprintf(tmpfile, sizeof(tmpfile), "/tmp/pacsearch_%d.txt", getpid());

    char full_cmd[2048];
    snprintf(full_cmd, sizeof(full_cmd), "PAGER=cat %s > %s 2>&1", cmd, tmpfile);
    system(full_cmd);

    FILE *fp = fopen(tmpfile, "r");
    if (!fp) {
        perror("fopen failed");
        return 1;
    }

    char line[4096];
    char **pkg_names = malloc(MAX_PACKAGES * sizeof(char*));
    if (!pkg_names) {
        perror("malloc failed");
        fclose(fp);
        return 1;
    }
    memset(pkg_names, 0, MAX_PACKAGES * sizeof(char*));

    int pkg_count = 0;
    int installed_pkgs = 0;

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

                char *slash = strchr(pkg_full, '/');
                if (slash) {
                    pkg_names[pkg_count] = strdup(slash + 1);
                } else {
                    pkg_names[pkg_count] = strdup(pkg_full);
                }

                if (strstr(clean_line, "[installed")) {
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

    // Download sizes
    char **sizes = malloc(MAX_PACKAGES * sizeof(char*));
    if (!sizes) {
        perror("malloc failed");
        for (int i = 0; i < pkg_count; i++) {
            if (pkg_names[i]) free(pkg_names[i]);
        }
        free(pkg_names);
        return 1;
    }
    memset(sizes, 0, MAX_PACKAGES * sizeof(char*));

    char size_file[256];
    snprintf(size_file, sizeof(size_file), "/tmp/pacsearch_sizes_%d.txt", getpid());

    int batch_size = 50;
    for (int batch_start = 0; batch_start < pkg_count; batch_start += batch_size) {
        int batch_end = batch_start + batch_size;
        if (batch_end > pkg_count) batch_end = pkg_count;

        size_t cmd_size = 256 + (batch_end - batch_start) * 128;
        char *size_cmd = malloc(cmd_size);
        if (!size_cmd) continue;

        strcpy(size_cmd, "expac -H M -S '%n %m' ");
        for (int i = batch_start; i < batch_end; i++) {
            if (pkg_names[i]) {
                if (strlen(size_cmd) + strlen(pkg_names[i]) + 2 < cmd_size - 100) {
                    strcat(size_cmd, pkg_names[i]);
                    if (i < batch_end - 1) strcat(size_cmd, " ");
                }
            }
        }

        char *size_full_cmd = malloc(cmd_size + 256);
        if (size_full_cmd) {
            snprintf(size_full_cmd, cmd_size + 256, "%s 2>/dev/null >> %s", size_cmd, size_file);
            system(size_full_cmd);
            free(size_full_cmd);
        }
        free(size_cmd);
    }

    FILE *sfp = fopen(size_file, "r");
    if (sfp) {
        char size_line[256];
        while (fgets(size_line, sizeof(size_line), sfp)) {
            size_line[strcspn(size_line, "\n")] = 0;
            char *space = strchr(size_line, ' ');
            if (space) {
                *space = '\0';
                char *pkg = size_line;
                char *size = space + 1;

                for (int i = 0; i < pkg_count; i++) {
                    if (pkg_names[i] && strcmp(pkg_names[i], pkg) == 0) {
                        sizes[i] = strdup(size);
                        break;
                    }
                }
            }
        }
        fclose(sfp);
    }

    // Clear screen
    system("clear");

    // Heading
    printf("%s\n", SEPARATOR_NORDIX);
    printf("❄️ " NARTIC UNDERLINE "Nordix package search" RESET " " YELLOW " Pacman" WHITE ": ");
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

    printf(NVFROST "Found:" WHITE " %d" NVFROST " Installed:" WHITE " %d" NVFROST " Diskspace:" WHITE " %s\n",
           pkg_count, installed_pkgs, diskspace);
    printf("%s\n", SEPARATOR_NORDIX);
    printf("\n");

    // Print package
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
            if (!first) {
                printf("\n%s\n", SEPARATOR);
            } else {
                first = 0;
            }

            fputs(line, stdout);
            pkg_index++;
            in_package = 1;
        } else {
            fputs(line, stdout);

            if (in_package && pkg_index >= 0 && pkg_index < pkg_count) {
                if (sizes[pkg_index]) {
                    printf("  \033[33mInstalled Size:\033[0m %s\n", sizes[pkg_index]);
                }
                in_package = 0;
            }
        }
    }
    fclose(fp);

    printf("\n%s\n", SEPARATOR);
    usleep(20000);
    // SCROLL
    if (scroll_mode != SCROLL_OFF) {
        scroll(scroll_mode);
    }
    usleep(20000);
  // // Clean up
 // for (int i = 0; i < pkg_count; i++) {
// (pkg_names[i]) free(pkg_names[i]);
 // if (sizes[i]) free(sizes[i]);
// }
free(pkg_names);
free(sizes);
remove(tmpfile);
remove(size_file);
 // char rm_cmd[512];
   // snprintf(rm_cmd, sizeof(rm_cmd), "rm -f %s %s", tmpfile, size_file);
 // system(rm_cmd);
    usleep(20000);
    // SCROLL
    if (scroll_mode != SCROLL_OFF) {
        scroll(scroll_mode);
    }
    return 0;
}
