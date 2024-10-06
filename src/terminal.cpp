#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <pty.h>

#include "terminal.h"

PTY::PTY() {
    masterFd = posix_openpt(O_RDWR | O_NOCTTY);
    if (masterFd == -1) {
        perror("Failed to create the master");
        exit(2);
    }

    if (grantpt(masterFd) == -1) {
        perror("Failed to grant permissions to PTY");
        exit(2);
    }

    if (unlockpt(masterFd) == -1) {
        perror("Failed to unlock PTY");
        exit(2);
    }
    
    char buffer[64];
    if (ptsname_r(masterFd, buffer, sizeof(buffer)) != 0) {
        perror("Failed to create the slave part of PTY");
        close(masterFd);
        exit(2);
    }

    slaveFd = open(buffer, O_RDWR | O_NOCTTY);
    if (slaveFd == -1) {
        perror("Failed to open slave");
        exit(2);
    }
    slave = buffer;
}

bool PTY::attachMaster() {
    for (int i : {0, 1, 2}) {
        if (dup2(masterFd, i) == -1) {
            perror("dup2");
            return false;
        }
    }
    return true;   
}

bool PTY::attachSlave() {
    for (int i : {0, 1, 2}) {
        if (dup2(slaveFd, i) == -1) {
            perror("dup2");
            return false;
        }
    }
    return true;
}

PTY::~PTY() {
    if (masterFd != -1) {
        close(masterFd);
    }
    if (slaveFd != -1) {
        close(slaveFd);
    }
}