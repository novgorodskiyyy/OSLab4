#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cstring>

#define MAX_MSG_LEN 20

struct Message {
    int sender_id;
    char text[MAX_MSG_LEN + 1];
    bool active;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <sender_id>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    int sender_id = atoi(argv[2]);

    Sleep(1000);

    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file" << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end);
    long long file_size = (long long)file.tellg();
    int record_count = (int)(file_size / sizeof(Message));

    std::cout << "Sender " << sender_id << " ready" << std::endl;
    std::cout << "\nCommands:\n  send - send message\n  exit - quit\n";

    while (true) {
        std::string command;
        std::cout << "\nSender " << sender_id << " enter command: ";
        std::cin >> command;

        if (command == "exit") break;

        if (command == "send") {
            std::string message_text;
            std::cout << "Enter message (max 20 chars): ";
            std::cin.ignore();
            std::getline(std::cin, message_text);

            if (message_text.length() > MAX_MSG_LEN) {
                message_text = message_text.substr(0, MAX_MSG_LEN);
            }

            bool sent = false;
            while (!sent) {
                for (int i = 0; i < record_count; ++i) {
                    file.seekg(i * sizeof(Message));
                    Message msg;
                    file.read((char*)&msg, sizeof(Message));

                    if (!msg.active) {
                        msg.active = true;
                        msg.sender_id = sender_id;
                        strncpy(msg.text, message_text.c_str(), MAX_MSG_LEN);
                        msg.text[MAX_MSG_LEN] = '\0';

                        file.seekp(i * sizeof(Message));
                        file.write((const char*)&msg, sizeof(Message));
                        file.flush();

                        std::cout << "Message sent: " << message_text << std::endl;
                        sent = true;
                        break;
                    }
                }

                if (!sent) {
                    std::cout << "File full. Waiting..." << std::endl;
                    Sleep(1000);
                }
            }
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }

    file.close();
    return 0;
}