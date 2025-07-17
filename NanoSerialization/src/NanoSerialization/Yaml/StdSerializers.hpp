#pragma once

#include "NanoSerialization/Core/Core.hpp"

#include "NanoSerialization/Yaml/Node.hpp"

namespace Nano::Serialization::Yaml
{

	////////////////////////////////////////////////////////////////////////////////////
	// Serializers
	////////////////////////////////////////////////////////////////////////////////////
	template<>
	struct Serializer<const char*>
	{
	public:
		inline static Node Serialize(Node& node, std::string_view key, const std::string& value)
		{
			// TODO: ...

			return node[key];
		}

		// Note: Only implements default types: bool, integers, floats, strings
		inline static std::optional<const char*> Deserialize(const Node& node)
		{
			if (!node.HasValue())
				return {};

			NS_LOG_WARN("It is not recommended to retrieve a const char* since it has no '\\0' character.");
			return node.m_Node.val().data();
		}
	};

	template<>
	struct Serializer<std::string_view>
	{
	public:
		inline static Node Serialize(Node& node, std::string_view key, const std::string& value)
		{
			// TODO: ...

			return node[key];
		}

		// Note: Only implements default types: bool, integers, floats, strings
		inline static std::optional<std::string_view> Deserialize(const Node& node)
		{
			if (!node.HasValue())
				return {};

			return std::string_view(node.m_Node.val().data(), node.m_Node.val().size());
		}
	};

}