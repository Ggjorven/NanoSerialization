#include "nspch.h"
#include "Node.hpp"

#include "NanoSerialization/Core/Core.hpp"

namespace Nano::Serialization::Yaml
{

	////////////////////////////////////////////////////////////////////////////////////
	// Constructor & Destructor
	////////////////////////////////////////////////////////////////////////////////////
	Node::ChildIterator::ChildIterator(ryml::NodeRef base, ryml::id_type current)
		: m_BaseNode(base), m_CurrentID(current)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Iterators
	////////////////////////////////////////////////////////////////////////////////////
	const Node::ChildIterator& Node::ChildIterator::operator ++ () const
	{
		m_CurrentID = m_BaseNode.tree()->next_sibling(m_CurrentID);
		return *this;
	}

	void Node::ChildIterator::operator ++ (int) const
	{
		++(*this);
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////////////////////////////
	Node Node::ChildIterator::operator * () const
	{
		return Node(ryml::NodeRef(m_BaseNode.tree(), m_CurrentID));
	}
	
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
	Node Node::Parent()
	{
		return Node(m_Node.parent());
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////////////////////////////
	const Node Node::operator [] (const char* key) const
	{
		return this->operator [] (std::string_view(key));
	}

	const Node Node::operator [] (std::string_view key) const
	{
		return Node(m_Node.at(ryml::to_csubstr(key.data())));
	}

	const Node Node::operator[](const std::string& key) const
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

	////////////////////////////////////////////////////////////////////////////////////
	// Iterators
	////////////////////////////////////////////////////////////////////////////////////
	Node::ChildIterator Node::begin()
	{
		return ChildIterator(m_Node, m_Node.first_child().id());
	}

	Node::ChildIterator Node::end()
	{
		return ChildIterator(m_Node, ryml::NONE);
	}

	const Node::ChildIterator Node::cbegin() const
	{
		return ChildIterator(m_Node, m_Node.last_child().id());
	}

	const Node::ChildIterator Node::cend() const
	{
		return ChildIterator(m_Node, ryml::NONE);
	}

}