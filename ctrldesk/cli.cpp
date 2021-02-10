#include "cli.h"

//Private

string CLI::ConvertWideToUtf8(wstring wstr)
{
    if (wstr.empty())
    {
        return string();
    }
    int count = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    string str;
    if (count)
    {
        str.resize(count);
        if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstr.c_str(), (int)wstr.size(), &str[0], count, NULL, NULL))
        {
            return str;
        }
    }

    return string();
}

wstring CLI::ConvertUtf8ToWide(string str)
{
    if (str.empty())
    {
        return wstring();
    }
    int count = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), NULL, 0);
    wstring wstr;
    if (count)
    {
        wstr.resize(count);
        if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), &wstr[0], count))
        {
            return wstr;
        }
    }
    return wstring();
}

//Public

CLI::CLI()
{

}

DWORD CLI::getError()
{
    return dwError;
}

bool CLI::checkForErrors()
{
    if ((dwError != ERROR_NO_MORE_FILES) && (dwError != 0))
    {
        return true;
    }
    return false;
}

vector <string> CLI::getPath(vector <string> argv_vector)
{
    vector <string> path_vector;
    wstring targetPath;
    wstring targetFormat;
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // If the directory is not specified as a command-line argument,
    // print usage.

    if (argv_vector.size() != 2)
    {
        string thisProgName = argv_vector[0].substr(argv_vector[0].rfind("\\") + 1);
        cout << "Usage: " << thisProgName << " <directory name>" << endl;
        dwError = 11;
        return path_vector; 
    }

    // Check that the input path plus 7 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.

    if (argv_vector[1].length() > (MAX_PATH - 7))
    {
        cout << "Directory path is too long." << endl;
        dwError = 11;
        return path_vector;
    }

    // Prepare string for use with FindFile functions.  First, copy the
    // string to a buffer, then append '\*' to the directory name.

    targetPath = ConvertUtf8ToWide(argv_vector[1]);
    targetPath = ConvertUtf8ToWide(argv_vector[1]);

    if (targetPath[targetPath.size() - 1] != '\\' && 
        targetPath[targetPath.size() - 1] != '/')
    {
        targetPath = targetPath + L"\\";
    }

    targetFormat = L"*.bmp";

    wcout << L"Target directory is " << targetPath + targetFormat << endl;

    // Find the first file in the directory.

    hFind = FindFirstFileW((targetPath + targetFormat).c_str(), &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        dwError = GetLastError();
        cout << "FindFirstFile ERROR: " << dwError << endl;
        return path_vector;
    }

    // List all the files in the directory with some info about them.

    do
    {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            string fullPath = ConvertWideToUtf8(targetPath + ffd.cFileName);
            path_vector.push_back(fullPath);
        }
    } while (FindNextFileW(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        cout << "FindNextFile ERROR: " << dwError << endl;
    }
    FindClose(hFind);

#ifdef _DEBUG
    for (vector <string>::iterator iter = path_vector.begin(); iter != path_vector.end(); ++iter)
    {
        cout << "  " + *iter << endl;
    }
#endif // _DEBUG

    return path_vector;
}