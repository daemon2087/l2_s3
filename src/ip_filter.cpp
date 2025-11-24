#include "ip_filter_lib.h"
#include <iostream>
#include <string>
#include <vector>

int main(){
    try{
        std::vector<std::string> lines;
        for(std::string line; std::getline(std::cin, line);){
            lines.push_back(line);
        }

        IPPool ip_pool = parse_ip_pool(lines);
        sort_ip_pool(ip_pool);

        // Вывод всех IP
        std::cout << "All IP: \n";
        for(const auto& ip : ip_pool){
            std::cout << ip_to_string(ip) << "\n";
        }

        // Фильтр по первому байту = 1
        std::cout << "Filtered by first byte = 1: \n";
        auto filtered_1 = filter_by_first_byte(ip_pool, 1);
        for(const auto& ip : filtered_1){
            std::cout << ip_to_string(ip) << "\n";
        }

        // Фильтр по первым двум байтам = 46, 70
        std::cout << "Filtered by first two bytes = 46, 70: \n";
        auto filtered_46_70 = filter_by_first_two_bytes(ip_pool, 46, 70);
        for(const auto& ip : filtered_46_70){
            std::cout << ip_to_string(ip) << "\n";
        }

        // Фильтр по любому байту = 46
        std::cout << "Filtered by any byte = 46: \n";
        auto filtered_any_46 = filter_by_any_byte(ip_pool, 46);
        for(const auto& ip : filtered_any_46){
            std::cout << ip_to_string(ip) << "\n";
        }
    }
    catch(const std::exception &e){
        std::cerr << e.what() << "\n";
    }

    return 0;
}
