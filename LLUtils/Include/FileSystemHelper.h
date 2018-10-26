#pragma once
#include <set>
namespace LLUtils
{
    class FileSystemHelper
    {
    public:
        static ListWString FindFiles(ListWString& filesList, std::filesystem::path workingDir, std::wstring fileTypes, bool recursive, bool caseSensitive)
        {
        
            using namespace std::filesystem;
            ListWString extensions = StringUtility::split(fileTypes, L';');
            std::set<std::wstring> extensionSet;

            for (const auto& ext : extensions)
                extensionSet.insert(caseSensitive == true ? ext : StringUtility::ToUpper(ext));

            auto AddFileIfExtensionsMatches = [&](const path& filePath)
            {
                //TODO : use c++17 string_view instead of erasing the dot
                std::wstring extNoDot = filePath.extension().wstring().erase(0, 1);

                if (extensionSet.find(caseSensitive == true ? extNoDot : StringUtility::ToUpper(extNoDot)) != extensionSet.end())
                    filesList.push_back(filePath.wstring());

            };

            if (recursive == true)
                for (const auto& p : recursive_directory_iterator(workingDir))
                    AddFileIfExtensionsMatches(p);
            else
                for (const auto& p : directory_iterator(workingDir))
                    AddFileIfExtensionsMatches(p);

            return filesList;
        }

        static std::wstring ResolveFullPath(std::wstring fileName)
        {
            using namespace std::filesystem;
            path p(fileName);
            if (p.empty() == false && p.is_absolute() == false)
            {
                p = current_path() / fileName;
                if (exists(p) == false)
                    p.clear();

            }

            return p.wstring();
        }

        template <class string_type>
        static bool EnsureDirectory(const string_type& path)
        {
            using namespace std;
            filesystem::path directoryName = path;
            directoryName.remove_filename();
            return filesystem::exists(directoryName) || filesystem::create_directories(directoryName);
        }

    };
}
