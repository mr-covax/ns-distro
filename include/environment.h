#pragma once

#include <string>
#include <functional>
#include "terminal.h"

class Environment {
public:
    Environment(std::string rootFsPath);

    bool prepareMountNamespace();
    bool preparePidNamespace();
    int enter(std::function<int()> setupFunc);

    ~Environment() = default;

private:
    PTY vterm{};
    std::string rootFs{};
    
    bool switchRoot();
};