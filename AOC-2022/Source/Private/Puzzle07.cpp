#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle07Helpers
{
    struct Directory
    {
    public:
        Directory()
            : name{ "" }
            , parent{ nullptr }
        {

        }

        Directory(const std::string& inName, Directory* inParent)
            : name{ inName }
            , parent{ inParent }
        {
            
        }

        uint64 size{ 0 };
        std::string name{ "" };
        Directory* parent;
        std::unordered_map<std::string, Directory> directories{ };
        std::unordered_map<std::string, uint64> files{ };
    };

    class FileSystem
    {
    public:
        void AddFileToCurrentWorkingDirectory(const std::string& name, int32 size)
        {
            check(currentWorkingDirectory != nullptr);
            check(currentWorkingDirectory->files.find(name) == currentWorkingDirectory->files.end());
            currentWorkingDirectory->files.emplace(name, size);
        }

        void AddDirectoryToCurrentWorkingDirectory(const std::string& name)
        {
            check(currentWorkingDirectory != nullptr);
            if (currentWorkingDirectory->directories.find(name) == currentWorkingDirectory->directories.end())
            {
                currentWorkingDirectory->directories.emplace(name, Directory{ name, currentWorkingDirectory });
            }
        }

        void ChangeDirectory(const std::string& command)
        {
            if (command == "/")
            {
                currentWorkingDirectory = &topDirectory;
            }
            else if (command == "..")
            {
                currentWorkingDirectory = currentWorkingDirectory->parent;
            }
            else
            {
                check(currentWorkingDirectory->directories.find(command) != currentWorkingDirectory->directories.end());
                currentWorkingDirectory = &currentWorkingDirectory->directories[command];
            }
        }

        const Directory& GetTopDirectory() const { return topDirectory;  }
        Directory& GetTopDirectory() { return topDirectory; }

    private:
        Directory topDirectory{ "/", &topDirectory };
        Directory* currentWorkingDirectory{ &topDirectory };
    };

    void PopulateFileSystem(FileSystem& fileSystem, const std::vector<std::string>& input)
    {
        for (int32 index{ 0 }; index < input.size(); ++index)
        {
            const std::string& line{ input[index] };
            check(line.size() != 0);
            if (line[0] == '$')
            {
                const auto command{ line.substr(2, 2) };
                if (command == "cd")
                {   
                    fileSystem.ChangeDirectory(line.substr(5));
                }
                else if (command == "ls")
                {
                    bool bReachedNextCommand{ false };
                    while (!bReachedNextCommand && index < input.size() - 1)
                    {
                        const std::string& lsLine{ input[++index] };
                        if (lsLine[0] == '$')
                        {
                            --index;
                            bReachedNextCommand = true;
                        }
                        else if (lsLine[0] == 'd')
                        {
                            fileSystem.AddDirectoryToCurrentWorkingDirectory(lsLine.substr(4));
                        }
                        else
                        {
                            std::stringstream ss{ lsLine };
                            int32 fileSize;
                            std::string fileName;
                            ss >> fileSize >> fileName;
                            fileSystem.AddFileToCurrentWorkingDirectory(fileName, fileSize);
                        }
                    }
                }
                else
                {
                    checkNoEntry();
                }
            }
        }
    }

    void GetDirectorySizes(const Directory& root, const std::string& parentPath, std::unordered_map<std::string, uint64>& directorySizes)
    {
        const std::string myPath{ parentPath + root.name };

        // Start with size 0.
        directorySizes.emplace(myPath, 0);
        
        // Add the size of each file contained in this directory.
        for (const auto& iter : root.files)
        {
            directorySizes[myPath] += iter.second;
        }

        // Recursively search subdirectories and add their values.
        for (const auto& iter : root.directories)
        {
            GetDirectorySizes(iter.second, myPath + iter.first + "/", directorySizes);
            directorySizes[myPath] += directorySizes[iter.first];
        }
    }

    uint64 GetDirectorySizes2(Directory& root)
    {
        for (auto& subDirectories : root.directories)
        {
            root.size += GetDirectorySizes2(subDirectories.second);
        }

        for (const auto& file : root.files)
        {
            root.size += file.second;
        }

        return root.size;
    }

    uint64 GetTotalSizeOfDirectoriesLessThan(const Directory& root, const uint64 lessThan)
    {
        uint64 total{ 0 };
        for (const auto& subDirectories : root.directories)
        {
            total += GetTotalSizeOfDirectoriesLessThan(subDirectories.second, lessThan);
        }
        if (root.size < lessThan)
        {
            total += root.size;
        }
        return total;
    }

    void GetSmallestDirectorySizeGreaterThanOrEqualTo(const Directory& root, const uint64 greaterThan, std::vector<uint64>& sizes)
    {
        if (root.size >= greaterThan)
        {
            sizes.emplace_back(root.size);
        }
        for (const auto& subDirectories : root.directories)
        {
            GetSmallestDirectorySizeGreaterThanOrEqualTo(subDirectories.second, greaterThan, sizes);
        }
    }

    void PrintDirectoryStructure(const Directory& root, const std::string& tabs, const std::string& prefix)
    {
        std::cout << tabs << "- " << prefix << " (dir, size=" << root.size << ")" << std::endl;
        for (const auto& subDirectory : root.directories)
        {
            PrintDirectoryStructure(subDirectory.second, " " + tabs, prefix + subDirectory.second.name + "/");
        }
        for (const auto& file : root.files)
        {
            std::cout << tabs << " - " << file.first << " (file, size=" << file.second << ")" << std::endl;
        }
    }
}

void Puzzle07::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle07Helpers;

	FileSystem fileSystem{ };
    PopulateFileSystem(fileSystem, input);

    uint64 totalSize{ GetDirectorySizes2(fileSystem.GetTopDirectory()) };
    uint64 totalSizeMatching{ GetTotalSizeOfDirectoriesLessThan(fileSystem.GetTopDirectory(), 100001) };
    std::cout << totalSizeMatching << std::endl;
    //PrintDirectoryStructure(fileSystem.GetTopDirectory(), "", "/");
}

void Puzzle07::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle07Helpers;

    FileSystem fileSystem{ };
    PopulateFileSystem(fileSystem, input);

    uint64 totalSize{ GetDirectorySizes2(fileSystem.GetTopDirectory()) };
    uint64 requiredSpace{ 30000000 - (70000000 - totalSize) };
    std::vector<uint64> candidates{ };
    GetSmallestDirectorySizeGreaterThanOrEqualTo(fileSystem.GetTopDirectory(), requiredSpace, candidates);
    std::sort(candidates.begin(), candidates.end());
    check(candidates.size() > 0);
    std::cout << candidates.front() << std::endl;
}
