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
    
    if (false)
    {
        Yaml::File file(yaml);

        std::string name = file["name"sv].As<std::string>().value();
        int32_t age = file["age"sv].As<int32_t>().value();
        double height = file["height"sv].As<double>().value();
        bool active = file["active"sv].As<bool>().value();

        std::cout << "Name: " << name << "\nAge: " << age << "\nHeight: " << height << "\nActive: " << std::boolalpha << active << "\n";
    }

    {
        ryml::Tree tree;
        auto root = tree.rootref();
        root |= ryml::NodeType_e::MAP;

        root.append_child() << ryml::key("name") << "John";
        root.append_child() << ryml::key("age") << "30";
        root.append_child() << ryml::key("admin") << "true";

        ryml::NodeRef address = root.append_child();
        address << ryml::key("address");
        address |= ryml::NodeType_e::MAP;

        address.append_child() << ryml::key("city") << "New York";
        address.append_child() << ryml::key("zip") << "10001";

        ryml::NodeRef list = root.append_child();
        list << ryml::key("list");
        list |= ryml::NodeType_e::SEQ;

        list.append_child() << "Val1";
        list.append_child() << "Val2";

        //auto n = root.append_child(ryml::NodeInit(ryml::NodeType(ryml::NodeType_e::KEYVAL), ryml::NodeScalar("name", "Bob")/*, ryml::NodeScalar("Bob")*/));
        //n.set_key("name");
        //n.set_val("Bob");
        
        //root["name"] = "Bob";
        //root["age"] = "23";
        //root["height"] = "1";
        //root["active"] = "false";

        std::string out_yaml;
        ryml::emitrs_yaml<std::string>(&tree, &out_yaml);

        std::cout << "--- Serialized YAML ---\n" << out_yaml;
    }

    {
        Yaml::File file;

        file << Yaml::NodeType::Key << "Name" << Yaml::NodeType::Value << "John";
        file << Yaml::NodeType::Key << "Age" << Yaml::NodeType::Value << 32;
        file << Yaml::NodeType::Key << "Height" << Yaml::NodeType::Value << 1.71;
        file << Yaml::NodeType::Key << "Active" << Yaml::NodeType::Value << true;

        auto seq = file << Yaml::NodeType::Sequence << "Sequence";
        seq << Yaml::NodeType::Value << "Val1";
        seq << Yaml::NodeType::Value << "Val2";

        std::cout << "--- Serialized YAML ---\n" << file.AsString();
    }

    return 0;
}