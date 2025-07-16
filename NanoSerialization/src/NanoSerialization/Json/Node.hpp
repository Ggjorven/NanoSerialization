#pragma once

#include "NanoSerialization/Yaml/Yaml.hpp"

#include <optional>
#include <string>
#include <string_view>

namespace Nano::Serialization::Json
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
		// Constructors & Destructor
		~Node() = default;
	
	private:
	};


}