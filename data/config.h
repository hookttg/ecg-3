#pragma once
#include <string>

#ifdef USE_BOOST
#include <boost/filesystem>
#else
#include <filesystem>
#endif

inline std::string resolvePath(const std::string &pathStr)
{
#ifdef USE_BOOST
    using namespace boost::filesystem;
#else
    using namespace std::tr2::sys;
#endif
    return (path("W:/GITHUB/ecg") / path(pathStr)).string();
}
