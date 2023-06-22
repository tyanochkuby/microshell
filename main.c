#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define max_n 2048
#define RESET   "\033[0m"
#define BLUE   "\033[36;40m"
#define GREEN   "\033[32;40m"
#define DOLLAR  " $ "

int spaces(char* str);

void splitstring (char* args[], char* str, char splitter)

void strcop(char* str1, char* str2)

void cdtilda(char *str);

void cd(char* args)

void rem(char* file)

void help()

char *getlogin();

void ls(char* args);

char *words[] = {"cd", "ls", "help", "exit", "rm"};

void sw(char *program, char *args);




int main(int argc, char *argv[])
{
    char command[max_n], program[max_n], arguments[max_n], workdir[max_n], out[max_n];

    int i = 0, size, space_pos;


    while(1)
    {
        getcwd(workdir, max_n);
        for(i = 0; i < max_n; i++)
        {
            program[i] = '\0';
            arguments[i] = '\0';
            out[i] = '\0';
        }

        strcop(out, GREEN);
        strcop(out, workdir);
        strcop(out, RESET);
        strcop(out, DOLLAR);

        strcpy(command, readline(out));
        add_history(command);

        command[strcspn(command, "\n")] = 0;
        size = strlen(command);
            if(strchr(command, ' ') != 0)
            {
                char *prc = strchr(command, ' ');
                space_pos = prc - command;


                for(i = 0; i < space_pos; i++)
                {
                    program[i] = command[i];
                }
                for(i = space_pos+1; i < size; i++)
                {
                    arguments[i-space_pos-1] = command[i];
                }
                sw(program, arguments);
            }
            else if (strlen(command))
                sw(command, NULL);
    }




    return 0;
}

void sw(char *program, char *args) {
    int i;
    for (i = 0; i < sizeof words/sizeof words[0]; i++)
    {
        if (!strcmp(program, words[i]))
        {
            switch(i)
            {
                case 0:
                    if(args == NULL)
                    {
                        cd(NULL);
                        break;
                    }
                    cd(args);
                    break;
                case 1:
                    ls(args);
                    break;
                case 2:
                    help();
                    break;
                case 3:
                    exit(0);
                    break;
                case 4:
                    rem(args);

            }
            return ;
        }
    }

    pid_t id = fork();
    if (id == 0)
    {
        if(args == NULL){
            char* forexec[2] = {program, NULL};
            if(execvp(program, forexec) == -1)
                printf("There is no such program sorry :/ \n");
        }
        else
        {
            char* forexec[spaces(args) + 3];
            splitstring(forexec, args, ' ');
            forexec[spaces(args) + 2] = NULL;
            forexec[0] = program;
            if(execvp(program, forexec) == -1)
                printf("There is no such program sorry :/ \n");
        }

    }
    else
        wait(NULL);
}

int spaces(char *str)
{
    int count = 0, i;
    for(i = 0; i < strlen(str); i++)
        if(str[i] == ' ')
            count++;
    return count;
}

void splitstring (char* args[], char* str, char splitter)
{
    char *token;
    args[spaces(str)+1] = NULL;
    int i = 1;
    token = strtok(str, " ");
    args[1] = token;
    while ((token = strtok(NULL, " ")) != NULL)
        args[++i] = token;
}

void strcop(char* str1, char* str2)
{
    int i, size1 = strlen(str1), size2 = strlen(str2);
    for(i = size1; i < size1 + size2; i++)
        str1[i] = str2[i - size1];
}

void cdtilda(char *str)
{

    if(str[0] == '~')
    {
        int i;
        /*
        char usr[256];
        getlogin_r(usr, 256);
        */
        char a[max_n];
        for(i = 0; i < max_n; i++)
            a[i] = '\0';
        strcop(a, "/home/");
        strcop(a, getlogin());
        int outsize = strlen(a), fromsize = strlen(str);
        for(i = outsize; i < fromsize + outsize - 1; i++)
        {
            a[i] = str[i - outsize + 1];
        }
        strcpy(str, a);
        str[strlen(str)] = '\0';
    }
}

void cd(char* args)
{
    if(args == NULL)
    {
        int i;
        char args1[max_n];
        for(i = 0; i < max_n; i++)
            args1[i] = '\0';
        args1[0] = '~';
        cdtilda(args1);
        if(chdir(args1) == -1)
            printf("cd: There is no such directory\n");
        for(i = 0; i < max_n; i++)
            args1[i] = '\0';
        return ;
    }
    cdtilda(args);
    if(chdir(args) == -1)
        printf("cd: There is no such directory\n");
}

void ls(char* args)
{
    DIR* dir;
    if(args != NULL)
    {
        cdtilda(args);
        dir = opendir(args);
        if(dir == 0)
        {
            printf("%s: there is no such directory\n", args);
            return ;
        }
    }
    else
        dir = opendir(".");
    struct dirent* entity = readdir(dir);
    char buf[max_n][max_n];
    int i = 0;
    while (entity != NULL)
    {
        if(entity->d_type == 4)
        {
            if(strcmp(entity->d_name, ".") && strcmp(entity->d_name, ".."))
                printf("%s%s%s\n", BLUE, entity->d_name, RESET);
        }

        else
        {
            strcpy(buf[i], entity->d_name);
            i++;
        }

        entity = readdir(dir);
    }
    int k;
    printf("%s", RESET);
    for(k = 0; k < i; k++)
        printf("%s\n",buf[k]);
    closedir(dir);
}

void rem(char* file)
{
    if (-1 == remove (file))
        printf ("Error\n");
    else
        printf ("Done\n");
}

void help()
{
    printf("Microshell by Roman Judeszko \nAvailable commands: \nls \nhelp \ncd \nrm \nexit \n");
}