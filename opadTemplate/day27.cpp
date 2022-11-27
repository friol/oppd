
//
// oppd treesize
// friol 2k22
//

#include "include/oppd.h"
#include <filesystem>
namespace fs = std::filesystem;

class file
{
private:

public:

	std::wstring name;
	long long int size;

	file(std::wstring n)
	{
		this->name = n;
		this->size = 0;
	}
};

class directory
{
private:

public:

	long long int totalSize; // including subdirs
	std::wstring baseDirName;
	std::vector<directory> subdirs;
	std::vector<file> files;

	directory(std::wstring basedir)
	{
		this->baseDirName = basedir;
		this->totalSize = 0;
	}

};

long long int scanDir(directory& dir)
{
	for (const auto& entry : fs::directory_iterator(dir.baseDirName))
	{
		if (entry.is_directory())
		{
			std::wstring dirName = entry.path().generic_wstring();
			//std::cout << "scanning " << dirName << std::endl;
			directory subdir(dirName);
			dir.totalSize += scanDir(subdir);
			dir.subdirs.push_back(subdir);
		}
		else
		{
			long long int fileSize = entry.file_size();
			file newf(entry.path().filename().generic_wstring());
			newf.size = fileSize;
			dir.totalSize += fileSize;
			dir.files.push_back(newf);
		}
		//std::cout << "[" << entry.path() << "] size [" << entry.file_size() << "]" << std::endl;
	}

	return dir.totalSize;
}

std::wstring intToMegabytes(long long int i)
{
	std::wstring measure = L"kb";
	i /= 1024;

	if (i > 1024)
	{
		i /= 1024;
		measure = L"Mb";

		if (i > 1024)
		{
			i /= 1024;
			measure = L"Gb";
		}
	}

	return std::to_wstring(i)+measure;
}

bool compareSizes(directory d1, directory d2)
{
	return (d1.totalSize>d2.totalSize);
}

bool compareFileSizes(file f1, file f2)
{
	return (f1.size> f2.size);
}

void day27usage(char** argv)
{
	std::wcout << "Usage:" << std::endl << argv[0] << " <directory>" << std::endl;
}

void printInfoline(std::wstring path, long long int dirsize, long long int parentDirsize,int maxNameLen)
{
	float percent = (float)dirsize / (float)parentDirsize;
	percent *= 100;
	percent = roundf(percent * 100) / 100;
	std::wcout << std::setw(maxNameLen) << path << " " << std::setw(20) << intToMegabytes(dirsize) << " " << std::setw(20) << percent << L"% ";

	for (int p = 0;p < 10;p++)
	{
		if ((p * 10) < percent)
		{
			int amount = (int)(percent - (p * 10));
			if ((amount <= 10)&&(amount>=0))
			{
				std::wcout << L"░";
			}
			else if ((amount <= 20)&&(amount>10))
			{
				std::wcout << L"▒";
			}
			else if ((amount <= 30)&&(amount>20))
			{
				std::wcout << L"▓";
			}
			else
			{
				std::wcout << L"█";
			}
		}
	}
	std::wcout << std::endl;
}

void day27(int argc, char** argv)
{
	setUnicodeConsole();
	clearScreen();
	consoleGotoxy(0, 0);

	std::wstring basepath;
	if (argc != 2)
	{
		day27usage(argv);
		return;
	}
	else
	{
		std::string p = argv[1];
		basepath = std::wstring(p.begin(),p.end());
	}
	//basepath = L"D:\\emulatorz\\";

	directory baseDir(basepath);

	long long int dirSize=scanDir(baseDir);
	std::sort(baseDir.subdirs.begin(), baseDir.subdirs.end(), compareSizes);
	std::sort(baseDir.files.begin(), baseDir.files.end(), compareFileSizes);

	// find longest filename
	int longestNameLen = 0;
	for (directory d : baseDir.subdirs)
	{
		if (d.baseDirName.size() > longestNameLen)
		{
			longestNameLen = (int)d.baseDirName.size();
		}
	}
	for (file f : baseDir.files)
	{
		if (f.name.size() > longestNameLen)
		{
			longestNameLen = (int)f.name.size();
		}
	}

	// print output

	printInfoline(baseDir.baseDirName, baseDir.totalSize, baseDir.totalSize,longestNameLen);
	for (directory d : baseDir.subdirs)
	{
		printInfoline(d.baseDirName, d.totalSize, baseDir.totalSize, longestNameLen);
	}

	for (file f : baseDir.files)
	{
		printInfoline(f.name, f.size, baseDir.totalSize, longestNameLen);
	}
}
