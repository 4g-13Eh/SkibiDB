#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Table.hpp"
#include "Attribute.hpp"
#include "termcolor.hpp"
#include "utilities.hpp"

Table::Table(const std::string &name, const std::vector<Attribute> &attributes)
{
    this->name = name;
    this->attributes = attributes;
    this->numRows = 0;
}

Table::Table(const std::string &name, const std::vector<Attribute> &attributes, const std::vector<std::map<std::string, std::string>> &data)
{
    this->name = name;
    this->attributes = attributes;
    this->numRows = data.size() > 0 ? data.size() : 0;
    this->data = data;
}

std::string Table::getTableName() const
{
    return name;
}

std::vector<Attribute> Table::getAttributes() const
{
    return attributes;
}

int Table::getNumRows() const
{
    return numRows;
}

std::vector<std::map<std::string, std::string>> Table::getData() const
{
    return data;
}

void Table::addAttribute(const Attribute &attribute)
{
    attributes.push_back(attribute);
}

void Table::removeAttribute(const std::string &name)
{
    for (auto it = attributes.begin(); it != attributes.end(); ++it)
    {
        if (it->getAttributeName() == name)
        {
            attributes.erase(it);
            return;
        }
    }
}

Attribute Table::getAttribute(const std::string &name) const
{
    for (const auto &attribute : attributes)
    {
        if (attribute.getAttributeName() == name)
        {
            return attribute;
        }
    }
    std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Attribute not found" << std::endl;
    exit(1);
}

Attribute Table::getAttribute(int index) const
{
    if (index < 0 || index >= attributes.size())
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid index" << std::endl;
    }
    return attributes[index];
}

std::string Table::toString() const
{
    nlohmann::json j;
    j["name"] = name;
    j["attributes"] = nlohmann::json::array();
    j["numRows"] = numRows;
    j["data"] = nlohmann::json::array();
    for (const auto &attribute : attributes)
    {
        nlohmann::json attr;
        attr["name"] = attribute.getAttributeName();
        attr["type"] = attribute.getAttributeType();
        j["attributes"].push_back(attr);
    }

    // Add data
    for (const auto &row : this->data)
    {
        nlohmann::json r;
        for (const auto &pair : row)
        {
            r[pair.first] = pair.second;
        }
        j["data"].push_back(r);
    }

    return j.dump(4);
}

void Table::addRow(const std::vector<std::string> &attributes, const std::vector<std::string> &values)
{
    // Check length of vectors, if they are not equal, return
    if (attributes.size() != values.size())
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Attributes and values do not match" << std::endl;
        return;
    }

    std::map<std::string, std::string> row;
    for (size_t i = 0; i < attributes.size(); i++)
    {
        row[attributes[i]] = values[i];
        row["__id__"] = generateUUID();
        row["__row__"] = std::to_string(this->numRows + 1);
        row["__created_at__"] = getCurrentTimestamp();
    }

    this->data.push_back(row);
    this->numRows++;
}

std::map<std::string, std::string> Table::getRow(int index) const
{
    if (index < 0 || index >= numRows)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid index" << std::endl;
        // Return an empty map or throw an exception
        return std::map<std::string, std::string>(); // Empty map
    }
    return data[index];
}

std::vector<std::map<std::string, std::string>> Table::getRows() const
{
    return data;
}

std::vector<std::map<std::string, std::string>> Table::select(const std::vector<std::string> &columns, const std::vector<std::string> &conditionTokens) const
{
    std::vector<std::map<std::string, std::string>> result;

    // Ensure conditionTokens has at least three elements
    if (conditionTokens.size() < 3)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid condition format" << std::endl;
        return result;
    }

    // Extract condition components
    const std::string &attribute = conditionTokens[0];
    const std::string &op = conditionTokens[1];
    const std::string &valueConst = conditionTokens[2];

    std::string value = valueConst;

    // Remove double quotes from value
    if (value.front() == '"')
    {
        value.erase(0, 1);
    }

    if (value.back() == '"')
    {
        value.pop_back();
    }

    // Validate operator
    bool validOperator = (op == "==" || op == "!=" || op == ">" || op == "<" || op == ">=" || op == "<=" || op == "LIKE");
    if (!validOperator)
    {
        std::cerr << termcolor::red << "[ERROR] " << termcolor::reset << "Invalid operator: " << op << std::endl;
        return result;
    }

    // Perform search
    for (const auto &row : data)
    {
        bool match = false;

        // Check if attribute exists in row
        if (row.find(attribute) != row.end())
        {
            const std::string &rowValue = row.at(attribute);

            // Perform comparison based on operator
            if (op == "==")
            {
                match = (strcasecmp(rowValue.c_str(), value.c_str()) == 0);
            }
            else if (op == "!=")
            {
                match = (rowValue != value);
            }
            else if (op == ">")
            {
                match = (std::stoi(rowValue) > std::stoi(value));
            }
            else if (op == "<")
            {
                match = (std::stoi(rowValue) < std::stoi(value));
            }
            else if (op == ">=")
            {
                match = (std::stoi(rowValue) >= std::stoi(value));
            }
            else if (op == "<=")
            {
                match = (std::stoi(rowValue) <= std::stoi(value));
            }
            else if (op == "LIKE")
            {
                match = (toLowerCase(rowValue).find(toLowerCase(value)) != std::string::npos);
            }
        }

        // Add row to result if it matches the condition
        if (match)
        {
            std::cout << termcolor::green << "[INFO] " << termcolor::reset << "Match found" << std::endl;
            result.push_back(row);
        }
    }

    return result;
}
