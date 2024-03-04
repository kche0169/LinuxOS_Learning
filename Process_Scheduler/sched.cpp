#include <iostream>
#include <cstdlib>
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include <thread>

void childProcess(int childNumber) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) {
         // 假设每个进程运行100次输出
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

        std::cout << "子进程 " << childNumber << ", PID: " << getpid() << ", 已工作时长: " << duration << " ms" << std::endl;
        
        // 睡眠约1ms（调整为100ms以减少输出量）
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void showVar(int ProcessNum, int resol, int total) {
    std::cout << "Process Number: " << ProcessNum << std::endl;
    std::cout << "Resol: " << resol << "ms" << std::endl;
    std::cout << "Total: " << total << "ms" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) { // 确保有足够的命令行参数
        std::cerr << "Usage: " << argv[0] << " <ProcessNum> <resol> <total>" << std::endl;
        return EXIT_FAILURE;
    }

    int ProcessNum = std::stoi(argv[1]);
    int resol = std::stoi(argv[2]);
    int total = std::stoi(argv[3]);
    // 命令行参数错误处理

    if (total % resol) {
        std::cerr << "Total: " << total << " must be a multiple of resol: " << resol << "." << std::endl;
        return EXIT_FAILURE;
    }


    showVar(ProcessNum, resol, total);

    for (int i = 0; i < ProcessNum; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            std::cerr << "fork失败" << std::endl;
            return 1;
        } else if (pid == 0) {
            childProcess(i);
            _exit(0); // 子进程安全退出
        }
    }
    // 父进程等待所有子进程完成
    for (int i = 0; i < ProcessNum; ++i) {
        wait(NULL);
    }

    std::cout << "所有子进程都已完成。" << std::endl;

    return 0;


}
