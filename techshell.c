#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void parseCmd(char* cmd);
int executeCmd(char** params);

char * cmd = NULL;
char *args = NULL;
char *ifile = NULL;
char *ofile = NULL;

void pwd(){
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s$ ", cwd);
}

// Split command into array of parameters
int parse(char* input)
{   int total = 0;
    char *token; //pointer to each command
    token = strtok(input, " ");
    //First thing is always the command...(cd, pwd, ls ...)
    
    while(token != NULL){
        if(cmd == NULL){
            cmd = strdup(token);
            token = strtok(NULL, " ");
        }
        if(strcmp(token, "<") == 0){
            //get the next token as infile.
            token = strtok(NULL, " ");
            ifile = strdup(token);
        }else if (strcmp(token, ">") == 0){
            //get the next token as outfile.
            token = strtok(NULL, " ");
            ofile = strdup(token);
        }else{
            
            total ++;
            if(args == NULL){
                args = strdup(token);
            }else{
            // resize the arguments string and concatenate the new one
            args = realloc(args, strlen(args) + 1 + strlen(token) + 1);
            strcat(args, " ");
            strcat(args, token);
        
            }
            
        }
        token = strtok(NULL, " ");
        //printf("Next token: %s\n", token);
        
    }
    printf("cmd = %s, args = %s, total = %d, infile = %s, outfile = %s\n", cmd, args, total, ifile, ofile);
    
    return total;
}

//change directory command
void CD(char destination[]){
	if(chdir(destination)==-1){
		char* error = strerror(errno);
		printf("%s\n", error);
	}
}

int execute(char** params)
{
    // Fork process
    pid_t pid = fork();

    // Error
    if (pid == -1) {
        char* error = strerror(errno);
        printf("%s\n", error);
        return 1;
    }

    // Child process
    else if (pid == 0) {
        // Execute command
        execvp(params[0], params);  

        // Error occurred
        char* error = strerror(errno);
        printf("exec error shell: %s: %s\n", params[0], error);
        return 0;
    }else {
        // Wait for child process to finish
        int childStatus;
        waitpid(pid, &childStatus, 0);
        exit(0);
    }
}

int main()
{
    char command[100];
    char* params[10];
      
    while(1) {
        cmd = NULL;
        args = NULL;
        ifile = NULL;
        ofile = NULL;
        //start by printing directory
        pwd();
        
        
        int limit=0;
        int exe=1;
        
        
        
        // Read command from standard input, exit on Ctrl+D
        if(fgets(command, sizeof(command), stdin) == NULL) exit(1);

        // Remove trailing newline character, if any
        if(command[strlen(command)-1] == '\n') {
            command[strlen(command)-1] = '\0';
        }

        // Split cmd into array of parameters
        int ParamSize = parse(command);
        
        //check for commands
        if(strcmp(cmd, "cd") == 0){
            chdir(args);
        }else if(strcmp(cmd, "exit") == 0){
            exit(0);
        }else{
        printf("In Elses //.....\n");
        //Parse args again in put them in  Trueparams
        char* TrueParams[ParamSize+2];
        TrueParams[0] = strdup(cmd);
        char *token = strtok(args, " ");
        int i = 1;
        while(token){
            TrueParams[i] = strdup(token);
            token = strtok(NULL, " ");
            i++;
        }
        TrueParams[ParamSize+1] = NULL;
        //print exevp stuff:
             printf("Args: ");
            for(int i = 0; i < ParamSize + 2; i++)
                printf("%s ", TrueParams[i]);
            printf("\n");
            
            //check for redirections
                   if(ifile != NULL){
                   FILE *infile = fopen(ifile, "r");
                   dup2(fileno(infile), 0);
                   fclose(infile);
                   //break;
                   }
                   if(ofile != NULL){
                   FILE *outfile = fopen(ofile, "w");
                   dup2(fileno(outfile), 1);
                   fclose(outfile);
                   }
            
     }
    }

    return 0;
}


