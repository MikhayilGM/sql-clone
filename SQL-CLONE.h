#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <vector>

enum class Command
{
    CREATE = 0,
    DROP,
    DATABASE,
    TABLE,
    INSERTINTO,
    SELECT,
    UPDATE,
    DELETE,
    OPENDATABASE,
};

enum class Types
{
    INT, 
    VARCHAR,
    DATE,
};

enum class where_operator
{
    equal = 0,
    greater,
    less,
    greater_equal,
    less_equal,
    not_equal, 
};

struct process_expression
{
    std::string field_name;
    where_operator operator_;
    std::string value;
};

std::map<std::string, Types> TypesParse;
std::map<std::string, Command> CommandParse;
std::map<std::string, where_operator> where_operatorParse;
std::string DataBaseName = "";

std::map<std::string, std::map<std::string, std::map<std::string, std::pair<Types, std::vector<std::string>>>>> DATABASE;

void push_parse_types();
void push_parse_commands();
void  push_parse_where_operator();
bool is_number(const std::string& str);
bool is_date(const std::string& expression);
int to_date(const std::string& date);
size_t find_pos(std::vector<std::string>& vec, const std::string& value);
std::vector<std::string> split(const std::string& str, const std::string& spliter);
std::string take_field_name(const std::string& fields_name);
std::string remove_if(const std::string& value, char removal_value);
bool check_type(const Types type, const std::string& expression);
bool is_type(const std::string& type_name);
bool equal(const process_expression& value, const Types& type_name, std::string& TableName, int& number_data);
bool greater(const process_expression& value, const Types& type_name, std::string& TableName, int number_data);
bool less(const process_expression& value, const Types& type_name, std::string& TableName, int number_data);
bool greater_equal(const process_expression& value, const Types& type_name, std::string& TableName, int number_data);
bool less_equal(const process_expression& value, const Types& type_name, std::string& TableName, int number_data);
bool not_equal(const process_expression& value, const Types& type_name, std::string& TableName, int number_data);
bool resolve_expressions(const std::vector<bool>& expressions, const std::vector<std::string>& logic);
bool resolve_where_token(const process_expression& value, const Types& type_name, std::string& TableName, int number_data);
bool where_token_to_bool(const std::string& expression, std::string& TableName, int number_data);
void add_data(const std::string& filename, const std::string& add_value);
void add_data(const std::string& filename, const std::vector<std::string>& add_value);
void update_data(const std::string& filename, const std::vector<std::string>& update_value);
std::vector<std::string> make_vector(const std::string& filename);
void create_database(std::vector<std::string>& CommandLine);
void create_table(std::vector<std::string>& CommandLine);
void create(std::vector<std::string>& CommandLine);
void insert_into(std::vector<std::string>& CommandLine);
bool where(std::vector<std::string>& CommandLine, std::string& TableName, int number_data);
void select(std::vector<std::string>& CommandLine);
void update(std::vector<std::string>& CommandLine);
void delete_data(std::vector<std::string>& CommandLine);
void open_db(std::vector<std::string>& CommandLine);
void drop_database(std::vector<std::string>& CommandLine);
void drop_table(std::vector<std::string>& CommandLine);
void drop(std::vector<std::string>& CommandLine);
void complete_command(std::vector<std::string>& CommandLine);
void execute();