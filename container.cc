#include <iostream>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

// Allocate memory on stack
char* stack_memory() {
  const int stackSize = 65536;
  auto *stack = new (std::nothrow) char[stackSize];

  if (stack == nullptr) { 
    printf("Cannot allocate memory\n");
    exit(EXIT_FAILURE);
  }  

  return stack + stackSize;
}

// The exec() family of functions replaces the current process image with a new 
// process image. The execv() function provide an array of pointers to 
// null-terminated strings that represent the argument list available to the 
// new program. The first argument, by convention, should point to the filename 
// associated with the file being executed. 
int run(const char *name) {
  char *_args[] = {(char *)name, (char *)0 };
  execvp(name, _args);
}

// Child process.
int jail(void *args) {

    // Remove all environment variables for this process.
    clearenv();

    // Load the shell process.
    run("/bin/sh");

    return EXIT_SUCCESS;
}

int main() {
    printf("Hello, world! (parent)\n");

    // clone() creates a new process, in a manner similar to fork(2).
    // Unlike fork(2), clone() allows the child process to share parts of
    // its execution context with the calling process, such as the virtual
    // address space, the table of file descriptors, and the table of signal
    // handlers.
    clone(jail, stack_memory(), SIGCHLD, 0);

    // The wait() system call suspends execution of the calling thread until
    // one of its children terminates.
    wait(nullptr);

    return EXIT_SUCCESS;
}
