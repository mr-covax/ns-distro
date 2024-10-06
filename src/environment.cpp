#include <iostream>
#include <cstring>
#include <filesystem>

#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/syscall.h>

#include "environment.h"

Environment::Environment(std::string rootFsPath) {
    // Before saving the path inside the object, it must exist
    // and it must be a directory
    if (std::filesystem::exists(rootFsPath) == false) {
        std::cout << "Error: specified rootfs does not exist" << std::endl;
        exit(1);
    }
    if (std::filesystem::is_directory(rootFsPath) == false) {
        std::cout << "Error: specified rootfs is not a directory" << std::endl;
        exit(1);
    }
    rootFs = rootFsPath;
}

bool Environment::prepareMountNamespace() {
    // Creating the mount namespace
    if (unshare(CLONE_NEWNS) == -1) {
        perror("unshare");
        return false;
    }

    // Set the mount propagation inside the entire namespace to private
    if (mount(NULL, "/", NULL, MS_PRIVATE | MS_REC, NULL) == -1) {
        perror("mount");
        return false;
    }

    return true;
}

bool Environment::preparePidNamespace() {
    // Creating the PID namespace
    if (unshare(CLONE_NEWPID) == -1) {
        perror("unshare");
        return false;
    }
    return true;
}

int Environment::enter(std::function<int()> setupFunc) {
    switch (fork()) {
    case 0:
        if (switchRoot() == false) {
            return 3;
        }
        setupFunc();
    
    case -1:
        perror("fork");
        return 2;
    
    default:
        wait(NULL);
    }
    return 0;
}

bool Environment::switchRoot() {
    // Because the current root cannot be on the rootfs
    // (consult fs/namespace.c), a bind mount of the folder
    // is needed.
    if (mount(rootFs.c_str(), rootFs.c_str(), NULL, MS_BIND, NULL) == -1) {
        perror("mount");
        return false;
    }

    // A trick from LXC developers: there is no need to store
    // the old root in a separate directory, we can just tell
    // pivot_root to put it underneath the new root.        
    if (syscall(SYS_pivot_root, rootFs.c_str(), rootFs.c_str()) == -1) {
        perror("pivot_root");
        return false;
    }

    // To make programs happy, mounting pseudo-filesystems.
    if (mount("proc", "/proc", "proc", 0, NULL) == -1) {
        perror("mount");
        return false;
    }

    // And switch to the root to fully enter the new namespace.
    chdir("/");
    
    return true;
}