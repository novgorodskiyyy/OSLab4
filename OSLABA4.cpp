#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

#define MAX_MSG_LEN 20
#define MAX_SENDERS 10

struct Message {
    int sender_id;
    char text[MAX_MSG_LEN + 1];
    bool active;
};

bool file_exists(const std::string& filename) {
    std::ifstream f(filename);
    return f.good();
}

void create_empty_file(const std::string& filename, int record_count) {
    std::fstream file(filename, std::ios::out | std::ios::binary);
    Message empty_msg;
    empty_msg.active = false;
    empty_msg.sender_id = -1;
    empty_msg.text[0] = '\0';

    for (int i = 0; i < record_count; ++i) {
        file.write((const char*)&empty_msg, sizeof(Message));
    }
    file.close();
}

int main() {
    std::string filename;
    int record_count;
    int sender_count;

    std::cout << "Enter binary filename: ";
    std::cin >> filename;

    std::cout << "Enter record count: ";
    std::cin >> record_count;

    if (file_exists(filename)) {
        std::cout << "File exists. Deleting..." << std::endl;
        std::remove(filename.c_str());
    }

    create_empty_file(filename, record_count);

    std::cout << "Enter number of Sender processes: ";
    std::cin >> sender_count;

    if (sender_count > MAX_SENDERS) {
        std::cout << "Too many Sender processes. Max: " << MAX_SENDERS << std::endl;
        return 1;
    }

    std::vector<STARTUPINFO> si(sender_count);
    std::vector<PROCESS_INFORMATION> pi(sender_count);
    std::vector<HANDLE> processes;

    for (int i = 0; i < sender_count; ++i) {
        ZeroMemory(&si[i], sizeof(STARTUPINFO));
        si[i].cb = sizeof(STARTUPINFO);
        ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));

        std::string cmd = "sender.exe " + filename + " " + std::to_string(i);

        if (!CreateProcess(
            NULL,
            const_cast<char*>(cmd.c_str()),
            NULL,
            NULL,
            FALSE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si[i],
            &pi[i]
        )) {
            std::cerr << "Failed to create Sender process " << i << std::endl;
            return 1;
        }

        processes.push_back(pi[i].hProcess);
        std::cout << "Started Sender process with ID: " << i << std::endl;
        CloseHandle(pi[i].hThread);
    }

    Sleep(2000);

    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file" << std::endl;
        return 1;
    }

    std::cout << "\nCommands:\n";
    std::cout << "  read - read message\n";
    std::cout << "  exit - quit\n";

    while (true) {
        std::string command;
        std::cout << "\nEnter command: ";
        std::cin >> command;

        if (command == "exit") {
            break;
        }
        else if (command == "read") {
            bool message_read = false;

            for (int i = 0; i < record_count; ++i) {
                file.seekg(i * sizeof(Message));
                Message msg;
                file.read((char*)&msg, sizeof(Message));

                if (msg.active) {
                    std::cout << "Message from Sender " << msg.sender_id << ": " << msg.text << std::endl;

                    msg.active = false;
                    file.seekp(i * sizeof(Message));
                    file.write((const char*)&msg, sizeof(Message));
                    file.flush();

                    message_read = true;
                    break;
                }
            }

            if (!message_read) {
                std::cout << "File empty. Waiting for messages..." << std::endl;
                Sleep(1000);
            }
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }

    file.close();

    for (HANDLE hProcess : processes) {
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
    }

    std::cout << "Receiver finished work" << std::endl;
    return 0;
}