#pragma once


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

bool validateArguments(int argc) {
    return argc == 3;
}

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <filename> <sender_id>" << std::endl;
}

int parseSenderId(const char* idStr) {
    return atoi(idStr);
}

std::fstream openFile(const std::string& filename) {
    return std::fstream(filename, std::ios::in | std::ios::out | std::ios::binary);
}

bool isFileOpen(std::fstream& file) {
    return file.is_open() && file.good();
}

int calculateRecordCount(std::fstream& file) {
    file.seekg(0, std::ios::end);
    long long file_size = static_cast<long long>(file.tellg());
    return static_cast<int>(file_size / sizeof(Message));
}

std::string getMessageFromUser() {
    std::string message_text;
    std::cout << "Enter message (max 20 chars): ";
    std::getline(std::cin, message_text);

    if (message_text.length() > MAX_MSG_LEN) {
        message_text = message_text.substr(0, MAX_MSG_LEN);
    }

    return message_text;
}

Message createMessage(int sender_id, const std::string& message_text) {
    Message msg;
    msg.sender_id = sender_id;
    msg.active = true;
    strncpy(msg.text, message_text.c_str(), MAX_MSG_LEN);
    msg.text[MAX_MSG_LEN] = '\0';
    return msg;
}

bool findAndWriteMessage(std::fstream& file, int record_count, const Message& new_msg) {
    for (int i = 0; i < record_count; ++i) {
        file.seekg(i * sizeof(Message));
        Message existing_msg;
        file.read(reinterpret_cast<char*>(&existing_msg), sizeof(Message));

        if (!existing_msg.active) {
            file.seekp(i * sizeof(Message));
            file.write(reinterpret_cast<const char*>(&new_msg), sizeof(Message));
            file.flush();
            return true;
        }
    }
    return false;
}

bool trySendMessage(std::fstream& file, int record_count, int sender_id, const std::string& message_text) {
    Message new_msg = createMessage(sender_id, message_text);
    return findAndWriteMessage(file, record_count, new_msg);
}

void sendMessageLoop(std::fstream& file, int record_count, int sender_id) {
    std::string message_text = getMessageFromUser();

    bool sent = false;
    while (!sent) {
        sent = trySendMessage(file, record_count, sender_id, message_text);

        if (!sent) {
            std::cout << "File full. Waiting..." << std::endl;
            Sleep(1000);
        }
        else {
            std::cout << "Message sent: " << message_text << std::endl;
        }
    }
}

void printSenderReady(int sender_id) {
    std::cout << "Sender " << sender_id << " ready" << std::endl;
}

void printCommands() {
    std::cout << "\nCommands:\n  send - send message\n  exit - quit\n";
}

void printUnknownCommand() {
    std::cout << "Unknown command" << std::endl;
}

void processCommands(std::fstream& file, int record_count, int sender_id) {
    while (true) {
        std::string command;
        std::cout << "\nSender " << sender_id << " enter command: ";
        std::cin >> command;

        if (command == "exit") {
            break;
        }
        else if (command == "send") {
            std::cin.ignore();
            sendMessageLoop(file, record_count, sender_id);
        }
        else {
            printUnknownCommand();
        }
    }
}

int main(int argc, char* argv[]) {
    if (!validateArguments(argc)) {
        printUsage(argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    int sender_id = parseSenderId(argv[2]);

    Sleep(1000);

    std::fstream file = openFile(filename);
    if (!isFileOpen(file)) {
        std::cerr << "Cannot open file" << std::endl;
        return 1;
    }

    int record_count = calculateRecordCount(file);
    printSenderReady(sender_id);
    printCommands();

    processCommands(file, record_count, sender_id);

    file.close();
    return 0;
}