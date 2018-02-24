#include "stdafx.h"

using namespace std;

unsigned long totalBytes = 0;
unsigned long totalFiles = 0;
unsigned long totalDirectories = 0;


wstring getDirectoryPath()
{
    wstring directoryPath;
    cout << "Enter the folder path: ";
    getline(wcin, directoryPath);
    return directoryPath;
}

void handleFile(WIN32_FIND_DATAW fileData)
{
    totalFiles++;
    WCHAR *fileName = fileData.cFileName;
    totalBytes += fileData.nFileSizeLow;
    wcout << L"FILE: " << fileName << L" SIZE: " << fileData.nFileSizeLow << L" B" << endl;
}

int getCategorySize(wstring directoryPath)
{
    totalDirectories++;
    long categoryBytes = 0;
    wstring directoryFindString = directoryPath + L"/*";
    WIN32_FIND_DATAW fileData;
    HANDLE const hFind = FindFirstFile(directoryFindString.c_str(), &fileData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        wcout << L"Invalid folder path: " << directoryPath << endl;
        return 0;
    }
    do
    {
        WCHAR *fileName = fileData.cFileName;
        if (fileName == L"." || fileName == L"..") { continue; }
        if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0)
        {
            categoryBytes = getCategorySize(directoryPath + L"/" + fileData.cFileName);
            continue;
        }
        categoryBytes += fileData.nFileSizeLow;
        totalFiles++;
    } while (NULL != FindNextFileW(hFind, &fileData));

    FindClose(hFind);
    return categoryBytes;
}

void handleDirectory(WIN32_FIND_DATAW directory, wstring rootDirectoryPath)
{
    WCHAR *directoryName = directory.cFileName;
    wstring directoryPath = rootDirectoryPath + L"/" + directoryName;
    long dirSize = getCategorySize(directoryPath);
    totalBytes += dirSize;
    wcout << L"DIRECTORY: " << directoryName << " SIZE: " << dirSize << " B" << endl;
}

int main()
{
    const wstring directoryPath = getDirectoryPath() + L"/*";
    WIN32_FIND_DATAW fileData;
    HANDLE const hFind = FindFirstFile(directoryPath.c_str(), &fileData);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        cout << "Invalid folder path" << endl;
        system("pause");

        return 0;
    }
    do
    {
        WCHAR *fileName = fileData.cFileName;
        if (fileName == L"." || fileName == L"..") { continue; }
        if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0) 
        {
            handleDirectory(fileData, directoryPath.substr(0, directoryPath.length()-2));
            continue;
        }
        handleFile(fileData);
    } 
    while (NULL != FindNextFileW(hFind, &fileData));

    FindClose(hFind);
    wcout << endl << L"=======SUMMARY=======" << endl;
    wcout << "SIZE: " << totalBytes / 1024 << " KB" << endl;
    wcout << "FILES COUNT: " << totalFiles << endl;
    wcout << "DIRECTORIES COUNT: " << totalDirectories << endl;

    system("pause");

    return 0;
}




