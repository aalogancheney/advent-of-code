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

        std::string name{ "" };
        Directory* parent;
        std::unordered_map<std::string, Directory> directories{ };
        std::unordered_map<std::string, int32> files{ };
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
}

void Puzzle07::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle07Helpers;

	FileSystem fileSystem{ };
    PopulateFileSystem(fileSystem, input);
}

void Puzzle07::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle07Helpers;

    FileSystem fileSystem{ };
    PopulateFileSystem(fileSystem, input);
}
