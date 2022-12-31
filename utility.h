#include <string>
#include <iostream>
#include <vector>

bool is_number(const std::string& str);

bool is_date(const std::string& expression);

int to_date(const std::string& date);

size_t find_pos(std::vector<std::string>& vec, const std::string& value);

std::vector<std::string> split(const std::string& str, const std::string& spliter);

std::string take_field_name(const std::string& fields_name);

std::string remove_if(const std::string& value, char removal_value);