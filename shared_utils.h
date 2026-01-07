#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <string>

bool file_exists(const std::string& filename);
void create_empty_file(const std::string& filename, int record_count);

#endif