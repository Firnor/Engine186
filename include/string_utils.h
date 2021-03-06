#pragma once

namespace e186
{
	namespace utils
	{
		
#ifdef USE_BACKSPACES_FOR_PATHS
		const char SEP_TO_USE = '\\';
		const char SEP_NOT_TO_USE = '/';
#else
		const char SEP_TO_USE = '/';
		const char SEP_NOT_TO_USE = '\\';
#endif

		std::string ExtractBasePath(const std::string& path);
		std::string CleanUpPath(const std::string& path);
		std::string CombinePaths(const std::string& first, const std::string& second);
		std::string CombinePaths(const char* first, const std::string& second);
		std::string CombinePaths(const std::string& first, const char* second);
		std::string CombinePaths(const char* first, const char* second);
	}
}
