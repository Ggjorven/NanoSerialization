#include <iostream>
#include <fstream>

#include "NanoSerialization/Core/Core.hpp"

#include "NanoSerialization/Yaml/File.hpp"
#include "NanoSerialization/Json/File.hpp"

using namespace Nano::Serialization;

using namespace std::literals;

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    std::string_view yaml = R"(
name: Alice
age: 30
height: 5.7
active: true)";

    if (false)
    {
        ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(yaml.data()));
        ryml::NodeRef root = tree.rootref();

        std::string name = std::string(root["name"].val().data(), root["name"].val().size());
        int age = 0;

        root["age"] >> age;
        double height = 0.0;
        root["height"] >> height;
        bool active = false;
        root["active"] >> active;

        std::cout << "Name: " << name << "\nAge: " << age << "\nHeight: " << height << "\nActive: " << std::boolalpha << active << "\n";
    }

    {
        Yaml::File file(yaml);

        std::string name = file["name"sv].As<std::string>().value();
        int32_t age = file["age"sv].As<int32_t>().value();
        double height = file["height"sv].As<double>().value();
        bool active = file["active"sv].As<bool>().value();

        std::cout << "Name: " << name << "\nAge: " << age << "\nHeight: " << height << "\nActive: " << std::boolalpha << active << "\n";
    }

    return 0;
}