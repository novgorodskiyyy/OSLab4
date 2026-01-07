#include "shared_utils.h"
#include <fstream>
#include "message.h"

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
        file.write((char*)&empty_msg, sizeof(Message));
    }
    file.close();
}