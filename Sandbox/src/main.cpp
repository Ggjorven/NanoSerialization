#include <iostream>

#include <toml++/toml.hpp>

using namespace std::literals;

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    auto config = toml::parse_file("test.toml");

    std::string_view libraryName = config["library"]["name"].value_or(""sv);
    std::string_view libraryAuthor = config["library"]["authors"][0].value_or(""sv);
    int64_t cppVersion = config["dependencies"]["cpp"].value_or(0);

    std::cout << "Library Name: " << libraryName << std::endl;
    std::cout << "Library Author: " << libraryAuthor << std::endl;
    std::cout << "Cpp Version: " << cppVersion << std::endl;

    std::cout << "\n\nTOML FILE:\n" << config << "\n";
    std::cout << "\n\nJSON FILE:\n" << toml::json_formatter{ config } << "\n";
    std::cout << "\n\nYAML FILE:\n" << toml::yaml_formatter{ config } << "\n";
}