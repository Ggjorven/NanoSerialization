#include "nspch.h"
#include "File.hpp"

namespace Nano::Serialization::Toml
{

	////////////////////////////////////////////////////////////////////////////////////
	// Constructors & Destructor
	////////////////////////////////////////////////////////////////////////////////////
	File::File()
		: Node(m_TomlTable), m_TomlTable()
	{
	}

	File::File(const std::filesystem::path& path)
		: Node(m_TomlTable), m_TomlTable(toml::parse_file(path.c_str()))
	{
	}

	File::File(std::string_view input)
		: Node(m_TomlTable), m_TomlTable(toml::parse(input))
	{
	}

	File::~File()
	{
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Extraction
	////////////////////////////////////////////////////////////////////////////////////
	std::string File::AsString() const
	{
		std::stringstream ss;
		ss << m_TomlTable;
		return ss.str();
	}

}
