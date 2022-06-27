#include <load_extension/load_extension.h>

#include <environment/environment_variable_path.h>
#include <log/log.h>
#include <metacall/metacall.h>

#include <assert.h>

#include <filesystem>
#include <string>

#define METACALL_EXTENSIONS_PATH "METACALL_EXTENSIONS_PATH" /*ENV Variable for plugin path*/

namespace fs = std::filesystem;

std::string get_ext_path()
{
	/* Initialize the library path */
	const char name[] = "metacall"
#if (!defined(NDEBUG) || defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG) || defined(__DEBUG__))
						"d"
#endif
		;

	dynlink_library_path_str path;
	size_t length = 0;

	/* The order of precedence is:
  * 1) Environment variable
  * 2) Dynamic link library path of the host library
  */
	dynlink_library_path(name, path, &length); //TODO: check return value

	char *lib_path = environment_variable_path_create(METACALL_EXTENSIONS_PATH, path, length + 1, NULL);
	if (!lib_path)
	{
		return "";
	}

	fs::path tmp(lib_path);
	environment_variable_path_destroy(lib_path);
	tmp /= "extensions";
	return tmp.string();
}

void load_extension(void *loader, void *context)
{
	std::string ext_path = get_ext_path();
	if (ext_path.empty())
	{
		/*TODO: log*/
		assert(!"Failed to get metacall lib path");
	}

	std::string m_begins = "metacall-";
	std::string m_ends = ".json";

	struct metacall_allocator_std_type std_ctx = { &std::malloc, &std::realloc, &std::free };
	void *config_allocator = metacall_allocator_create(METACALL_ALLOCATOR_STD, (void *)&std_ctx);

	auto i = fs::recursive_directory_iterator(ext_path);
	while (i != fs::recursive_directory_iterator())
	{
		if (i.depth() == 2)
			i.disable_recursion_pending();

		fs::directory_entry dir(*i);
		if (dir.is_regular_file())
		{
			std::string config = dir.path().filename().c_str();

			if (config == "metacall.json" ||
				(config.substr(0, 9) == m_begins &&
					config.substr(config.size() - m_ends.size()) == m_ends))
			{
				log_write("metacall", LOG_LEVEL_DEBUG, "Loading extension: %s", dir.path().filename().c_str());
				metacall_load_from_configuration(dir.path().c_str(), NULL, config_allocator);
				i.pop();
				continue;
			}
		}
		i++;
	}

	metacall_allocator_destroy(config_allocator);
}