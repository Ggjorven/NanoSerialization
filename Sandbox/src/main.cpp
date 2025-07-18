#include <iostream>
#include <fstream>

#include "NanoSerialization/Core/Core.hpp"

#include "NanoSerialization/Yaml/File.hpp"
#include "NanoSerialization/Json/File.hpp"

using namespace Nano::Serialization;

template<typename T, size_t N>
struct Nano::Serialization::Yaml::Serializer<std::array<T, N>>
{
public:
    inline static void Serialize(Node& node, const std::array<T, N>& value) 
    {
        auto sequence = node.Parent();
        //auto sequence = node;

        sequence.MakeSingleLine();

        for (const auto& val : value)
        {
            Node arrayElementNode = sequence.AddChild();
            Serializer<T>::Serialize(arrayElementNode, val);
        }
    }

    inline static std::optional<std::array<T, N>> Deserialize(const Node& node)
    {
        if (!node.HasChildren())
            return {};

        std::array<T, N> arr;

        size_t i = 0;
        for (const Node& child : node)
            arr[i++] = Serializer<T>::Deserialize(child).value();

        return arr;
    }
};

template<typename T>
struct Nano::Serialization::Yaml::Serializer<std::vector<T>>
{
public:
    inline static void Serialize(Node& node, const std::vector<T>& value)
    {
        auto sequence = node.Parent();
        //auto sequence = node;

        auto t = sequence.m_Node.type();

        sequence.MakeSingleLine();

        for (const auto& val : value)
        {
            Node arrayElementNode = sequence.AddChild();
            Serializer<T>::Serialize(arrayElementNode, val);
        }
    }

    inline static std::optional<std::vector<T>> Deserialize(const Node& node)
    {
        if (!node.HasChildren())
            return {};

        std::vector<T> vec;
        vec.reserve(node.NumOfChildren());

        for (const Node& child : node)
            vec.emplace_back(Serializer<T>::Deserialize(child).value());

        return vec;
    }
};

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    if (false)
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

        ryml::NodeRef transform = root.append_child();
        transform << ryml::key("transform");
        transform |= ryml::NodeType_e::SEQ;
        transform |= ryml::NodeType_e::FLOW_SL;

        transform.append_child() << "1";
        transform.append_child() << "1";
        transform.append_child() << "1";

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
        auto e1Map = entities << Yaml::NodeType::Map;
        auto e1 = e1Map << Yaml::NodeType::Key << "Entity" << Yaml::NodeType::Value << 2149028149;
        auto transform = e1Map << Yaml::NodeType::Map << "Transform"; 

        auto posSeq = transform << Yaml::NodeType::Sequence << "Position";
        posSeq << Yaml::NodeType::Value << std::vector({ 1, 2, 3, 4, 5, 6, 7 });

        auto sizeSeq = transform << Yaml::NodeType::Sequence << "Size";
        sizeSeq << Yaml::NodeType::Value << std::to_array({ 8, 9, 10, 11, 12, 13, 14 });
        transform << Yaml::NodeType::Key << "Rotation" << Yaml::NodeType::Value << "[1, 3, 1]";

        auto e2Map = entities << Yaml::NodeType::Map;
        auto e2 = e2Map << Yaml::NodeType::Key << "Entity" << Yaml::NodeType::Value << 93842098321;
        auto transform2 = e2Map << Yaml::NodeType::Map << "Transform";
        transform2 << Yaml::NodeType::Key << "Position" << Yaml::NodeType::Value << "[2, 1, 1]";
        transform2 << Yaml::NodeType::Key << "Size" << Yaml::NodeType::Value << "[2, 2, 1]";
        transform2 << Yaml::NodeType::Key << "Rotation" << Yaml::NodeType::Value << "[2, 3, 1]";

        auto e3Map = entities << Yaml::NodeType::Map;
        auto e3 = e3Map << Yaml::NodeType::Key << "Entity" << Yaml::NodeType::Value << 32094870923;
        auto transform3 = e3Map << Yaml::NodeType::Map << "Transform";
        transform3 << Yaml::NodeType::Key << "Position" << Yaml::NodeType::Value << "[2, 1, 1]";
        transform3 << Yaml::NodeType::Key << "Size" << Yaml::NodeType::Value << "[2, 2, 1]";
        transform3 << Yaml::NodeType::Key << "Rotation" << Yaml::NodeType::Value << "[2, 3, 1]";

        std::cout << "--- Serialized YAML ---\n" << file.AsString();

        std::cout << "\n";

        for (const auto& i : posSeq.As<std::vector<int>>().value())
        {
            std::cout << "int: " << i << '\n';
        }

        for (const auto& i : sizeSeq.As<std::array<int, 7>>().value())
        {
            std::cout << "int 2: " << i << '\n';
        }

        for (const auto& entity : entities)
        {
            std::cout << "Entity ID: " << entity["Entity"].As<uint64_t>().value() << '\n';
        }
    }

    return 0;
}