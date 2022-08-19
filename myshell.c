#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static char* space1 = " \t\n\r\v\f";
static char* space2 = " \t\n";
static int redir = 0;

void myPrint(char *msg)
{
    write(STDOUT_FILENO, msg, strlen(msg));

}

void error() // error message
{
    char error_message[30] = "An error has occurred\n";
    write(STDOUT_FILENO, error_message, strlen(error_message));
    //exit(1);
}

int Fork(void) // fork wrapper including error check
{
    pid_t pid = fork();
    if (pid == -1) {
        error();
    }
    return pid;
}

int emptyCheck(char *t){
  char temp[514];
  char* token;
  strcpy(temp, t);
//   for (int i = 0; i < strlen(t); i++){
//     // if (t[i] == ' ' || t[i] != '\t' || t[i] != '\n' || t[i] != '\0' || t[i] != '\r' || t[i] != '\v' || t[i] != '\f'){
//     //   return 0;
//     // }
//   }
  if((token=strtok(temp, space1)) == NULL){
    return 1;
  }
  else{
      return 0;
  }
}
// int empty(char *cmd){
//   for (int i = 0; i < strlen(cmd); i++){
//     if (cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n' && cmd[i] != '\0' && cmd[i] != '\r' && cmd[i] != '\v' && cmd[i] != '\f'){
//       return 0;
//     }
//   }
//   return 1;
// }

// char* sys_cmd[10];
void trim_newline (char *s) {
    int l = strlen(s);
    if (s[l-1]=='\n') {
        s[l-1]='\0';
    }
}

void built_in_cmd(char *t, char* p) {
        // trim_newline(token_token);
        if (strcmp("exit",t) == 0) {
            if (strtok_r(NULL,space2, &p) !=NULL) {
                    error();
                }
            else {
                exit(0);
            }    
        }
            
        else if (strcmp("pwd",t) == 0) {
            char cwd[512];
           
            getcwd(cwd, 512);
            if (strtok_r(NULL,space2, &p) !=NULL) {
                    error();
                }
            else{
                myPrint(cwd);
                myPrint("\n");
            }
        } 

        else if (strcmp("cd",t) == 0) {
            t = strtok_r(NULL,space2, &p);
            if (t==NULL) {
                char *path = getenv("HOME");
                chdir(path);
            }
            else {
                char cd[512];
                strcpy(cd,t);
                if (strtok_r(NULL,space2, &p) !=NULL) {
                    error();
                }
                else if (chdir(cd)!=0) {
                    error();
                    }
                }
            }
        return;
}

void redir_checker (char* token) {
    if (strstr(token, ">")!=NULL) {
        redir = 1;

    }

    if (strstr(token, ">+")!=NULL) {
        redir = 2;
    }

    else {
        redir = 0;
    }
}

void redirection (char *t, char **sys_cmd) {
    char *token_token3, *token_token4, *p3, *p4;
    token_token3 = strtok_r(t, ">", &p3);


    if (strcmp(token_token3, "cd")==0 || strcmp(token_token3, "exit")==0 || strcmp(token_token3, "pwd")==0) {
        error();
        return;
    }

    token_token4 = strtok_r(t,space1, &p4);

    int i=0;
    while (token_token3!=NULL){
        sys_cmd[i]=token_token3;
        token_token3 = strtok_r(NULL,space1, &p4);
        i++;
    }
    sys_cmd[i]=NULL;

    //myPrint("adnjksadnas\n");

    token_token3=strtok_r(NULL,">",&p3);
    if (strtok_r(NULL,">",&p3)!=NULL||token_token3==NULL) {
        //printf("error1\n");
        error();
        return;
    }

    token_token4 = strtok_r(token_token3,space1, &p4);
    if (strtok_r(NULL,space2,&p4)!=NULL||token_token4==NULL) {
        //printf("error2\n");
        error();
    }
    
    else if (access(token_token4,F_OK) != -1) {
        //printf("toke4:%sd\n", token_token4);
        //printf("error3\n");
        error();
    }

    else {
        //myPrint("adnjksadnas\n");
        int fd = open(token_token4, O_RDWR | O_CREAT | O_TRUNC, 0664);
        if (fd==-1) {
        //printf("error4\n");
        error();}
        else{

        dup2(fd, STDOUT_FILENO);
        execvp(sys_cmd[0],sys_cmd);
        //printf("error5\n");
        error();
        exit(0);
        }
        close(fd);
    }

    
    return;
}

void redirection_advanced(char* t, char**sys_cmd) {
    char *token_token3, *token_token4, *p3, *p4;
    token_token3 = strtok_r(t, ">", &p3);

    //myPrint("asdf\n");
    if (strcmp(token_token3, "cd")==0 || strcmp(token_token3, "exit")==0 || strcmp(token_token3, "pwd")==0) {
        error();
        return;
    }

    if (token_token3[0]=='+'||token_token3==NULL || emptyCheck(token_token3)==1) {
        error();
        return;
    }

    token_token4 = strtok_r(token_token3, space1, &p4);
    int i = 0;
    while (token_token4!=NULL) {
        sys_cmd[i]=token_token4;
        token_token4=strtok_r(NULL,space1,&p4);
        i++;
    }
    sys_cmd[i]=NULL;

    token_token3 = strtok_r(NULL,">",&p3);
    token_token3 += 1;
    if (strtok_r(NULL,">",&p3)!=NULL||token_token3==NULL) {
        error();
        return;
    }

    token_token4 = strtok_r(token_token3,space1,&p4);
    if (strtok_r(NULL,space2,&p4)!=NULL||token_token4==NULL) {
        error();
        return;
    }

    else {
        int fd1 = open(token_token4, O_RDWR | O_CREAT, 0664);
        if (fd1==-1) {
            error();
            return;
        }

        //myPrint("asdf\n");

        int fd2 = open("temp.txt", O_RDWR | O_CREAT, 0664);
        if (fd2==-1) {
            error();
            return;
        }

        int status = 0;
        int count = 0;
        char adv_buff[512];

        int pid = Fork();
        if (pid == 0) {// Child
            //count = read(fd1, adv_buff, 512);
            //printf("count fd1: %d\n",count);
            // myPrint("asdf\n");
            while ((count = read(fd1, adv_buff, 512)) > 0){
                //myPrint("asdf\n");
                write(fd2, adv_buff, count);
            }
            lseek(fd1, 0, SEEK_SET);
            dup2(fd1, STDOUT_FILENO);
            execvp(sys_cmd[0], sys_cmd);
            error();
            exit(0);
        }

        else {
            wait(&status);
        }

        lseek(fd2, 0, SEEK_SET);
        lseek(fd1, 0, SEEK_CUR);
        //myPrint("asdf\n");
        // count = read(fd2, adv_buff, 512);
        //printf("count fd2: %d\n",count);
        while ((count = read(fd2, adv_buff, 512)) > 0) {
            write(fd1,adv_buff,count);
            
            //yPrint("asdf\n");
        }
        close(fd1);
        close(fd2);
        remove("temp.text");
    }
    return;
    }

void parser (char *input, int batchfile) { 
    // parses using semicolon and then whitespace as delimiter
    // calls built-in function and system command handler
    char *p1, *p2;
    char *token;
    char* token_token;
    char redir_token[512];
    char command[512];
    strcpy(command, input);
    int printed = 0;
    char* check_semicolon = strchr(command, ';');
    token = strtok_r(input, ";", &p1);
    while (token !=NULL) {
        // (redir_checker(token));
        
        // if (redir!= 0) {
        //     strcpy(redir_token,token);
        // }
        int empty = emptyCheck(token);
        // for (int i = 0; i < strlen(token); i++){
        //     printf("entered loop token: %s white\n", token);
        
        // }
        if(batchfile && !printed && (!empty || check_semicolon)){
            //printf("printed for batchfile \n");
            myPrint(command);
            printed = 1;
        }
        if (empty == 1){

        }
        else{
            if (strstr(token, ">+") != NULL){
	        redir = 1;
	        strcpy(redir_token, token);
            }
        else if (strchr(token, '>') != NULL){
            //printf("entered regular red \n");
	        redir = 2;
	        strcpy(redir_token, token);
      }

        token_token = strtok_r(token,space1, &p2);
        if (strcmp(token_token, "cd")==0 || strcmp(token_token, "exit")==0 || strcmp(token_token, "pwd")==0) {
            if (redir != 0) {
                error();
            }
            else {
                built_in_cmd(token_token, p2);
            }
        }

        else { // system commands
            char** sys_cmd = malloc(sizeof(char *) & sizeof(input));
            int i = 0;
            while (token_token !=NULL) {
                sys_cmd[i] = malloc(sizeof(char) & sizeof(input));
                sys_cmd[i] = token_token;
                token_token = strtok_r(NULL, space1, &p2);
            i++;
            }
            sys_cmd[i]=NULL;

            int child_pid = Fork();
            int child_status = 0;
            if (child_pid == 0) { // Child
                    if (redir == 2) {
                        //printf("entered child reg");
                        redirection(redir_token, sys_cmd);
                        exit(0);
                    }
                    else if (redir== 1) {
                        //myPrint("asdf");
                        redirection_advanced(redir_token, sys_cmd);
                        exit(0);
                    }
                execvp(sys_cmd[0],sys_cmd);
                error();
                exit(0);
            }
            else { //Parent
                wait(&child_status);
            }
            free(sys_cmd);
        }
        }
        token = strtok_r(NULL, ";", &p1);
        redir = 0;
        //printf("Got next token: %s\n", token);
    }
    return;
}


int main(int argc, char *argv[]) 
{
    if (argc>2) {
        error();
        exit(0);
    }
    int fd3 = 0;
    if (argc>1) {
        if (access(argv[1], F_OK)== -1) {
            error();
            exit(0);
        }
        fd3 = open(argv[1],O_RDONLY,0666);
        if (fd3 == -1) {
            error();
        }
        //printf("read\n");
        dup2(fd3, STDIN_FILENO);
        }
    while (1) {
        if (!fd3) {
            myPrint("myshell> ");
        } 
        char cmd_buff[514];
        char *pinput;
        // int len;

        pinput = fgets(cmd_buff, 514, stdin);
        if (!pinput) {
            exit(0);
        }
        if(pinput[strlen(pinput) - 1] != '\n'){
            myPrint(pinput);
            //printf("entering error loop\n");
            while(pinput[strlen(pinput) - 1] != '\n'){
                pinput = fgets(cmd_buff, 514, stdin);
                myPrint(pinput);
            }
            error();
        }
        else{
            parser(pinput, fd3);
        }
        // char* input[20]; // seperates input based on ; and store into input[]
        // int j;
        // for (j=0; j<20; j++) {
        //     input[j] = "0";
        // }
        // // input = (char*)malloc(20*sizeof(char*));
        // // int j = 0;
        // // while (j<20) {
        // //     input[j] = '0';
        // // }
        // char* token;
        // char* rest = pinput;
        // int i = 0;
        // while ((token = strtok_r(rest, ";", &rest))) {
        //     input[i] = token;
        //     i++;
        // }

        // myPrint(input[0]);
        // myPrint("\n");
        // myPrint(input[1]);
        // myPrint("\n");
        // myPrint(input[2]);
        
        
        }

            // i = 0;
            // while (strcmp("0",input[i]) != 0) { //system commands
            //     myPrint("test1");
            //     if ((strcmp("pwd",input[i]) != 0) && (strcmp("exit",input[i]) != 0) && (strcmp("cd",input[i]) != 0)) {
            //         myPrint("test2");
            //         int k = 0;
            //         while (k<10) {
            //             sys_cmd[k]="";
            //             k++;
            //         }
            //         myPrint("test2.2");
            //         char* sys_token;
            //         char* sys_rest = input[i];
            //         j = 0;
            //         myPrint("test2.5");
            //         while ((sys_token = strtok_r(sys_rest, " ", &sys_rest))) {
            //             sys_cmd[j] = sys_token;
            //             j++;
            //             myPrint(sys_cmd[j]);
                        
            //         }
            //         exec_sys(sys_cmd);
                    
            //     }
            // i++;
            // }
        
        
        // myPrint(input[0]);
        // myPrint(input[1]);
        // myPrint(input[2]);
        // myPrint(input[3]);
    

        // myPrint(cmd_buff);
        return 0;
      
    }
