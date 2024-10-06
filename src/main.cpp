#include <iostream>
#include <unistd.h>

#include "environment.h"

int startShell() {
    char *cmd = "/bin/sh";
    char *initArgs[] = {cmd, NULL};
    char *initEnv[] = {NULL};
    return execve(cmd, initArgs, initEnv);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout 
        	<< "Usage: " << argv[0] << " [rootfs]\n" 
        	<< "Upon the switch, /bin/sh in the new root is executed."
        	<< std::endl;
        return 1;
    }
    
    Environment env{argv[1]};
    if (env.prepareMountNamespace() == false)
        return 2;
    if (env.preparePidNamespace() == false)
        return 2;
    
    return env.enter(startShell);
}
