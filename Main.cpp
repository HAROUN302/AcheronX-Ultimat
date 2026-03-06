#include "AcheronX_Core.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "--- [ ACHERON-X ULTIMATE TERMINAL ] ---" << std::endl;
    if (argc < 2) { 
        std::cout << "Usage: ./acheron <file_path>" << std::endl; 
        return 1; 
    }

    std::string target = argv[1];
    std::cout << "[*] Initiating Non-Linear SPN Protocol..." << std::endl;
    
    if (AcheronEngine::Encrypt(target, "DUMMY_PUB_KEY")) {
        std::cout << "[+] Status: Target Neutralized & Encrypted." << std::endl;
        std::cout << "[+] Anti-Forensic Shredding: COMPLETED." << std::endl;
    } else {
        std::cout << "[-] Critical Error: Operation Aborted." << std::endl;
    }
    return 0;
}
