#include <iostream>
#include <unistd.h>
#include <err.h>
#include <cstring>

int child() {
    std::cout << "Here is my pid: " << getpid() << std::endl;
    // const char* path = "/bin/ls";
    const char* path = "/bin/echo";

    // char* const args[] = {const_cast<char*>("ls"), const_cast<char*>("-l"), nullptr};
    char* const args[] = {const_cast<char*>("/bin/echo"), const_cast<char*>("Hello!"), nullptr};

    
    // 设置环境变量，这里仅使用当前环境
    char* const env[] = {nullptr};

    // 尝试执行指定的命令
    if (execve(path, args, env) == -1) {
        std::cerr << "execve failed with error: " << strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}

void parent(pid_t ret) {

    std::cout << "Here is my pid: " << getpid() << std::endl;
    std::cout << "Here is my child pid: " << ret << std::endl;
}


int main () {
    pid_t ret;
    ret = fork();

    if (ret == 0) {
        std::cout << "Here is the child process" << std::endl;
        child();
    } else if (ret > 0) {
        std::cout << "Here is the parent process" << std::endl;
        parent(ret);
    } else {
        std::runtime_error("fork() failed");
    }
    return 0;

}
