#include "nspch.h"
#include "File.hpp"

#include <fstream>
#include <sstream>

namespace Nano::Serialization::Yaml
{

	////////////////////////////////////////////////////////////////////////////////////
	// Constructors & Destructor
	////////////////////////////////////////////////////////////////////////////////////
	File::File()
		: Node(m_Tree.rootref()), m_Source({}), m_Tree({})
	{
		// Note: Reinit so it actually points to the new empty tree
		m_Node = m_Tree.rootref();
	}

	File::File(const std::filesystem::path& path)
		: Node(m_Tree.rootref()), m_Source({}), m_Tree({})
	{
		std::ifstream file(path);
		
		std::stringstream ss;
		ss << file.rdbuf();
		
		m_Source = ss.str();
		m_Tree = ryml::parse_in_arena(ryml::to_csubstr(m_Source.c_str()));

		// Note: Reinit so it actually points to the new parsed tree
		m_Node = m_Tree.rootref();
	}

	File::File(std::string_view input)
		: Node(m_Tree.rootref()), m_Source(input), m_Tree(ryml::parse_in_arena(ryml::to_csubstr(m_Source.c_str())))
	{
		// Note: Reinit so it actually points to the new parsed tree
		m_Node = m_Tree.rootref();
	}

	File::~File()
	{
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Extraction
	////////////////////////////////////////////////////////////////////////////////////
	std::string File::AsString() const
	{
		std::string outYaml;
		ryml::emitrs_yaml<std::string>(m_Tree.rootref(), &outYaml);
		return outYaml;
	}

}
