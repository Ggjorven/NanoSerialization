#pragma once

#include "NanoSerialization/Json/Json.hpp"
#include "NanoSerialization/Json/Node.hpp"

#include <filesystem>
#include <string_view>

namespace Nano::Serialization::Json
{

	////////////////////////////////////////////////////////////////////////////////////
	// File
	////////////////////////////////////////////////////////////////////////////////////
	class File : public Node // Note: A json file doesn't exclusively mean a filesystem file, it just means a collection of JSON data
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