#include "nspch.h"
#include "Node.hpp"

#include "NanoSerialization/Core/Core.hpp"

namespace Nano::Serialization::Toml
{

	////////////////////////////////////////////////////////////////////////////////////
	// Constructors & Destructor
	////////////////////////////////////////////////////////////////////////////////////
	Node::Node(toml::node& node)
		: m_Node(&node)
	{
	}

	Node::Node(toml::node_view<toml::node> node)
		: m_Node(node)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Methods
	////////////////////////////////////////////////////////////////////////////////////
	Node Node::At(size_t index) const
	{
		return m_Node[index];
	}
	
	Node Node::At(std::string_view input) const
	{
		return m_Node[input];
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////////////////////////////
	Node Node::operator [] (size_t index) const
	{
		return At(index);
	}

	Node Node::operator [] (std::string_view input) const
	{
		return At(input);
	}

	Node Node::operator << (NodeType type)
	{
		m_LatestType = type;
		return *this;
	}

	Node Node::operator << (std::string_view value)
	{
		if (m_LatestType == NodeType::Value)
			return this->template operator << <std::string_view> (value);

		if (m_LatestType == NodeType::Key)
		{

		}
		else if (m_LatestType == NodeType::BeginList)
		{

		}
		else if (m_LatestType == NodeType::BeginTable)
		{

		}

		NS_UNREACHABLE();
		return *this;
	}

}