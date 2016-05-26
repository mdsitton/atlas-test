#include "util.hpp"

#include <fstream>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <Winerror.h>
#include <shlobj.h>
#elif defined(PLATFORM_OSX)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#include <linux/limits.h>
#endif


void replace_substr(std::string &tagetStr, const std::string find, const std::string replace)
{
    size_t index = 0;
    while (true) {
         index = tagetStr.find(find, index);
         if (index == std::string::npos) {
             break;
         }

         tagetStr.replace(index, replace.size(), replace);

         index += replace.size();
    }
}

std::string read_file(std::string filename, FileMode mode)
{
    auto fileMode = std::ios::in | std::ios_base::ate;
    if (mode == FileMode::Binary) {
         fileMode |= std::ios_base::binary;
    }
    std::ifstream in(filename, fileMode);
    if (in) {
        std::string contents;
        contents.resize(static_cast<unsigned int>(in.tellg()));
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    } else {
        return "";
    }
}

std::string get_base_path() // executable path
{
	std::string basePath;

#if defined(PLATFORM_WINDOWS)
	char buffer[MAX_PATH];//always use MAX_PATH for filepaths
	GetModuleFileName( NULL, buffer, sizeof(buffer) );

	basePath = buffer;

#elif defined(PLATFORM_OSX)
	char path[8192];
	uint32_t size = sizeof(path);

	if ( _NSGetExecutablePath( path, &size ) < 0 )
	{
		return "";
	}

	basePath = path;

#elif defined(PLATFORM_LINUX)
	char buff[PATH_MAX];
	ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
	if (len != -1)
	{
		buff[len] = '\0';
		basePath = buff;
	} else
		basePath = "";
#endif

	// remove executable name so we just have the path
	int pos = basePath.rfind( PATH_SEP );

	basePath = basePath.substr( 0, pos+1 );

#if OSX_APP_BUNDLE
	appPath = basePath; // store full appPath

	// on osx we only want the path containing the app when checking BasePath
	pos = basePath.rfind( "MacOS" );

	if ( pos != std::string::npos )
	{
		basePath = basePath.substr( 0, pos+1 );

		pos = basePath.rfind( "Contents" );

		if ( pos != std::string::npos )
		{
			basePath = basePath.substr( 0, pos+1 );

			pos = basePath.rfind( APP_NAME );

			if ( pos != std::string::npos )
				basePath = basePath.substr( 0, pos );
		}
	}
#endif
    // Strip out weird occurances of /. at the end of the path
    std::string locator;
    locator.append(std::string(PATH_SEP));
    locator.append(".");

    pos = basePath.rfind(locator);
	if (pos != std::string::npos) {
        basePath = basePath.substr(0, pos);
    }

    // Strip trailing slash
    if (basePath.compare(basePath.size()-1, 1, PATH_SEP) == 0)
    {
        basePath = basePath.substr(0, basePath.size()-1);
    }

	return basePath;
}

std::string expand_path(const std::string virPath)
{
    std::string path = get_base_path();
    std::string virPathCopy = virPath;

    replace_substr(virPathCopy, "/", PATH_SEP);

    path.append(virPath);
    std::cout << path << std::endl;
    return path;
}
