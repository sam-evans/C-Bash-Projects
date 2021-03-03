#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Name: Samuel Evans
//Date: 2/22/2021
//Project: Simple Tech Shell


char str[129];
enum {NOT_FOUND=0,FOUND};
enum {false=0,true};
static char *ptr;
const char *del;
int ReadLine(char *, int , FILE *);

char *mystrtok(char* string,const char *delim)
{
  int j,flag=NOT_FOUND;
  char *p;
  if(string != NULL)
    {
      ptr=string;
      p=string;
    }
  else
    {
      if(*ptr == '\0')
	return NULL;
 
      p=ptr;
    }
 
  while(*ptr != '\0')
    {
      del=delim;
      while(*del != '\0')
        {
	  if(*ptr == *del)
            {
	      if(ptr == p)
                {
		  p++;
		  ptr++;
                }
	      else
                {
		  *ptr='\0';
		  ptr++;
 
		  return p;
                }
            }
	  else
            {
	      del++;
            }
        }
      ptr++;
    }
  return p;
}
 
int main()
{
  int i;
  char *p_str,*token;
  char delim[10];
    delim[0] = ' ';
    delim[1] = '\t';
    delim[2] = '\n';
    delim[3] = '\0';
  char cwd[1024];
  char *tok[129];
 
  while(1)
  {
    tok[0] = NULL;
    fflush(stdin);
    fflush(stdout);
    getcwd(cwd, sizeof(cwd));
    printf("\n~%s$ ", cwd);
    fflush(stdin);
    ReadLine(str, 128, stdin);
 
    for (i = 1, p_str = str; ; i++, p_str = NULL)
    {
      token = mystrtok(p_str,delim);
      tok[i-1] = token;
      if (token == NULL)
      
	    break;
      
      
    }
    if(tok[0] != NULL)
    {
      if(strcmp(tok[0],"cd") == 0)
      {
        if (chdir(tok[1]) != 0)
          perror("\nchdir() error()");
        getcwd(cwd, sizeof(cwd));
        printf("\nCurrent working directory is: %s\n", cwd);
      }

      else if(strcmp(tok[0],"pwd") == 0)
        if (getcwd(cwd, sizeof(cwd)) == NULL)
          perror("\ngetcwd() error");
        else
          printf("\nCurrent working directory is: %s\n", cwd);

      else if(strcmp(tok[0],"exit") == 0){
        exit(3);
        return(0);
      }
      else{
        pid_t pid = fork();
        if(pid == 0){
          //child process

          int i, in = 0, out = 0;
          char input[64], output[64];

          //handles redirection
          //got help from https://stackoverflow.com/questions/11515399/implementing-shell-in-c-and-need-help-handling-input-output-redirection

          // finds where '<' or '>' occurs and make that tok[i] = NULL , to ensure that command wont't read that
          for(i = 0; tok[i] != '\0'; i++){
            if(strcmp(tok[i], "<") == 0){

              tok[i] = NULL;
              strcpy(input, tok[i+1]);
              in = 2;

              break;
            }
            if(strcmp(tok[i], ">")==0){

              tok[i] = NULL;
              strcpy(output, tok[i+1]);
              out=2;

              break;
            }
          }
         
          if(in == 2){

            stdin = freopen(input, "r", stdin);

            if(stdin < 0){
              perror("\nFile could not be accessed");
            }

            execvp(tok[0], tok);
            fclose(stdin); 
          }
          else if(out == 2){
            stdout = freopen(output, "w", stdout);
            if(stdout < 0){
              perror("\nFile could not be accessed");
            }
            execvp(tok[0], tok);
            fclose(stdout);
            
          }
          else{
            execvp(tok[0], tok);
            
          }
        }
        if( pid > 0){
          wait(NULL);
        }
        if( pid == -1){
          perror("Fork Failed");
        }
        
      }
        
    }
  }
}


int ReadLine(char *buff, int size, FILE *fp)
{
  buff[0] = '\0';
  buff[size - 1] = '\0';             /* mark end of buffer */
  char *tmp;

  if (fgets(buff, size, fp) == NULL) {
    *buff = '\0';                   /* EOF */
    return false;
  }
  else {
    /* remove newline */
    if ((tmp = strrchr(buff, '\n')) != NULL) {
      *tmp = '\0';
    }
  }
  return true;
}