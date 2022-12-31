#include "SQL_Clone.h"

void  SQL_Clone::push_parse_where_operator()
{
    where_operatorParse["="] = where_operator::equal;
    where_operatorParse[">"] = where_operator::greater;
    where_operatorParse["<"] = where_operator::less;
    where_operatorParse[">="] = where_operator::greater_equal;
    where_operatorParse["<="] = where_operator::less_equal;
    where_operatorParse["<>"] = where_operator::not_equal;
}

bool SQL_Clone::equal(const process_expression& value, const Types& type_name, std::string& TableName, int& number_data)
{
    switch (type_name)
    {
    case Types::INT:
        return std::stoi(value.value) == std::stoi(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;

    case Types::DATE:
        return to_date(value.value) == to_date(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;

    case Types::VARCHAR:
        value.value == DATABASE[DataBaseName][TableName][value.field_name].second[number_data];
        break;
    
    default:
        return false;
        break;
    }
    return false;
}

bool SQL_Clone::greater(const process_expression& value, const Types& type_name, std::string& TableName, int number_data)
{
    switch (type_name)
    {
    case Types::INT:
        return std::stoi(value.value) > std::stoi(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;

    case Types::DATE:
        return to_date(value.value) > to_date(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;
    
    default:
        return false;
        break;
    }
}

bool SQL_Clone::less(const process_expression& value, const Types& type_name, std::string& TableName, int number_data)
{
    switch (type_name)
    {
    case Types::INT:
        return std::stoi(value.value) < std::stoi(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;

    case Types::DATE:
        return to_date(value.value) < to_date(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;
    
    default:
        return false;
        break;
    }
}

bool SQL_Clone::greater_equal(const process_expression& value, const Types& type_name, std::string& TableName, int number_data)
{
    switch (type_name)
    {
    case Types::INT:
        return std::stoi(value.value) >= std::stoi(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;

    case Types::DATE:
        return to_date(value.value) >= to_date(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;
    
    default:
        return false;
        break;
    }
}

bool SQL_Clone::less_equal(const process_expression& value, const Types& type_name, std::string& TableName, int number_data)
{
    switch (type_name)
    {
    case Types::INT:
        return std::stoi(value.value) <= std::stoi(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;

    case Types::DATE:
        return to_date(value.value) <= to_date(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;
    
    default:
        return false;
        break;
    }
}

bool SQL_Clone::not_equal(const process_expression& value, const Types& type_name, std::string& TableName, int number_data)
{
    switch (type_name)
    {
    case Types::INT:

        return std::stoi(value.value) != std::stoi(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;

    case Types::DATE:
        return to_date(value.value) != to_date(DATABASE[DataBaseName][TableName][value.field_name].second[number_data]);
        break;

    case Types::VARCHAR:
        value.value != DATABASE[DataBaseName][TableName][value.field_name].second[number_data];
        break;
    
    default:
        return false;
        break;
    }

    return false;
}

bool resolve_expressions(const std::vector<bool>& expressions, const std::vector<std::string>& logic)
{
    bool resolved_expressions = expressions.front();  
    
    if(expressions.size() != logic.size()+1)
    {
        std::cout << "Wrong Format of WHERE expressions.\n";
        return false; 
    }

    for(int i = 1; i < expressions.size(); ++i)
    {
        if(logic[i-1] == "OR")
        {
            resolved_expressions = resolve_expressions || expressions[i];
        }
        else if(logic[i-1] == "AND")
        {
            resolved_expressions = resolve_expressions && expressions[i];
            if(resolved_expressions == false)
            {
                return resolved_expressions;
            }
        }
    }

    return resolved_expressions;
}

bool SQL_Clone::resolve_where_token(const process_expression& value, const Types& type_name, std::string& TableName, int number_data)
{
    switch (value.operator_)
    {
    case where_operator::equal:
        return equal(value, type_name, TableName, number_data);
    
    case where_operator::greater:
        return greater(value, type_name, TableName, number_data);

    case where_operator::less:
        return less(value, type_name, TableName, number_data);

    case where_operator::greater_equal:
        return greater_equal(value, type_name, TableName, number_data);

    case where_operator::less_equal:
        return less_equal(value, type_name, TableName, number_data);

    case where_operator::not_equal:
        return not_equal(value, type_name, TableName, number_data);
    
    default:
        return false;
        break;
    }
}

process_expression SQL_Clone::string_to_expression(const std::string& expression)
{

    process_expression token;
    std::string tmp;
    for (int i = 0; i < expression.size(); i++)
    {
        while(expression[i] <= '<' || expression[i] >= '>')
        {
            tmp.push_back(expression[i]);
            ++i;
        }

        token.field_name = tmp;
        tmp.clear();
        tmp.push_back(expression[i]);

        if(expression[++i] >= '=' && expression[i] <= '>')
        {
            tmp.push_back(expression[i++]);
        }

        
        token.operator_ = where_operatorParse[tmp];
        tmp.clear();
        while ( i < expression.size() )
        {
            tmp.push_back(expression[i]);
            ++i;
        }
        token.value = tmp;
    }

    return token;

}



bool SQL_Clone::where_token_to_bool(const std::string& expression, std::string& TableName, int number_data)
{
    process_expression token = string_to_expression(expression);

    if(DATABASE[DataBaseName][TableName][token.field_name].first == Types::VARCHAR && ( token.operator_ != where_operator::equal || token.operator_ != where_operator::not_equal))
    {
        std::cout << "VARCHAR type cant compare with <, >, >= or <= operator.\n";
        return false;
    }

    if(!check_type(DATABASE[DataBaseName][TableName][token.field_name].first, token.value))
    {
        return false;
    }

    return resolve_where_token(token, DATABASE[DataBaseName][TableName][token.field_name].first, TableName, number_data);

    return true;
}

void SQL_Clone::add_data(const std::string& filename, const std::string& add_value)
{
    std::ifstream fin;
    std::ofstream fout;
    fin.open(filename);
    std::vector<std::string> vector;
    while(!fin.eof())
    {
        char tmp[64];
        fin.getline(tmp, 64, '\n');
        std::string smt = tmp;
        if(smt != "")
        {
            vector.push_back(tmp);
        }
    }
    fin.close();
    fout.open(filename);
    
    for(int i = 0; i < vector.size(); ++i)
    {
        fout << vector[i] << '\n';
    }

    fout << add_value;
}

void SQL_Clone::add_data(const std::string& filename, const std::vector<std::string>& add_value)
{
    std::ifstream fin;
    std::ofstream fout;
    fin.open(filename);
    std::vector<std::string> vector;
    while(!fin.eof())
    {
        char tmp[64];
        fin.getline(tmp, 64, '\n');
        std::string smt = tmp;
        if(smt != "")
        {
            vector.push_back(tmp);
        }
    }
    fin.close();
    fout.open(filename);
    
    for(int i = 0; i < vector.size(); ++i)
    {
        fout << vector[i] << '\n';
    }

    for (int i = 0; i < add_value.size() - 1; i++)
    {
        fout << add_value[i] << '\n';
    }

    fout << add_value[add_value.size() - 1];
    
}

void SQL_Clone::update_data(const std::string& filename, const std::vector<std::string>& update_value)
{
    std::ofstream fout;

    fout.open(filename);

    for (int i = 0; i < update_value.size() - 1; i++)
    {
        fout << update_value[i] << '\n';
    }

    fout << update_value[update_value.size() - 1];
}

std::vector<std::string> SQL_Clone::make_vector(const std::string& filename)
{
    std::ifstream inputfile;
    inputfile.open(filename);
    std::vector<std::string> fields_name;

    while(!inputfile.eof())
    {
        char tmp[64];
        inputfile.getline(tmp, 64, '\n');
        std::string str = tmp;
        fields_name.push_back(str);
    }

    return fields_name;
}

void SQL_Clone::create_database(std::vector<std::string>& CommandLine)
{
    CommandLine[2] = remove_if(CommandLine[2] + ".data", ';');
    DataBaseName = CommandLine[2];
    std::fstream file;
    file.open(CommandLine[2], std::ios::out);
    file.close();
}

void SQL_Clone::create_table(std::vector<std::string>& CommandLine)
{
    if(CommandLine.size() < 7 || CommandLine[3][0] != '(' || CommandLine[CommandLine.size() - 1][0] != ')')
    {
        std::cout << "Wrong Format of Command CREATE TABLE. '()'\n";
        return;
    }

    std::fstream table_file;
    table_file.open(CommandLine[2] + ".tb");
    if(table_file.is_open())
    {
        std::cout << "Such Table already exists.\n";
        table_file.close();
        return;
    }

    for(int i = 4; i < CommandLine.size() - 3; i+=2)
    {
        if(CommandLine[i+1][CommandLine[i+1].size() - 1] != ',')
        {
            std::cout << "Wrong Format of Command CREATE TABLE ','.\n";
            return;
        }

        if(!is_type(CommandLine[i]))
        {
            std::cout << "Such Type is not exists\n";
            return;
        }
    }

    table_file.close();

    if(DataBaseName != "")
    {

        table_file.open(CommandLine[2] + ".tb", std::ios::out);
        std::fstream fields_creator;
        for(int i = 4; i < CommandLine.size() - 3; i+=2)
        {
            CommandLine[i+1] = remove_if(CommandLine[i+1], ',');
            fields_creator.open(CommandLine[i+1] + ".field", std::ios::out);
            table_file << CommandLine[i] << ' ' << CommandLine[i+1] << '\n';
            fields_creator.close();
            DATABASE[DataBaseName][CommandLine[2]][CommandLine[i+1]] = std::make_pair(TypesParse[CommandLine[i]], std::vector<std::string>());
        }

        CommandLine[CommandLine.size() - 2] = remove_if(CommandLine[CommandLine.size() - 2], ',');
        table_file << CommandLine[CommandLine.size() - 3] << ' ' << CommandLine[CommandLine.size() - 2];
        fields_creator.open(CommandLine[CommandLine.size() - 2] + ".field", std::ios::out);
        fields_creator.close();
        DATABASE[DataBaseName][CommandLine[2]][CommandLine[CommandLine.size() - 2]] = std::make_pair(TypesParse[CommandLine[CommandLine.size() - 3]], std::vector<std::string>());
        table_file.close();

        add_data(DataBaseName, CommandLine[2]);

    }
    else
    {
        std::cout << "DataBase not entered.\n";
        return;
    }
}

void SQL_Clone::create(std::vector<std::string>& CommandLine)
{
    switch (CommandParse[CommandLine[1]])
    {
        case Command::DATABASE:
        {   
            create_database(CommandLine);    
            break;
        }
        case Command::TABLE:
        {
            create_table(CommandLine);
            break;
        }
        default:
            break;
    }
}

void SQL_Clone::insert_into(std::vector<std::string>& CommandLine)
{
    if(DataBaseName == "")
    {
        std::cout << "DataBase not entered.\n";
        return;
    }

    if(!DATABASE[DataBaseName].contains(CommandLine[1]))
    {
        std::cout << "Table with name " << CommandLine[1] << " is not exists.\n";
        return;
    }

    std::vector<std::string> columns;

    int i = 0;

    for(i = 3; CommandLine[i][0] != ')'; ++i)
    {
        columns.push_back(remove_if(CommandLine[i], ','));
        if(!DATABASE[DataBaseName][CommandLine[1]].contains(columns.back()))
        {
            std::cout << "There is no field " << columns.back() << " in Table " << CommandLine[1] << ".\n";
            return;
        }
    }
    std::vector<std::vector<std::string>> input_info;

    if(CommandLine[++i] != "VALUES")
    {
        std::cout << "Wrong format of Command INSERT_INTO, no VALUES part.\n";
        return;
    }


    if(CommandLine[++i][0] != '(')
    {
        std::cout << "Wrong Format of INSERT_INTO.('(')\n";
        return;
    }


    ++i;
    {
    std::vector<std::string> tmp;
        for( int j = 0; i < CommandLine.size(); ++i, ++j)
        {
            if(CommandLine[i][0] == ')')
            {
                if(j == columns.size() - 1)
                {
                    std::cout << "There is Not enough Input Info\n";
                    return;
                }

                input_info.push_back(tmp);
                tmp.clear();
                j = -1;
                if (++i < CommandLine.size() && CommandLine[i][0] != '(')
                {
                    std::cout << "Wrong Format of INSERT_INTO.(2)\n";
                    return;
                }
                else
                {
                    continue;
                }
            }
            else if(j >= columns.size())
            {
                std::cout << "Wrong Format of INSERT_INTO (Input values are more then columns).\n";
                return;
            }

            tmp.push_back(remove_if(CommandLine[i], ','));

        }
    }

    std::vector<std::string> UnTochedFields = make_vector(CommandLine[1] + ".tb");

    for (int i = 0; i < UnTochedFields.size(); i++)
    {
        UnTochedFields[i] = take_field_name(UnTochedFields[i]);
    }
    

    for (int i = 0; i < columns.size(); ++i)
    {
        for (int j = 0; j < UnTochedFields.size(); ++j)
        {
            if(columns[i] == UnTochedFields[j])
            {
                UnTochedFields.erase(UnTochedFields.begin() + j);
                --j;
            }
        }
    }
    

    for (int i = 0; i < columns.size(); i++)
    {
        std::vector<std::string> tmp;
        for (int j = 0; j < input_info.size(); j++)
        {
            tmp.push_back(input_info[j][i]);
        }

        DATABASE[DataBaseName][CommandLine[1]][columns[i]].second = tmp;
        add_data(columns[i] + ".field", tmp);
    }

    for(int i = 0; i < UnTochedFields.size(); ++i)
    {
        add_data(UnTochedFields[i] + ".field", std::vector<std::string>(input_info[0][0].size(), "NONE"));
    }
    


    // INSERT_INTO table_name ( column1, column2, column3 ) VALUES ( value1, value2, value3 ), ( value1, value2, value3 ), ( value1, value2, value3 )
}

bool SQL_Clone::where(std::vector<std::string>& CommandLine, std::string& TableName, int number_data)
{
    int i = find_pos(CommandLine, "WHERE") + 1;
    std::vector<bool>  expressions;
    std::vector<std::string>  logic_operators;
    if(i != 0)
    {
        while(i < CommandLine.size())
        {
            if(CommandLine[i] == "OR" || CommandLine[i] == "AND")
            {
                logic_operators.push_back(CommandLine[i]);
            }
            else
            {
                expressions.push_back(where_token_to_bool(CommandLine[i], TableName, number_data));
            }
            ++i;
        }

        return resolve_expressions(expressions, logic_operators);
    }
    else
    {
        return true;
    }

    //WHERE expression1 AND/OR expression2 ... expression = field=value
}

void SQL_Clone::select(std::vector<std::string>& CommandLine)
{
    if(DataBaseName == "")
    {
        std::cout << "DataBase not entered.\n";
        return;
    }

    std::string TableName;

    for (int i = 0; i < CommandLine.size(); ++i)
    {
        if(CommandLine[i] == "FROM")
        {
            TableName = remove_if(CommandLine[i+1], ';');
            if(!DATABASE[DataBaseName].contains(TableName))
            {
                std::cout << "There is no Table with name " << remove_if(CommandLine[CommandLine.size() - 1], ';') << ".\n";
                return;
            }
            break;
        }
        else if( i == CommandLine.size() - 1)
        {
            std::cout << "Wrong Format of SELECT, no FROM keyword.\n";
            return;
        }
    }
    
    if(CommandLine[1] == "*")
    {
        std::vector<std::string> table_fields = make_vector(TableName + ".tb");

        for (int i = 0; i < table_fields.size(); ++i)
        {
            table_fields[i] = take_field_name(table_fields[i]);
        }
        
        std::map<std::string, std::vector<std::string>> fields_value;

        for(int i = 0; i < table_fields.size(); ++i)
        {
            fields_value[table_fields[i]] = make_vector(table_fields[i] + ".field");
        }
        
        for (int i = 0; i < fields_value[table_fields[0]].size(); i++)
        {

            if(where(CommandLine, TableName, i))
            {
                for (int j = 0; j < table_fields.size(); ++j)
                {
                   std::cout << '|' << fields_value[table_fields[j]][i];
                }
                std::cout << '|' << std::endl;
            }
            {
                continue;
            }
            std::cout << "End of Select" << std::endl;
            
        }
        

    }
    else 
    {
        std::vector<std::string> table_fields;

        for(int i = 1; CommandLine[i] != "FROM"; ++i)
        {
            if(!DATABASE[DataBaseName][TableName].contains(table_fields.back()))
            {
                std::cout << "There is no " << remove_if(CommandLine[i], ',') << " in Table with " << TableName << " name.\n";
                return;
            }

            table_fields.push_back(remove_if(CommandLine[i], ','));
        }

        std::map<std::string, std::vector<std::string>> fields_value;

        for(int i = 0; i < table_fields.size(); ++i)
        {
            std::cout << '|' << table_fields[i];
            fields_value[table_fields[i]] = make_vector(table_fields[i] + ".field");
        } 
        std::cout << '|' << std::endl;

        for (int i = 0; i < fields_value[table_fields[0]].size(); i++)
        {

            if(where(CommandLine, TableName, i))
            {
                for (int j = 0; j < table_fields.size(); ++j)
                {
                   std::cout << '|' << fields_value[table_fields[j]][i];
                }
            }
            {
                continue;
            }
            
            std::cout << '|' << std::endl;
        }

    }
    
    // SELECT column1, column2 FROM table_name WHERE ...; | SELECT * FROM table_name WHERE ...;
}

void SQL_Clone::update(std::vector<std::string>& CommandLine)
{
    if(DataBaseName == "")
    {
        std::cout << "DataBase not entered.\n";
        return;
    }

    if(!DATABASE[DataBaseName].contains(CommandLine[1]))
    {
        std::cout << "Table with name " << CommandLine[1] << " is not exists.\n";
        return;
    }
    
    if(CommandLine[2] != "SET")
    {
        std::cout << "Wrong Format of Command of UPDATE command(No SET part).\n";
        return;
    }
    
    if(find_pos(CommandLine, "WHERE") == UINT_MAX)
    {
        std::cout << "Wrong Format of Command of UPDATE command(No condition).\n";
        return;
    }

    std::vector<process_expression> set_expression; 

    for(int i = 3; CommandLine[i] != "WHERE"; ++i)
    {
        set_expression.push_back(string_to_expression(remove_if(CommandLine[i], ',')));

        if(!DATABASE[DataBaseName][CommandLine[1]].contains(set_expression.back().field_name))
        {
            std::cout << "There is no " << set_expression.back().field_name << " in Table " << CommandLine[1] << ".\n";
            return;
        }

        if(( set_expression.back().operator_ != where_operator::equal))
        {
            std::cout << "SET expression can't be another operation else '='\n";
            return;
        }

        if(!check_type(DATABASE[DataBaseName][CommandLine[1]][set_expression.back().field_name].first, set_expression.back().value))
        {
            return;
        }
    }

    bool is_updated = 0;

    for(int i = 0; i < DATABASE[DataBaseName][CommandLine[1]][set_expression.back().field_name].second.size(); ++i)
    {
        if(where(CommandLine, CommandLine[1], i))
        {
            is_updated = 1;
            for(int j = 0; j < set_expression.size(); ++j)
            {
                DATABASE[DataBaseName][CommandLine[1]][set_expression[j].field_name].second[i] = set_expression[j].value;
                
            }
        }
    }

    if(is_updated)
    {
        for(int i = 0; i < set_expression.size(); ++i)
        {
            update_data(set_expression[i].field_name + ".field", DATABASE[DataBaseName][CommandLine[1]][set_expression[i].field_name].second);
        }
    }

    // UPDATE table_name SET column1=value1, column2=value2, ... WHERE condition;
}

void SQL_Clone::delete_data(std::vector<std::string>& CommandLine)
{

    if(DataBaseName == "")
    {
        std::cout << "DataBase not entered.\n";
        return;
    }

    if(!DATABASE[DataBaseName].contains(CommandLine[2]))
    {
        std::cout << "Table with name " << CommandLine[2] << " is not exists.\n";
        return;
    }
    
    if(find_pos(CommandLine, "WHERE") == UINT_MAX)
    {
        std::cout << "Wrong Format of Command of DELETE command(No condition).\n";
        return;
    }

    bool is_deleted = 0;

    std::vector<std::string> fields_name = make_vector(CommandLine[2] + ".tb");

    for(int i = 0; i < fields_name.size(); ++i)
    {
        fields_name[i] = take_field_name(fields_name[i]);
    }

    for (int i = 0; i < DATABASE[DataBaseName][CommandLine[2]][fields_name.front()].second.size(); ++i)
    {
        if(where(CommandLine, CommandLine[2], i))
        {
            is_deleted = 1;
            for(int j = 0; j < fields_name.size(); ++j)
            {
                DATABASE[DataBaseName][CommandLine[2]][fields_name[j]].second.erase(DATABASE[DataBaseName][CommandLine[2]][fields_name[j]].second.begin() + i);
            }
            --i;
        }
    }

    if(is_deleted)
    {
        for(int i = 0; i < fields_name.size(); ++i)
        {
            update_data(fields_name[i] + ".field", DATABASE[DataBaseName][CommandLine[2]][fields_name[i]].second);
        }
    }
    


    // DELETE FROM table_name WHERE condition;
}

void SQL_Clone::open_db(std::vector<std::string>& CommandLine)
{

    if(CommandLine[1] != "DATABASE")
    {
        std::cout << "Wrong format of command OPEN(No Keyword DATABASE)\n";
        return;
    }

    std::ifstream file;

    CommandLine[2] = remove_if(CommandLine[2], ';');
    file.open(CommandLine[2] + ".data");

    if(!file.is_open())
    {
        std::cout << "No such Database.\n";
        return;
    }

    DataBaseName = CommandLine[2] + ".data";

    std::vector<std::string> Tables = make_vector(DataBaseName);
    std::map<std::string, std::vector<std::string>> FieldsName;
    std::map<std::string, std::vector<std::string>> FieldsType;

    for(int i = 0; i < Tables.size(); ++i)
    {
        FieldsType[Tables[i]] = make_vector(Tables[i] + ".tb");
        FieldsName[Tables[i]] = FieldsType[Tables[i]];
        for (int j = 0; j < FieldsType[Tables[i]].size(); j++)
        {
            FieldsType[Tables[i]][j] = split(FieldsType[Tables[i]][j], " ")[0];
            FieldsName[Tables[i]][j] = split(FieldsName[Tables[i]][j], " ")[1];
        }     
    }

    std::map<std::string, std::map<std::string, std::map<std::string, std::pair<Types, std::vector<std::string>>>>> FieldsInfo;

    for(int i = 0; i < Tables.size(); ++i)
    {
        for(int j = 0; j < FieldsName[Tables[i]].size(); ++j)
        {
            FieldsInfo[DataBaseName][Tables[i]][FieldsName[Tables[i]][j]] = std::make_pair(TypesParse[FieldsType[Tables[i]][j]], make_vector(FieldsName[Tables[i]][j] + ".field"));
        }
    }

    DATABASE = FieldsInfo;

    // OPEN DATABASE database_name;
}

void SQL_Clone::drop_database(std::vector<std::string>& CommandLine)
{
    std::string db_name = remove_if(CommandLine[2]+".data", ';');
    std::vector<std::string> Tables = make_vector(db_name);
    std::remove(db_name.c_str());
    std::vector<std::vector<std::string>> Fields;
    for(int i = 0; i < Tables.size(); ++i)
    {
        Fields.push_back(make_vector(Tables[i] + ".tb"));
        std::remove((Tables[i]+".tb").c_str());
        for(int j = 0; j < Fields[i].size(); ++j)
        {
            std::remove((take_field_name(Fields[i][j])+".field").c_str());
        }
    }

    // DROP DATABASE databasename;
}

void SQL_Clone::drop_table(std::vector<std::string>& CommandLine)
{
    std::string table_name = remove_if(CommandLine[2], ';');
    if(!DATABASE[DataBaseName].contains(table_name))
    {
        std::cout << "There is Table with name " << table_name << " in Database.\n";
        return;
    }
    std::vector<std::string> Fields = make_vector(table_name+".tb");
    std::remove((table_name+".tb").c_str());
    for(int i = 0; i < Fields.size(); ++i)
    {
        std::remove((take_field_name(Fields[i])+".field").c_str());
    }

    // DROP TABLE table_name;
}

void SQL_Clone::drop(std::vector<std::string>& CommandLine)
{

    switch (CommandParse[CommandLine[1]])
    {
    case Command::DATABASE:
    
        drop_database(CommandLine);
        break;

    case Command::TABLE:
    
        drop_table(CommandLine);
        break;
    
    default:
        break;
    }



    // DROP DATABASE databasename;
    // DROP TABLE tablename;
}

void SQL_Clone::complete_command(std::string Command)
{

    std::vector<std::string> CommandLine = split(Command, " ");
    switch (CommandParse[CommandLine[0]])
    {
        case Command::CREATE:

            create(CommandLine);
            break;

        case Command::INSERTINTO:
            insert_into(CommandLine);
            break;

        case Command::SELECT:

            select(CommandLine);
            break;
        
        case Command::UPDATE:

            update(CommandLine);
            break;
        
        case Command::DELETE:

            delete_data(CommandLine);
            break;

        case Command::OPENDATABASE:

            open_db(CommandLine);
            break;

        case Command::DROP:

            drop(CommandLine);
            break;

        default:
            break;
    }
}

void SQL_Clone::execute()
{
    std::string comm;
    while(comm != "Exit")
    {    
        getline(std::cin, comm);
    }

    push_parse_commands();
    push_parse_types();
    push_parse_where_operator();

    complete_command(comm);
}