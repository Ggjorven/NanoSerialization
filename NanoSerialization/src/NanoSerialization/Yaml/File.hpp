#pragma once

#include "NanoSerialization/Yaml/Yaml.hpp"
#include "NanoSerialization/Yaml/Node.hpp"

#include <filesystem>
#include <string_view>

namespace Nano::Serialization::Yaml
{

	////////////////////////////////////////////////////////////////////////////////////
	// File
	////////////////////////////////////////////////////////////////////////////////////
	class File : public Node // Note: A yaml file doesn't exclusively mean a filesystem file, it just means a collection of TOML data
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
	};

}