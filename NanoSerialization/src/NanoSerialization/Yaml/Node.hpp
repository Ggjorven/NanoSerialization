#pragma once

#include "NanoSerialization/Yaml/Yaml.hpp"

#include <optional>
#include <string>
#include <string_view>

namespace Nano::Serialization::Yaml
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
	class Node
	{
	public:
		// Constructors & Destructor
		Node(const ryml::NodeRef& node);
		~Node() = default;
	
		// Methods

		// Operators
		Node operator [] (std::string_view key);

		// Extraction
		template<typename T>
		std::optional<T> As() const;

		// Helpers
		inline bool HasValue() const { return m_Node.has_val(); }
		inline bool HasChild(std::string_view child) const { return m_Node.has_child(ryml::to_csubstr(child.data())); }

		// TODO: Children iterators

	protected:
		ryml::NodeRef m_Node;

		template<typename T>
		friend struct Serializer;
	};

	////////////////////////////////////////////////////////////////////////////////////
	// Custom type serializer
	////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct Serializer
	{
	public:
		inline static Node Serialize(Node& node, std::string_view key, const T& value)
		{
			// TODO: ...

			return node[key];
		}

		// Note: Only implements default types: bool, integers, floats 
		inline static std::optional<T> Deserialize(const Node& node) requires(
			std::is_same_v<std::decay_t<T>, bool> ||
			std::is_integral_v<std::decay_t<T>> ||
			std::is_floating_point_v<std::decay_t<T>>
		)
		{
			if (!node.HasValue())
				return {};

			std::decay_t<T> out;
			node.m_Node >> out;
			return out;
		}
	};

	// Note: This is here instead of in StdSerializers.hpp since it is kind of a default type in
	// yaml. 
	template<>
	struct Serializer<std::string>
	{
	public:
		inline static Node Serialize(Node& node, std::string_view key, const std::string& value)
		{
			// TODO: ...

			return node[key];
		}

		// Note: Only implements default types: bool, integers, floats, strings
		inline static std::optional<std::string> Deserialize(const Node& node) 
		{
			if (!node.HasValue())
				return {};

			return std::string(node.m_Node.val().data(), node.m_Node.val().size());
		}
	};

	////////////////////////////////////////////////////////////////////////////////////
	// Extraction
	////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	std::optional<T> Node::As() const
	{
		return Serializer<T>::Deserialize(*this);
	}

}