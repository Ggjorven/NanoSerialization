#pragma once

#include "NanoSerialization/Toml/Toml.hpp"
#include "NanoSerialization/Toml/Node.hpp"

#include <filesystem>
#include <string_view>

namespace Nano::Serialization::Toml
{

	////////////////////////////////////////////////////////////////////////////////////
	// File
	////////////////////////////////////////////////////////////////////////////////////
	class File : public Node // Note: A toml file doesn't exclusively mean a filesystem file, it just means a collection of TOML data
	{
	public:
		// Constructors & Destructor
		File();
		File(const std::filesystem::path& path);
		File(std::string_view input);
		~File();

		// Methods

		// Extraction
		std::string AsString() const;

	private:
		toml::table m_TomlTable;
	};

}