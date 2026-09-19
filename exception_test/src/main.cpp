#include <iostream>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <chrono>

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

bool isDebuggerPresent() {
#if defined(_WIN32) || defined(_WIN64)
    // Windows: Use the Win32 API to check the Process Environment Block (PEB)
    return IsDebuggerPresent() != 0;

#elif defined(__linux__)
    // Linux Method 1: Check TracerPid in /proc/self/status
    std::ifstream statusFile("/proc/self/status");
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.rfind("TracerPid:", 0) == 0) {
            int tracerPid = std::stoi(line.substr(10));
            if (tracerPid != 0) {
                return true; // A process is tracing this program
            }
        }
    }

    // Linux Method 2: Try to attach ptrace to ourselves (only one process can attach at a time)
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
        return true; // Ptrace failed because a debugger is already attached
    }
    return false;

#elif defined(__APPLE__)
    // macOS: Query sysctl for the P_TRACED flag in the process information structure
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    struct kinfo_proc info;
    size_t size = sizeof(info);
    
    // Initialize structure
    info.kp_proc.p_flag = 0;

    if (sysctl(mib, 4, &info, &size, NULL, 0) == 0) {
        return (info.kp_proc.p_flag & P_TRACED) != 0;
    }
    return false;

#else
    // Fallback for unsupported platforms
    return false;
#endif
}

std::atomic<bool> keepRunning{true};

void monitorDebugger() {
    while (keepRunning) {
        if (isDebuggerPresent()) {
            std::cout << "[!] Debugger attached midway! Terminating...\n";
            std::exit(EXIT_FAILURE);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Check twice a second
    }
}

int main() {
    // Perform check at startup
    if (isDebuggerPresent()) {
        std::cout << "[!] Debugger detected. Terminating process immediately.\n";
        std::exit(EXIT_FAILURE);
    }

    std::thread debugThread(monitorDebugger);
    debugThread.detach(); // Runs silently in the background

    std::cout << "[+] Program running normally without a debugger.\n";

    // ... Your application code here ...
    int a;
    while(1){
	a++;
	a--;
    }

    return 0;
}
