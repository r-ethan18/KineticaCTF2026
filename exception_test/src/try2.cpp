#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <atomic>

// Windows Headers
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
// Linux Headers
#elif defined(__linux__)
    #include <sys/ptrace.h>
    #include <unistd.h>
    #include <fstream>
    #include <string>
// macOS Headers
#elif defined(__APPLE__)
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <unistd.h>
#endif

// Check method safe for continuous background execution
bool isDebuggerPresentContinuous() {
#if defined(_WIN32) || defined(_WIN64)
    return IsDebuggerPresent() != 0;

#elif defined(__linux__)
    // Only check TracerPid in background loop to prevent false positives from ptrace
    std::ifstream statusFile("/proc/self/status");
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.rfind("TracerPid:", 0) == 0) {
            try {
                int tracerPid = std::stoi(line.substr(10));
                if (tracerPid != 0) {
                    return true;
                }
            } catch (...) {
                return false;
            }
        }
    }
    return false;

#elif defined(__APPLE__)
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    struct kinfo_proc info;
    size_t size = sizeof(info);
    info.kp_proc.p_flag = 0;

    if (sysctl(mib, 4, &info, &size, NULL, 0) == 0) {
        return (info.kp_proc.p_flag & P_TRACED) != 0;
    }
    return false;

#else
    return false;
#endif
}

// Initial check that includes one-time lockouts (like ptrace)
bool isDebuggerPresentStartup() {
#if defined(__linux__)
    // Execute ptrace TRACEME once at startup to lock out debugger attachment
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
        return true; 
    }
#endif
    return isDebuggerPresentContinuous();
}

std::atomic<bool> keepRunning{true};

void monitorDebugger() {
    while (keepRunning) {
        if (isDebuggerPresentContinuous()) {
            std::cout << "\n[!] Debugger attached midway! Terminating...\n";
            std::exit(EXIT_FAILURE);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    // 1. Perform initial startup check & lock Linux ptrace
    if (isDebuggerPresentStartup()) {
        std::cout << "[!] Debugger detected at startup. Terminating...\n";
        std::exit(EXIT_FAILURE);
    }

    // 2. Launch background monitoring thread
    std::thread debugThread(monitorDebugger);
    debugThread.detach();

    std::cout << "[+] Program running normally without a debugger.\n";

    // Dummy application loop
    volatile int a = 0;
    while (true) {
        a++;
        a--;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
