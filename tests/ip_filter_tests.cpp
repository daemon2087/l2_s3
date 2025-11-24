// Определение модуля тестов - задает имя набора тестов
#define BOOST_TEST_MODULE ip_filter_test
#include <boost/test/unit_test.hpp>
#include "../src/ip_filter_lib.h"

// Начало группы тестов для функции split
BOOST_AUTO_TEST_SUITE(test_split_function)

// Тест: проверка разбиения пустой строки
BOOST_AUTO_TEST_CASE(test_split_empty_string)
{
    auto result = split("", '.');
    // Проверка: размер результата должен быть равен 1
    BOOST_CHECK_EQUAL(result.size(), 1);
    // Проверка: первый элемент должен быть пустой строкой
    BOOST_CHECK_EQUAL(result[0], "");
}

// Тест: проверка разбиения простой строки без разделителей
BOOST_AUTO_TEST_CASE(test_split_simple_string)
{
    auto result = split("11", '.');
    // Проверка: размер результата должен быть равен 1
    BOOST_CHECK_EQUAL(result.size(), 1);
    // Проверка: результат должен содержать исходную строку
    BOOST_CHECK_EQUAL(result[0], "11");
}

// Тест: проверка разбиения строки на две части
BOOST_AUTO_TEST_CASE(test_split_two_parts)
{
    auto result = split("11.22", '.');
    // Проверка: размер результата должен быть равен 2
    BOOST_CHECK_EQUAL(result.size(), 2);
    // Проверка: первая часть должна быть "11"
    BOOST_CHECK_EQUAL(result[0], "11");
    // Проверка: вторая часть должна быть "22"
    BOOST_CHECK_EQUAL(result[1], "22");
}

// Тест: проверка разбиения строки с пустыми частями (две точки подряд)
BOOST_AUTO_TEST_CASE(test_split_empty_parts)
{
    auto result = split("..", '.');
    // Проверка: размер результата должен быть равен 3 (пустая строка, пустая строка, пустая строка)
    BOOST_CHECK_EQUAL(result.size(), 3);
    // Проверка: все три части должны быть пустыми строками
    BOOST_CHECK_EQUAL(result[0], "");
    BOOST_CHECK_EQUAL(result[1], "");
    BOOST_CHECK_EQUAL(result[2], "");
}

// Тест: проверка разбиения строки, начинающейся с точки
BOOST_AUTO_TEST_CASE(test_split_start_dot)
{
    auto result = split(".11", '.');
    // Проверка: размер результата должен быть равен 2
    BOOST_CHECK_EQUAL(result.size(), 2);
    // Проверка: первая часть должна быть пустой строкой
    BOOST_CHECK_EQUAL(result[0], "");
    // Проверка: вторая часть должна быть "11"
    BOOST_CHECK_EQUAL(result[1], "11");
}

// Тест: проверка разбиения строки, заканчивающейся точкой
BOOST_AUTO_TEST_CASE(test_split_end_dot)
{
    auto result = split("11.", '.');
    // Проверка: размер результата должен быть равен 2
    BOOST_CHECK_EQUAL(result.size(), 2);
    // Проверка: первая часть должна быть "11"
    BOOST_CHECK_EQUAL(result[0], "11");
    // Проверка: вторая часть должна быть пустой строкой
    BOOST_CHECK_EQUAL(result[1], "");
}

// Конец группы тестов для функции split
BOOST_AUTO_TEST_SUITE_END()

// Начало группы тестов для логики фильтрации IP-адресов
BOOST_AUTO_TEST_SUITE(test_ip_filter_logic)

// Тест: проверка парсинга IP-адреса из строки с табуляцией
BOOST_AUTO_TEST_CASE(test_parse_ip_pool)
{
    std::vector<std::string> lines = {"1.2.3.4\tsome_data"};
    auto ip_pool = parse_ip_pool(lines);
    
    // Проверка: должен быть распарсен один IP-адрес
    BOOST_CHECK_EQUAL(ip_pool.size(), 1);
    // Проверка: IP-адрес должен состоять из 4 октетов
    BOOST_CHECK_EQUAL(ip_pool[0].size(), 4);
    // Проверка: первый октет должен быть "1"
    BOOST_CHECK_EQUAL(ip_pool[0][0], "1");
    // Проверка: второй октет должен быть "2"
    BOOST_CHECK_EQUAL(ip_pool[0][1], "2");
    // Проверка: третий октет должен быть "3"
    BOOST_CHECK_EQUAL(ip_pool[0][2], "3");
    // Проверка: четвертый октет должен быть "4"
    BOOST_CHECK_EQUAL(ip_pool[0][3], "4");
}

// Тест: проверка сортировки IP-адресов в обратном лексикографическом порядке
BOOST_AUTO_TEST_CASE(test_sort_ip_pool)
{
    std::vector<std::string> lines = {
        "1.2.3.4\tsome_data",
        "2.1.1.1\tsome_data",
        "1.1.1.1\tsome_data"
    };
    
    auto ip_pool = parse_ip_pool(lines);
    sort_ip_pool(ip_pool);
    
    // Проверка: первый IP должен начинаться с "2" (наибольший)
    BOOST_CHECK_EQUAL(ip_pool[0][0], "2");
    // Проверка: второй IP должен начинаться с "1"
    BOOST_CHECK_EQUAL(ip_pool[1][0], "1");
    // Проверка: второй октет второго IP должен быть "2" (1.2.3.4)
    BOOST_CHECK_EQUAL(ip_pool[1][1], "2");
    // Проверка: третий IP должен начинаться с "1"
    BOOST_CHECK_EQUAL(ip_pool[2][0], "1");
    // Проверка: второй октет третьего IP должен быть "1" (1.1.1.1)
    BOOST_CHECK_EQUAL(ip_pool[2][1], "1");
}

// Тест: проверка фильтрации IP-адресов по первому байту
BOOST_AUTO_TEST_CASE(test_filter_by_first_byte)
{
    std::vector<std::string> lines = {
        "1.2.3.4\tsome_data",
        "2.1.1.1\tsome_data",
        "1.1.1.1\tsome_data"
    };
    
    auto ip_pool = parse_ip_pool(lines);
    auto filtered = filter_by_first_byte(ip_pool, 1);
    
    // Проверка: должно быть отфильтровано 2 IP-адреса с первым байтом = 1
    BOOST_CHECK_EQUAL(filtered.size(), 2);
    // Проверка: первый отфильтрованный IP должен начинаться с "1"
    BOOST_CHECK_EQUAL(filtered[0][0], "1");
    // Проверка: второй отфильтрованный IP должен начинаться с "1"
    BOOST_CHECK_EQUAL(filtered[1][0], "1");
}

// Тест: проверка фильтрации IP-адресов по первым двум байтам
BOOST_AUTO_TEST_CASE(test_filter_by_first_two_bytes)
{
    std::vector<std::string> lines = {
        "46.70.1.1\tsome_data",
        "46.71.1.1\tsome_data",
        "46.70.2.2\tsome_data"
    };
    
    auto ip_pool = parse_ip_pool(lines);
    auto filtered = filter_by_first_two_bytes(ip_pool, 46, 70);
    
    // Проверка: должно быть отфильтровано 2 IP-адреса с первыми байтами 46.70
    BOOST_CHECK_EQUAL(filtered.size(), 2);
    // Проверка: первый отфильтрованный IP должен начинаться с "46"
    BOOST_CHECK_EQUAL(filtered[0][0], "46");
    // Проверка: второй байт первого IP должен быть "70"
    BOOST_CHECK_EQUAL(filtered[0][1], "70");
    // Проверка: второй отфильтрованный IP должен начинаться с "46"
    BOOST_CHECK_EQUAL(filtered[1][0], "46");
    // Проверка: второй байт второго IP должен быть "70"
    BOOST_CHECK_EQUAL(filtered[1][1], "70");
}

// Тест: проверка фильтрации IP-адресов по любому байту (в любом октете)
BOOST_AUTO_TEST_CASE(test_filter_by_any_byte)
{
    std::vector<std::string> lines = {
        "1.2.3.4\tsome_data",
        "5.6.7.8\tsome_data",
        "9.46.11.12\tsome_data"
    };
    
    auto ip_pool = parse_ip_pool(lines);
    auto filtered = filter_by_any_byte(ip_pool, 46);
    
    // Проверка: должен быть найден один IP-адрес, содержащий байт 46
    BOOST_CHECK_EQUAL(filtered.size(), 1);
    // Проверка: найденный IP должен содержать "46" во втором октете
    BOOST_CHECK_EQUAL(filtered[0][1], "46");
}

// Тест: проверка преобразования IP-адреса из вектора строк в строковое представление
BOOST_AUTO_TEST_CASE(test_ip_to_string)
{
    std::vector<std::string> ip_parts = {"192", "168", "1", "1"};
    std::string result = ip_to_string(ip_parts);
    // Проверка: результат должен быть строкой "192.168.1.1"
    BOOST_CHECK_EQUAL(result, "192.168.1.1");
}

// Конец группы тестов для логики фильтрации IP-адресов
BOOST_AUTO_TEST_SUITE_END()
