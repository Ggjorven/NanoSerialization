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
		class ChildIterator
		{
		public:
			// Compliance with std standard.
			using iterator_concept = std::input_iterator_tag;
			using iterator_category = std::forward_iterator_tag;
			using value_type = Node;
			using difference_type = std::ptrdiff_t;
			using pointer = Node*;
			using const_pointer = const Node*;
			using reference = Node&;
			using const_reference = const Node&;
		public:
			// Constructor & Destructor
			ChildIterator(ryml::NodeRef base, ryml::id_type current);
			~ChildIterator() = default;

			// Iterators
			const ChildIterator& operator ++ () const;
			void operator ++ (int) const;

			bool operator == (const ChildIterator& other) const { return (m_CurrentID == other.m_CurrentID); }
			bool operator != (const ChildIterator& other) const { return !(*this == other); }

			// Operator
			Node operator * () const;

		private:
			mutable ryml::NodeRef m_BaseNode;
			mutable ryml::id_type m_CurrentID = ryml::NONE;
		};
	public:
		// Constructors & Destructor
		Node(const ryml::NodeRef& node);
		~Node() = default;
	
		// Methods
		Node Parent();

		// Operators
		const Node operator [] (const char* key) const;
		const Node operator [] (std::string_view key) const;
		const Node operator [] (const std::string& key) const;

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

		inline size_t NumOfChildren() const { return m_Node.num_children(); }

		// Iterators
		ChildIterator begin();
		ChildIterator end();
		const ChildIterator cbegin() const;
		const ChildIterator cend() const;

	protected:
		mutable ryml::NodeRef m_Node;

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

	template<typename T>
	struct Serializer<std::vector<T>>
	{
	public:
		inline static void Serialize(Node& node, const std::vector<T>& value)
		{
			auto sequence = node.Parent();

			sequence.m_Node |= ryml::FLOW_SL;

			// Hacky way // FUTURE TODO: ...
			sequence.m_Node.remove_child(node.m_Node);

			for (const auto& val : value)
			{
				Node arrayElementNode = Node(sequence.m_Node.append_child());
				Serializer<T>::Serialize(arrayElementNode, val);
			}
		}

		inline static std::optional<std::vector<T>> Deserialize(const Node& node)
		{
			if (!node.m_Node.has_children())
				return {};

			std::vector<T> vec;
			vec.reserve(node.m_Node.num_children());

			for (const auto& child : node.m_Node.children())
			{
				Node childNode = Node(child);
				vec.emplace_back(Serializer<T>::Deserialize(childNode).value());
			}

			return vec;
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