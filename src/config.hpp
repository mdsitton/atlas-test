#pragma once

#if defined(_WIN32)
    #define PLATFORM_WINDOWS
    #define PATH_SEP "\\"
#elif defined(__linux__)
    #define PLATFORM_LINUX
    #define PATH_SEP "/"
#elif defined(__APPLE__)
    #define PLATFORM_OSX
    //should define OSX_APP_BUNDLE here...
    #define PATH_SEP "/"
#endif
