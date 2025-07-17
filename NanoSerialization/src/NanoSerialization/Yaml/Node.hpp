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

		Sequence, 
		Map
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
		Node operator [] (const char* key);
		Node operator [] (std::string_view key);
		Node operator [] (const std::string& key);

		Node operator << (NodeType type);
		Node operator << (const char* key);
		Node operator << (std::string_view key);
		Node operator << (const std::string& key);

		template<typename T>
		Node operator << (const T& value);

		// Extraction
		template<typename T>
		std::optional<T> As() const;

		// Helpers
		inline bool HasValue() const { return m_Node.has_val(); }
		inline bool HasChild(std::string_view child) const { return m_Node.has_child(ryml::to_csubstr(child.data())); }

		// Iterators
		// TODO: ...

	protected:
		ryml::NodeRef m_Node;

		template<typename T>
		friend struct Serializer;
	};

	////////////////////////////////////////////////////////////////////////////////////
	// Custom type serializer
	////////////////////////////////////////////////////////////////////////////////////
	// Note: Only implements default types: integers, floats 
	template<typename T>
	struct Serializer
	{
	public:
		inline static void Serialize(Node& node, const T& value) requires(
			std::is_integral_v<std::decay_t<T>> ||
			std::is_floating_point_v<std::decay_t<T>>
		)
		{
			node.m_Node << value;
		}

		inline static std::optional<T> Deserialize(const Node& node) requires(
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

	template<>
	struct Serializer<bool>
	{
	public:
		inline static void Serialize(Node& node, bool value)
		{
			std::stringstream ss;
			ss << std::boolalpha << value;

			node.m_Node << ss.str().c_str();
		}

		inline static std::optional<bool> Deserialize(const Node& node)
		{
			if (!node.HasValue())
				return {};

			bool out;
			node.m_Node >> out;
			return out;
		}
	};

	template<>
	struct Serializer<std::string>
	{
	public:
		inline static void Serialize(Node& node, const std::string& value)
		{
			node.m_Node << value.c_str();
		}

		inline static std::optional<std::string> Deserialize(const Node& node) 
		{
			if (!node.HasValue())
				return {};

			return std::string(node.m_Node.val().data(), node.m_Node.val().size());
		}
	};

	////////////////////////////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	Node Node::operator << (const T& value)
	{
		Serializer<T>::Serialize(*this, value);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Extraction
	////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	std::optional<T> Node::As() const
	{
		return Serializer<T>::Deserialize(*this);
	}

}