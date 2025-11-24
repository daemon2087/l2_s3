#include "ip_filter_lib.h"
#include <algorithm>
#include <stdexcept>

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

IPPool parse_ip_pool(const std::vector<std::string>& lines)
{
    IPPool ip_pool;

    for(const auto& line : lines)
    {
        std::vector<std::string> v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
    }

    return ip_pool;
}

void sort_ip_pool(IPPool& ip_pool)
{
    std::sort(ip_pool.begin(), ip_pool.end(),
        [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
            // Convert to int for numeric comparison
            auto a0 = std::stoi(a[0]), a1 = std::stoi(a[1]), a2 = std::stoi(a[2]), a3 = std::stoi(a[3]);
            auto b0 = std::stoi(b[0]), b1 = std::stoi(b[1]), b2 = std::stoi(b[2]), b3 = std::stoi(b[3]);
            return std::tie(a0, a1, a2, a3) > std::tie(b0, b1, b2, b3);
        });
}

IPPool filter_by_first_byte(const IPPool& ip_pool, int first_byte)
{
    IPPool result;
    for (const auto& ip : ip_pool) {
        if (std::stoi(ip[0]) == first_byte) {
            result.push_back(ip);
        }
    }
    return result;
}

IPPool filter_by_first_two_bytes(const IPPool& ip_pool, int first_byte, int second_byte)
{
    IPPool result;
    for (const auto& ip : ip_pool) {
        if (std::stoi(ip[0]) == first_byte && std::stoi(ip[1]) == second_byte) {
            result.push_back(ip);
        }
    }
    return result;
}

IPPool filter_by_any_byte(const IPPool& ip_pool, int byte_value)
{
    IPPool result;
    for (const auto& ip : ip_pool) {
        bool match = false;
        for (const auto& part : ip) {
            if (std::stoi(part) == byte_value) {
                match = true;
                break;
            }
        }
        if (match) {
            result.push_back(ip);
        }
    }
    return result;
}

std::string ip_to_string(const std::vector<std::string>& ip)
{
    std::string result;
    for (size_t i = 0; i < ip.size(); ++i) {
        if (i > 0) result += ".";
        result += ip[i];
    }
    return result;
}
