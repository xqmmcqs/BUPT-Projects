#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#ifndef MAXNAMLEN
#define MAXNAMLEN 255
#endif // MAXNAMLEN

char *opt_arg;
int opt_ind = 1;

struct Flags
{
    bool recursive;
    bool all;
    long low;
    long high;
    double modified;
} flags;

char pwd[MAXNAMLEN];

int getOpt(int argc, char *argv[]);

bool match(const char *pattern);

void output(const char *path);

void help_info();

int main(int argc, char *argv[])
{
    int path_args_cnt = 0;
    char **path_args_list = (char **)malloc(argc * sizeof(char *));

    flags.high = flags.modified = 0x7fffffff;
    int ch;
    while ((ch = getOpt(argc, argv)) != -1)
    {
        switch (ch)
        {
        case 0:
            path_args_list[++path_args_cnt] = opt_arg;
            break;
        case 'r':
            flags.recursive = true;
            break;
        case 'a':
            flags.all = true;
            break;
        case 'l':
            flags.low = atoll(opt_arg);
            if (strcmp(opt_arg, "0") && !flags.low)
                help_info();
            break;
        case 'h':
            flags.high = atoll(opt_arg);
            if (strcmp(opt_arg, "0") && !flags.high)
                help_info();
            break;
        case 'm':
            flags.modified = atoll(opt_arg) * 3600 * 24;
            if (strcmp(opt_arg, "0") && !flags.modified)
                help_info();
            break;
        case '?':
            help_info();
            break;
        }
    }

    if (getcwd(pwd, MAXNAMLEN) == NULL)
    {
        fprintf(stderr, "getcwd(): Error.");
        free(path_args_list);
        exit(1);
    }

    if (!path_args_cnt) // no path args, list pwd
    {
        match(pwd);
        free(path_args_list);
        return 0;
    }

    for (int i = 1; i <= path_args_cnt; ++i)
    {
        char *pattern = (char *)malloc((strlen(path_args_list[i]) + MAXNAMLEN + 1) * sizeof(char));
        if (!strlen(path_args_list[i]) || (path_args_list[i][0] == '.' && path_args_list[i][1] != '.'))
        {
            strcpy(pattern, pwd);
            strcpy(pattern + strlen(pwd), path_args_list[i] + 1);
        }
        else if ((path_args_list[i][0] == '.' && path_args_list[i][1] == '.' && path_args_list[i][2] == '/') || strrchr(path_args_list[i], '/') == NULL)
        {
            strcpy(pattern, pwd);
            pattern[strlen(pwd)] = '/';
            strcpy(pattern + strlen(pwd) + 1, path_args_list[i]);
        }
        else
            strcpy(pattern, path_args_list[i]);
        // printf("%s\n", pattern);

        if (!match(pattern))
            fprintf(stderr, "list: cannot access '%s': No such file or directory\n", path_args_list[i]);
        free(pattern);
    }
    free(path_args_list);
    return 0;
}

int getOpt(int argc, char *argv[])
{
    if (opt_ind >= argc)
        return -1;
    if (argv[opt_ind][0] != '-')
    {
        opt_arg = argv[opt_ind];
        opt_ind++;
        return 0;
    }
    if (argv[opt_ind][1] == '-')
    {
        opt_ind++;
        return -1;
    }
    else if (argv[opt_ind][1] == 'r' || argv[opt_ind][1] == 'a')
    {
        opt_arg = NULL;
        char opt = argv[opt_ind][1];
        opt_ind++;
        return opt;
    }
    else if (argv[opt_ind][1] == 'l' || argv[opt_ind][1] == 'h' || argv[opt_ind][1] == 'm')
    {
        if (opt_ind + 1 >= argc)
        {
            opt_arg = NULL;
            return '?';
        }
        else
            opt_arg = argv[opt_ind + 1];
        char opt = argv[opt_ind][1];
        opt_ind += 2;
        return opt;
    }
    else
    {
        opt_ind++;
        return '?';
    }
}

bool match(const char *pattern)
{
    char *last_slash = strrchr(pattern, '/');
    char *last_pattern = (char *)malloc(strlen(pattern) * sizeof(char));
    char *file_name = (char *)malloc(MAXNAMLEN * sizeof(char));
    strncpy(last_pattern, pattern, last_slash - pattern);
    strcpy(file_name, last_slash + 1);

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(last_pattern)) == NULL)
    {
        free(last_pattern);
        free(file_name);
        return false;
    }

    if (!strlen(file_name)) // pattern end with '/'
    {
        *last_slash = 0;
        last_slash = strrchr(last_pattern, '/');
        *last_slash = 0;
        strcpy(file_name, last_slash + 1);
        closedir(dir);
        dir = opendir(last_pattern);
    }
    // printf("%s %s\n", last_pattern, file_name);

    while ((ent = readdir(dir)) != NULL)
    {
        if (!strcmp(file_name, ent->d_name))
        {
            output(pattern);
            return true;
        }
    }
    closedir(dir);
    free(last_pattern);
    free(file_name);
    return false;
}

void output(const char *path)
{
    struct stat statbuf;
    stat(path, &statbuf);
    if (S_ISDIR(statbuf.st_mode))
    {
        DIR *dir;
        struct dirent *ent;
        struct stat statbuf;

        if ((dir = opendir(path)) == NULL)
        {
            fprintf(stderr, "Can`t open directory %s\n", path);
            return;
        }

        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                if (!flags.all)
                    continue;
            char *new_path = (char *)malloc((strlen(path) + MAXNAMLEN + 1) * sizeof(char));
            strcpy(new_path, path);
            if (!strcmp(path, "/"))
                strcpy(new_path + 1, ent->d_name);
            else
            {
                strcpy(new_path + strlen(path) + 1, ent->d_name);
                *(new_path + strlen(path)) = '/';
            }
            stat(new_path, &statbuf);
            if (statbuf.st_size >= flags.low && statbuf.st_size <= flags.high && difftime(time(NULL), statbuf.st_mtime) <= flags.modified)
                printf("%10ld %s\n", statbuf.st_size, new_path);
            if (!flags.recursive)
            {
                free(new_path);
                continue;
            }
            if (S_ISDIR(statbuf.st_mode))
            {
                if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
                    continue;
                output(new_path);
            }
            free(new_path);
        }
    }
    else
    {
        stat(path, &statbuf);
        if (statbuf.st_size >= flags.low && statbuf.st_size <= flags.high && difftime(time(NULL), statbuf.st_mtime) <= flags.modified)
            printf("%10ld %s\n", statbuf.st_size, path);
    }
}

void help_info()
{
    printf("usage: list [-r] [-a] [-l <minimum_size>] [-h <maximum_size>] [-m <modified_days>] [file ...]\n");
    exit(1);
}
