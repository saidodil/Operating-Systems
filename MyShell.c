/**
 * @file MyShell.c
 * @author Juhua Hu
 * @author Dilnoza Saidova
 * @version February 10, 2023
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF 255
#define MAX 100

/**
  @brief Fork a child to execute the command using execvp. The parent should wait for the child to terminate
  @param args Null terminated list of arguments (including program).
  @return returns 1, to continue execution and 0 to terminate the MyShell prompt.
 */
int execute(char **args)
{
  int result = 1;
  // if (args[0] == NULL)
  //   result = 1;
  if (strcmp(args[0], "exit") == 0)
  {
    printf("exiting");
    result = 0;
  }
  else
  {
    int rc = fork();
    if (rc > 0)
    {
      wait(NULL);
    }
    else if (rc == 0)
    {
      execvp(args[0], args);
      printf("error executing command: No such file or directory \n");
      exit(0);
    }
    else
    {
      printf("fork failed \n");
      exit(0);
    }
  }
  free(args);
  return result;
}

/**
  @brief gets the input from the prompt and splits it into tokens. Prepares the arguments for execvp
  @return returns char** args to be used by execvp
 */
char **parse(void)
{
  char **args = (char **)malloc(MAX * sizeof(char));
  char *token;
  char input[BUF];
  if (args != NULL)
  {
    for (int i = 0; i < MAX; i++)
    {
      args[i] = (char *)malloc(BUF * sizeof(char));
    }
  }
  printf("MyShell>");
  fgets(input, BUF, stdin);
  if (input[0] == '\n')
  {
    strcpy(args[0], "");
  }
  else
  {
    int cnt = 1;
    strcpy(args[0], strtok(input, " \n"));
    while ((token = strtok(NULL, " \n")) != NULL)
    {
      strcpy(args[cnt], token);
      cnt++;
    }
    if (cnt > 0)
    {
      args[cnt] = "\0";
    }
  }
  return args;
}

/**
   @brief Main function should run infinitely until terminated manually using CTRL+C or typing in the exit command
   It should call the parse() and execute() functions
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  char **args;
  do
  {
    args = parse();
  } while (execute(args));
  return EXIT_SUCCESS;
}