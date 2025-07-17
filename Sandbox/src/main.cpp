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

        ryml::NodeRef mapSeq = root.append_child();
        mapSeq << ryml::key("mapSeq");
        mapSeq |= ryml::NodeType_e::SEQ;

        ryml::NodeRef e1 = mapSeq.append_child();
        e1 |= ryml::MAP;

        // Now add children to the map (as key-value pairs)
        e1.append_child() << ryml::key("E1") << "1";

        ryml::NodeRef e2 = mapSeq.append_child();
        e2 |= ryml::MAP;

        e2.append_child() << ryml::key("E2") << "2";

        
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

        auto map = file << Yaml::NodeType::Map << "Map";
        map << Yaml::NodeType::Key << "Key1" << Yaml::NodeType::Value << "Val1";
        map << Yaml::NodeType::Key << "Key2" << Yaml::NodeType::Value << "Val2";

        auto entities = file << Yaml::NodeType::Sequence << "Entities";
        auto eMap = entities << Yaml::NodeType::Map;
        auto e1 = eMap << Yaml::NodeType::Key << "E1" << Yaml::NodeType::Value << "02149028149";
        auto transform = eMap << Yaml::NodeType::Map << "Transform"; 
        transform << Yaml::NodeType::Key << "Position" << Yaml::NodeType::Value << "[1, 1, 1]";
        transform << Yaml::NodeType::Key << "Size" << Yaml::NodeType::Value << "[1, 2, 1]";
        transform << Yaml::NodeType::Key << "Rotation" << Yaml::NodeType::Value << "[1, 3, 1]";

        std::cout << "--- Serialized YAML ---\n" << file.AsString();
    }

    return 0;
}