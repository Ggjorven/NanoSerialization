#include <iostream>
#include <fstream>

#include "NanoSerialization/Core/Core.hpp"

#include "NanoSerialization/Toml/Toml.hpp"
#include "NanoSerialization/Toml/File.hpp"
#include "NanoSerialization/Toml/Node.hpp"

using namespace Nano::Serialization;

using namespace std::literals;

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    // Input
    {
        Toml::File file(std::filesystem::path("input.toml"));

        std::string_view libraryName = file["library"]["name"].As<std::string_view>().value();
        std::string_view libraryAuthor = file["library"]["authors"][0].As<std::string_view>().value();
        int64_t cppVersion = file["dependencies"]["cpp"].As<int>().value();

        std::cout << "Library Name: " << libraryName << std::endl;
        std::cout << "Library Author: " << libraryAuthor << std::endl;
        std::cout << "Cpp Version: " << cppVersion << std::endl;
    }

    // Output
    {
        Toml::File file;
        Toml::Node& fileNode = file;

        fileNode << Toml::NodeType::Key << "First" << Toml::NodeType::Value << "String";

        std::ofstream out(std::filesystem::path("output.toml"));
        out << file.AsString();
        out.close();
    }
}