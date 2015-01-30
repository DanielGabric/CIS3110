#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

#define INITIAL 100
#define ARGUMENT_SIZE 100


typedef struct 
{
    char ** arguments;
    int size;
    int isAmp;
    char * input_file;
    char * output_file;
    int isInput;
    int isOutput;
    int isPipe;
    char ** pipeArgs;
}Input; 

void piping(Input * in);
void hasPipe(Input * in);
void parse(Input * og,char *string);
void execute(Input * in);
void listA(Input * list);
void add(Input * list);
void multiply(Input * list);
void redIn(Input * in);
void redOut(Input * in);
int strToInt(char * str);
void isAmper(Input * in);
void sig_handler(int signal);

int main()
{
    char *in=malloc(sizeof(char)*250);
    Input * input = malloc(sizeof(Input));
    input->size = 0;
    input->isAmp = 0;
    //sigset(SIGCHLD,sig_handler);

    do
    {
        printf("Daniel's Shell>");
        fgets(in,240,stdin);
        in[strlen(in)-1]='\0';
        parse(input,in);
        if(input->arguments[0]==NULL)
            continue;
        isAmper(input);
        redIn(input);
        redOut(input);
        hasPipe(input);
        if(input->isPipe)
        {
            piping(input);
            continue;
        }
        

        if(!strcmp(input->arguments[0],"exit"))
        {
            exit(0);
            break;
        }
        else if(!strcmp(input->arguments[0],"add"))
        {
            add(input);
            continue;
        
        }
        else if(!strcmp(input->arguments[0],"arg")||!strcmp(input->arguments[0],"args"))
        {
            
            listA(input);
            continue;
        
        }
        else if(!strcmp(input->arguments[0],"muls"))
        {
            multiply(input);
            continue;
        
        }
        execute(input);
    
    } while (1);
    free(in);
    free(input);
    return (0);
}

void sig_handler(int signal)
{
  int status;
  wait(&status);
}

/*Redirects input for a command*/
void redIn(Input * in)
{
    int i;
    for(i=1;i<in->size;++i)
    {
        if(in->arguments[i]==NULL)
        {
            break;
        }
        if(!strcmp(in->arguments[i],"<"))
        {
            if(in->arguments[i+1]!=NULL)
            {
                in->input_file = in->arguments[i+1];
                in->isInput = 1;
                printf("Redirecting input to %s\n",in->input_file);
            }
            int j;
            /*Deleting all arguments from < and on*/
            for(j=i;j<in->size;++j)
            {
                in->arguments[j]='\0';
            }
            return;
        }
    }
    in->isInput = 0;

}

/*Redirects output for a command*/
void redOut(Input * in)
{
    int i;
    for(i=0;i<in->size;++i)
    {
        if(in->arguments[i]==NULL)
        {
            break;
        }
        if(!strcmp(in->arguments[i],">"))
        {
            if(in->arguments[i+1]!=NULL)
            {
                in->output_file = in->arguments[i+1];
                in->isOutput = 1;
                printf("Redirecting output to %s\n",in->input_file);
            }
            int j;
            /*Deleting all arguments from the > and on*/
            for(j=i;j<in->size;++j)
            {
                in->arguments[j]='\0';
            }
            return;
        }
    }
    in->isOutput = 0;
}

/*Checks if argument is &, if so set isAmp to true and remove the ampersand*/
void isAmper(Input * in)
{
    if(!strcmp(in->arguments[in->size-1],"&"))
    {
        in->arguments[in->size-1]='\0';
        in->size--;
        in->isAmp = 1;
    }
    else
    {
        in->isAmp = 0;
    }
}

void hasPipe(Input * in)
{

    int i;
    for(i=0;i<in->size;++i)
    {
        if(in->arguments[i]==NULL)
        {
            break;
        }
        if(!strcmp(in->arguments[i],"|"))
        {

            if(in->arguments[i+1]!=NULL)
            {
                in->pipeArgs = malloc(sizeof(char*)*in->size);
                int j;
                for(j=i+1;j<in->size;++j)
                {
                    in->pipeArgs[j-i-1]=in->arguments[j];
                }

                in->isPipe = 1;
            }
            int j;
            /*Deleting all arguments from the > and on*/
            for(j=i;j<in->size;++j)
            {
                in->arguments[j]='\0';
            }
            return;
        }
    }
    in->isPipe = 0;
}


void piping(Input * in)
{
    
    int status;
    int fd[2];
    int f = pipe(fd);
    if(f==-1)
    {
        printf("Error in piping!\n");
        return;
    }
    int pid = fork();
    if(!pid)
    {
        close(0);
        dup(fd[0]);
        close(fd[1]);
        execvp(in->pipeArgs[0],in->pipeArgs);
        exit(0);
        
    }
    else
    {
        
        int pid1 = fork();
        if(!pid1)
        {
            close(1);
            dup(fd[1]);
            close(fd[0]);
            execvp(in->arguments[0],in->arguments);
            exit(0);
        }
        else
        {
            wait(&status);
        }
    }

}

/*Creates new child processes*/
void execute(Input* in)
{
    int status;
    /*if(in->isAmp)
    {
        sigset(SIGCHLD,sig_handler);
    }*/
    int childPid = fork();
    if(childPid == 0)
    {
        if(in->isInput)
        {
            freopen(in->input_file,"r",stdin);
        }
        if(in->isOutput)
        {
            freopen(in->output_file,"w+",stdout);
        }
        execvp(in->arguments[0],in->arguments);
        exit(-1);
    }
    else
    {
        if(!in->isAmp)
        {
            wait(&status);
        }
        else
        {
            sigset(SIGCHLD, sig_handler);
        }
    }

    

}

/*Parses a command line string, into an array of strings of arguments*/
void  parse(Input * og,char *string)
{
    og->size = 0;
    
    og->arguments = malloc(sizeof(char*)*INITIAL);
    int n = INITIAL;
    char * token = strtok(&string[0]," ");

    while(token != NULL)
    {
        /*If counter for number of arguments is more than max space, realloc*/
        if(og->size>=INITIAL)
        {
            printf("Error, memory full!");
            exit(0);
        }
        int size = strlen(token);
        if(size >= ARGUMENT_SIZE)
        {
            printf("Error, memory full!");
            exit(0);
        }
        og->arguments[og->size]=malloc(sizeof(char)*(size+1));
        strncpy(og->arguments[og->size],token,size);
        og->arguments[og->size++][size]='\0';
        token = strtok(NULL," ");
    }
    if(og->size >= INITIAL)
    {
        printf("Error, memory full!");
        exit(0);
    }
    og->arguments[og->size]='\0';
}


void multiply(Input * list)
{
    int i;
    int tot = 1;
    for(i=1;i<list->size;++i)
    {
        int n  = (int)strtol(list->arguments[i],NULL,0);
        if(!n)
        {
            printf("Error, not all of your arguments are integers!\n");
            return;
        }
        tot*=n;
    }
    printf("%d\n",tot);
}

void listA (Input * list)
{
    int i;
    printf("argc = %d  arguments =",list->size-1);
    for(i=1;i<list->size;++i)
    {
        printf(" %s,",list->arguments[i]);
    }
    printf("\n");
}

void add(Input * list)
{
    int i;
    
    int tot = 0;
    for(i=1;i<list->size;++i)
    {
        int n = (int)strtol(list->arguments[i],NULL,0);
        if(!n)
        {
            printf("Error, not all of your arguments are integers!\n");
            return;
        }
        tot+=n;
    }
    printf("%d\n",tot);
}
