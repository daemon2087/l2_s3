#ifndef IP_FILTER_LIB_H
#define IP_FILTER_LIB_H

#include <string>
#include <vector>

using IPPool = std::vector<std::vector<std::string>>;

std::vector<std::string> split(const std::string &str, char d);
IPPool parse_ip_pool(const std::vector<std::string>& lines);
void sort_ip_pool(IPPool& ip_pool);
IPPool filter_by_first_byte(const IPPool& ip_pool, int first_byte);
IPPool filter_by_first_two_bytes(const IPPool& ip_pool, int first_byte, int second_byte);
IPPool filter_by_any_byte(const IPPool& ip_pool, int byte_value);
std::string ip_to_string(const std::vector<std::string>& ip);

#endif // IP_FILTER_LIB_H
