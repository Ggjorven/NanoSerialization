#pragma once

#include "NanoSerialization/Toml/Toml.hpp"

#include <optional>
#include <string>
#include <string_view>

namespace Nano::Serialization::Toml
{

	template<typename T>
	struct Serializer;

	////////////////////////////////////////////////////////////////////////////////////
	// NodeType
	////////////////////////////////////////////////////////////////////////////////////
	enum class NodeType : uint8_t
	{
		None = 0, 

		Key, 
		Value, 

		BeginList, 
		EndList, 

		BeginTable,
		EndTable
	};

	////////////////////////////////////////////////////////////////////////////////////
	// Node
	////////////////////////////////////////////////////////////////////////////////////
	class Node // Note: A toml file doesn't exclusively mean a filesystem file, it just means a collection of TOML data
	{
	public:
		inline constexpr static size_t s_MaxInsertTreeSize = 4096ull;
	public:
		// Constructors & Destructor
		Node(toml::node& node);
		Node(toml::node_view<toml::v3::node> node);
		~Node() = default;

		// Methods
		Node At(size_t index) const;
		Node At(std::string_view input) const;

		// Operators
		Node operator [] (size_t index) const;
		Node operator [] (std::string_view input) const;

		Node operator << (NodeType type);
		Node operator << (std::string_view name);
		template<typename T>
		Node operator << (const T& value);

		// Extraction
		template<typename T>
		std::optional<T> As() const;
	
	private:
		toml::node_view<toml::node> m_Node;

		// Insert
		NodeType m_LatestType = NodeType::None;

		template<typename T>
		friend struct Serializer;
	};

	////////////////////////////////////////////////////////////////////////////////////
	// Custom Serializer
	////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct Serializer
	{
	public:
		inline static Node Serialize(Node& node, std::string_view key, T&& value)
		{
			// TODO: ...
			//node.m_Node.ref<toml::table>().insert_or_assign(key, toml::table{ { key, std::forward<T>(value) } });
			//node.m_Node.node()->operator[]

			return node.m_Node[key];
		}

		inline static std::optional<T> Deserialize(const Node& node)
		{
			return node.m_Node.value<T>();
		}
	};
	
	////////////////////////////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	Node Node::operator << (const T& value)
	{
		//if (!(m_LatestType == NodeType::Value))
		//	// TODO: Asserts
		//	return *this;
	
		// TODO: ...
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Extraction
	////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	std::optional<T> Node::As() const
	{
		static_assert(std::is_same_v<std::optional<T>, toml::optional<T>>, "[NanoSerialization] Toml optional doesn't match std::optional, this is required per the current implementation.");
		return Serializer<T>::Deserialize(*this);
	}

}