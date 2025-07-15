#pragma once

#include "NanoSerialization/Toml/Toml.hpp"

#include <optional>
#include <string_view>

namespace Nano::Serialization::Toml
{

	template<typename T>
	struct Serializer;

	////////////////////////////////////////////////////////////////////////////////////
	// Node
	////////////////////////////////////////////////////////////////////////////////////
	class Node // Note: A toml file doesn't exclusively mean a filesystem file, it just means a collection of TOML data
	{
	public:
		// Constructors & Destructor
		Node(toml::v3::node& node);
		Node(toml::v3::node_view<toml::v3::node> node);
		~Node() = default;

		// Methods
		Node At(size_t index) const;
		Node At(std::string_view input) const;

		// Operators
		Node operator [] (size_t index) const;
		Node operator [] (std::string_view input) const;

		// Extraction
		template<typename T>
		std::optional<T> As() const;
	
	private:
		toml::node_view<toml::node> m_Node;

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
		inline static void Serialize()
		{
			// TODO: ...
		}

		inline static std::optional<T> Deserialize(const Node& node)
		{
			return node.m_Node.value<T>();
		}
	};

	////////////////////////////////////////////////////////////////////////////////////
	// Templated functions
	////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	std::optional<T> Node::As() const
	{
		static_assert(std::is_same_v<std::optional<T>, toml::optional<T>>, "[NanoSerialization] Toml optional doesn't match std::optional, this is required per the current implementation.");
		return Serializer<T>::Deserialize(*this);
	}

}