#include "nspch.h"
#include "Node.hpp"

#include "NanoSerialization/Core/Core.hpp"

namespace Nano::Serialization::Yaml
{

	////////////////////////////////////////////////////////////////////////////////////
	// Constructors & Destructor
	////////////////////////////////////////////////////////////////////////////////////
	Node::Node(const ryml::NodeRef& node)
		: m_Node(node)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Methods
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////////////////////////////
	Node Node::operator [] (const char* key)
	{
		return this->operator [] (std::string_view(key));
	}

	Node Node::operator [] (std::string_view key)
	{
		return Node(m_Node.at(ryml::to_csubstr(key.data())));
	}

	Node Node::operator[](const std::string& key)
	{
		return this->operator [] (std::string_view(key));
	}

	Node Node::operator << (NodeType type)
	{
		switch (type)
		{
		case NodeType::Key:
		{
			Node newNode = Node(m_Node.append_child());
			newNode.m_Node |= ryml::KEY;
			return newNode;
		}
		case NodeType::Value:
		{
			if ((m_Node.get()->m_type & ryml::KEY) && !(m_Node.get()->m_type & ryml::MAP) && !(m_Node.get()->m_type & ryml::SEQ))
			{
				m_Node |= ryml::VAL;
				return *this;
			}
			else
			{
				Node newNode = Node(m_Node.append_child());
				newNode.m_Node |= ryml::VAL;
				return newNode;
			}
		}

		case NodeType::Sequence:
		{
			Node newNode = Node(m_Node.append_child());
			newNode.m_Node |= ryml::SEQ;
			return newNode;
		}

		case NodeType::Map:
		{
			Node newNode = Node(m_Node.append_child());
			newNode.m_Node |= ryml::MAP;
			return newNode;
		}

		default:
			NS_UNREACHABLE();
			break;
		}

		return *this;
	}

	Node Node::operator << (const char* key)
	{
		return this->operator << (std::string(key));
	}

	Node Node::operator << (std::string_view key)
	{
		return this->operator << (std::string(key));
	}

	Node Node::operator << (const std::string& key)
	{
		if ((m_Node.get()->m_type & ryml::VAL))
			return this->template operator << <std::string> (std::string(key));

		auto rymlKey = ryml::key(key);
		m_Node << rymlKey;

		return *this;
	}

}