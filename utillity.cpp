#include "utility.h"

bool is_number(const std::string& str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if(!isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
    
}

bool is_date(const std::string& expression)
{
    if(
        expression.size() != 10 &&
        expression[0] >= '0' && expression[0] <= '1' && 
        expression[1] >= '0' && expression[1] <= '9' && 
        expression[2] == '-' &&
        expression[3] >= '0' && expression[3] <= '1' && 
        expression[4] >= '0' && expression[4] <= '9' && 
        expression[5] == '-' &&
        expression[6] >= '0' && expression[6] <= '2' && 
        expression[7] >= '0' && expression[7] <= '9' && 
        expression[8] >= '0' && expression[8] <= '9' && 
        expression[9] >= '0' && expression[9] <= '9' 
    )
    {
        if((expression[0] == '1'  && expression[1] > '1') || (expression[3] == '1'  && expression[4] > '1'))
        {
            return true;
        }
        else
        {
            std::cout << "Wrong format of Date type.(Months Must be less then 12)\n";
            return false;
        }
    }
    else
    {
        std::cout << "Wrong format of Date type.(Format must be dd-mm-yyyy)\n";
        return false;
    }

    //dd-mm-yyyy
}

int to_date(const std::string& date)
{
    return (date[0] - '0')*10 + (date[1] - '0')*1 + (date[3] - '0')*30*10 + (date[4] - '0')*30 + (date[6] - '0')*365*1000 +  (date[7] - '0')*365*100 + (date[8] - '0')*365*10 + (date[9] - '0')*365;
}

size_t find_pos(std::vector<std::string>& vec, const std::string& value)
{
    for(size_t i = 0; i < vec.size(); ++i)
    {
        if(vec[i] == value)
        {
            return i;
        }
    }

    return UINT_MAX;
}

std::vector<std::string> split(const std::string& str, const std::string& spliter)
{
    std::vector<std::string> vec;
    std::string tmp;

    for(size_t i = 0; i < str.size(); i++)
    {
        bool smt = 1;
        for (size_t j = 0; j < spliter.size(); j++)
        {
            if(spliter[j] == str[i])
            {
                if(tmp.size() != 0)
                {
                    vec.push_back(tmp);
                    tmp.clear();
                }
                smt = 0;
                break;
            }
        }

        if(smt)
        {
            tmp.push_back(str[i]);
        }
        
    }

    vec.push_back(tmp);

    return vec;
}

std::string take_field_name(const std::string& fields_name)
{
    return split(fields_name, " ")[1];
}

std::string remove_if(const std::string& value, char removal_value)
{
    std::string tmp;
    for(size_t i = 0; i < value.size(); ++i)
    {

        if(value[i] != removal_value)
        {
            tmp.push_back(value[i]);
        }
    }

    return tmp;
}