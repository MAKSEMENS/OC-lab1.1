#include <windows.h>
#include <iostream>
#include <map>
#include <clocale>
#include <vector>
#include <format>
#include <array>
#include <sstream>

using namespace std;
constexpr std::array<int, 21> attributeEnums = {

        FILE_ATTRIBUTE_HIDDEN,
        FILE_ATTRIBUTE_SYSTEM,
        FILE_ATTRIBUTE_DIRECTORY,
        FILE_ATTRIBUTE_ARCHIVE,
        FILE_ATTRIBUTE_DEVICE,
        FILE_ATTRIBUTE_NORMAL,
        FILE_ATTRIBUTE_TEMPORARY,
        FILE_ATTRIBUTE_SPARSE_FILE,
        FILE_ATTRIBUTE_REPARSE_POINT,
        FILE_ATTRIBUTE_COMPRESSED,
        FILE_ATTRIBUTE_OFFLINE,
        FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
        FILE_ATTRIBUTE_ENCRYPTED,
        FILE_ATTRIBUTE_INTEGRITY_STREAM,
        FILE_ATTRIBUTE_VIRTUAL,
        FILE_ATTRIBUTE_NO_SCRUB_DATA,
        FILE_ATTRIBUTE_EA,
        FILE_ATTRIBUTE_PINNED,
        FILE_ATTRIBUTE_UNPINNED,
        FILE_ATTRIBUTE_RECALL_ON_OPEN,
        FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS

};


std::map<int, std::string> attributeMap = {

        {FILE_ATTRIBUTE_HIDDEN, "Hidden"},
        {FILE_ATTRIBUTE_SYSTEM, "Part of OS"},
        {FILE_ATTRIBUTE_DIRECTORY, "Directory"},
        {FILE_ATTRIBUTE_ARCHIVE, "Marked for backup or removal"},
        {FILE_ATTRIBUTE_DEVICE, "Reserved for system use"},
        {FILE_ATTRIBUTE_NORMAL, "Normal"},
        {FILE_ATTRIBUTE_TEMPORARY, "Temporary"},
        {FILE_ATTRIBUTE_SPARSE_FILE, "Sparse file"},
        {FILE_ATTRIBUTE_REPARSE_POINT, "Reparse point or symbolic link"},
        {FILE_ATTRIBUTE_COMPRESSED, "Compressed"},
        {FILE_ATTRIBUTE_OFFLINE, "Offline"},
        {FILE_ATTRIBUTE_NOT_CONTENT_INDEXED, "Not content indexed"},
        {FILE_ATTRIBUTE_ENCRYPTED, "Encrypted"},
        {FILE_ATTRIBUTE_INTEGRITY_STREAM, "Integrity stream"},
        {FILE_ATTRIBUTE_VIRTUAL, "Virtual"},
        {FILE_ATTRIBUTE_NO_SCRUB_DATA, "No scrub data"},
        {FILE_ATTRIBUTE_EA, "Extended attributes"},
        {FILE_ATTRIBUTE_PINNED, "Pinned"},
        {FILE_ATTRIBUTE_UNPINNED, "Unpinned"},
        {FILE_ATTRIBUTE_RECALL_ON_OPEN, "Recall on open"},
        {FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS, "Recall on data access"}

};

vector<string> getListOfAttributes(DWORD mask) {
    std::vector<std::string> result;

    for (auto attr : attributeEnums) {
        if (mask & attr) {
            result.emplace_back(attributeMap.at(attr));
        }
    }
    return result;
}

vector<string> getFileAttributesInternal(string path) {
    auto attributesMask = GetFileAttributesA(path.c_str());
    if (attributesMask == INVALID_FILE_ATTRIBUTES) {
        cout << "Something went wrong!\n";
        return {};
    }

    return getListOfAttributes(attributesMask);
}

void getFileAttributes() {
    cout << "Enter file path: ";
    string path;
    cin >> path;

    auto attributes = getFileAttributesInternal(path);

    for (auto& el : attributes) {
        cout << format(" {}\n", el);
    }

}


void showMenu(){
        
    cout<<"Options: "<<endl;
    cout<<"  Show disc list - 1"<<endl;
    cout<<"  Show info about disc - 2"<<endl;
    cout<<"  Creating and removing of directory - 3"<<endl;
    cout<<"  Creating files - 4"<<endl;
    cout<<"  Copying and removing files - 5"<<endl;
    cout<<"  Analyzing and changing file attributes -6"<<endl;
    cout<<"  Get or set file time - 7"<<endl;
    cout<<"  Exit - 0"<<endl;
        
}
void showDrives(){
    DWORD dwDrives  = 0, dwMask = 1;
    CHAR chDrive;
    dwDrives = GetLogicalDrives();
    printf("Currently available Disk Drives : \n");
    for(UINT i = 0; i< 26;i++)
    {
        if(dwDrives & dwMask)
        {
            chDrive = 'A' + i;
            printf("%c:\n",chDrive);
        }
        dwMask <<= 1;
    }
}

void getDiskInfo ( string diskName)
{
    map<string, LPCSTR> pairs
            {
        
                    {"A","A:\\"}, {"B","B:\\"},{"C", "C:\\"},{"D", "D:\\"},
                    {"E", "E:\\"},{"F", "F:\\"},{"G", "G:\\"},{"H", "H:\\"},
                    {"I", "I:\\"},{"J", "J:\\"},{"K", "K:\\"},{"L", "L:\\"},
                    {"M", "M:\\"},{"N", "N:\\"},{"O", "O:\\"},{"P", "P:\\"}
        
            };
    int type = GetDriveType(pairs.at(diskName));
    cout<<"Drive type:";
    if (type == DRIVE_UNKNOWN)
        cout << " UNKNOWN" << endl;
    if (type == DRIVE_NO_ROOT_DIR)
        cout << " DRIVE NO ROOT DIR" << endl;
    if (type == DRIVE_REMOVABLE)
        cout << " REMOVABLE" << endl;
    if (type == DRIVE_FIXED)
        cout << " FIXED" << endl;
    if (type == DRIVE_REMOTE)
        cout << " REMOTE" << endl;
    if (type == DRIVE_CDROM)
        cout << " CDROM" << endl;
    if (type == DRIVE_RAMDISK)
        cout << " RAMDISK" << endl;

    char NameBuffer[MAX_PATH];
    char SysNameBuffer[MAX_PATH];
    DWORD VSNumber;
    DWORD MCLength;
    DWORD FileSF;

    if (GetVolumeInformation(pairs.at(diskName),NameBuffer, sizeof(NameBuffer),
                             &VSNumber,&MCLength,&FileSF,SysNameBuffer,sizeof(SysNameBuffer)))
    {
            
        cout <<  "Tom name: " << NameBuffer << endl;
        cout << "File system name: " << SysNameBuffer << endl;
        cout << "Serial number: " << VSNumber << endl;
        cout <<  "Max length of tom's name: " <<MCLength << endl;
        cout <<  "File system flag: " <<FileSF << endl;
            
    }

    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

    // Получаем информацию о свободном месте на диске
    if (GetDiskFreeSpaceEx("C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        // Преобразуем байты в гигабайты для удобства чтения
        double totalGB = static_cast<double>(totalNumberOfBytes.QuadPart) / (1024 * 1024 * 1024);
        double freeGB = static_cast<double>(totalNumberOfFreeBytes.QuadPart) / (1024 * 1024 * 1024);

        cout << "Total space on the disk: " << totalGB << " GB" << endl;
        cout << "Free space on the disk: " << freeGB << " GB" << endl;
    } else {
        DWORD error = GetLastError(); // Получаем код ошибки
        cerr << "Failed to retrieve disk space information. Error code: " << error << endl;
    }
}

void createAndRemoveDirectory( int param) {
    std::cout << "Input directory: ";
    std::string temp_path;
    std::getline(std::cin>>ws, temp_path); // Считываем строку
    LPCSTR path = temp_path.c_str();

    if (param == 1) {
        if (CreateDirectory(path, nullptr)) { // Создаем каталог
            std::cout << "Directory created successfully." << std::endl;
        }
     else {
        DWORD error = GetLastError(); // Получаем код ошибки
        if (error == ERROR_ALREADY_EXISTS) {
            std::cerr << "Directory already exists." << std::endl;
        } else {
            std::cerr << "Failed to create directory. Error code: " << error << std::endl;
        }
    }
}
    else if (param == 2){
        if (RemoveDirectory(path)) { // Удаляем каталог
            wcout << "Directory removed successfully." << endl;
        } else {
            DWORD error = GetLastError(); // Получаем код ошибки
            wcerr << "Failed to remove directory. Error code: " << error << endl;
        }
    }
}

void copyAndMoveFile(int check){
    int input;
    LPCTSTR lpExistingFileName;
    LPCTSTR lpNewFileName;
    WINBOOL bFailIfExists;
    string temp_existF, temp_newF;

    cout<<"Input existing file directory:";
    cin>>temp_existF;
    lpExistingFileName = temp_existF.c_str();
    cout<<"Input new file directory:";
    cin>>temp_newF;
    lpNewFileName = temp_newF.c_str();
    cout<<"Input 0 to rewrite file (if already exist) and 1 else:  ";
    std::cin >> input;
    if (check==1){

        // Преобразование ввода в BOOL
        bFailIfExists = (input == 0) ? FALSE : TRUE;

        if (CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists)) {
            cout << "File was successfully copied." << endl;
        } else {
            DWORD error = GetLastError();
            cerr << "Failed to copy file. Error code: " << error << std::endl;

        }
    }
    if (check==2){

        bFailIfExists = (input == 0) ? MOVEFILE_REPLACE_EXISTING : 0;
        // Перемещение файла
        if (MoveFileEx(lpExistingFileName, lpNewFileName, bFailIfExists) != 0) {
            std::wcout << L"File moved successfully." << std::endl;
        } else {
            DWORD error = GetLastError();
            if (error == ERROR_ALREADY_EXISTS) {
                std::wcerr << L"File with the same name already exists in the target directory." << std::endl;
            } else {
                std::wcerr << L"Failed to move file. Error code: " << error << std::endl;
            }
        }
    }
    else {
        cout<<"You input incorrect number."<<endl;
    }
}

/*void getAndSetAttributes(){
    LPCSTR filePath = "E:\\aaa\\3.txt";

    // Получаем атрибуты файла
    DWORD fileAttributes = GetFileAttributes(filePath);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "Failed to get file attributes. Error code: " << GetLastError() << std::endl;
    }

    // Выводим атрибуты файла
    std::cout << "File attributes: " << fileAttributes << std::endl;

}*/

void newFile(LPCSTR fileName){
    HANDLE hFile = CreateFile(
            fileName,                    // Имя файла
            GENERIC_WRITE,              // Режим доступа (здесь: запись)
            0,                          // Флаги создания (здесь: создать новый файл)
            NULL,                       // Атрибуты безопасности (здесь: по умолчанию)
            CREATE_ALWAYS,             // Режим открытия (здесь: всегда создавать новый файл)
            FILE_ATTRIBUTE_NORMAL,     // Атрибуты файла (здесь: обычный файл)
            NULL                        // Дескриптор файла шаблона (здесь: отсутствует)
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError(); // Получаем код ошибки
        cerr << "Failed to create file. Error code: " << error << endl;
    }else{
        cout << "File created successfully." << endl;
    }

    CloseHandle(hFile); // Закрываем дескриптор файла
}

void getFileTime() {
    std::string path;
    std::cout << "Enter path to file: ";
    std::cin >> path;

    HANDLE hFile1;
    FILETIME ftCreate, ftLastAccess, ftLastWrite;
    SYSTEMTIME stCreate, stAccess, stWrite;

    hFile1 = CreateFile(path.c_str(),
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        nullptr,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        nullptr);

    if (hFile1 == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        std::cout << "Failed to open file. Error: " << error<< std::endl;
        return;
    }

    if (!GetFileTime(hFile1, &ftCreate, &ftLastAccess, &ftLastWrite)) {
        DWORD error = GetLastError();
        std::cout << "Failed to get file time. Error: " << error << std::endl;
        CloseHandle(hFile1);
        return;
    }


    FileTimeToSystemTime(&ftCreate, &stCreate);
    FileTimeToSystemTime(&ftLastAccess, &stAccess);
    FileTimeToSystemTime(&ftLastWrite, &stWrite);

    std::cout << "File creation time: " << stCreate.wDay << "/" << stCreate.wMonth << "/" << stCreate.wYear << " "
              << stCreate.wHour + 3 << ":" << stCreate.wMinute << std::endl;
    std::cout << "Last access time: " << stAccess.wDay << "/" << stAccess.wMonth << "/" << stAccess.wYear << " "
              << stAccess.wHour + 3 << ":" << stAccess.wMinute << std::endl;
    std::cout << "Last write time: " << stWrite.wDay << "/" << stWrite.wMonth << "/" << stWrite.wYear << " "
              << stWrite.wHour + 3 << ":" << stWrite.wMinute << std::endl;

    CloseHandle(hFile1);
}

void setFileTime() {
    string path;
    cout << "Enter path to file: ";
    cin >> path;

    cout << "Enter date {dd mm yyyy}: ";
    int day, month, year;
    cin >> day >> month >> year;

    SYSTEMTIME systemTime;

    GetSystemTime(&systemTime);

    systemTime.wDay = day;
    systemTime.wMonth = month;
    systemTime.wYear = year;

    FILETIME fileTime;

    SystemTimeToFileTime(&systemTime, &fileTime);

    HANDLE filename = CreateFile(path.c_str(),
                                 FILE_WRITE_ATTRIBUTES,
                                 FILE_SHARE_READ | FILE_SHARE_WRITE,
                                 nullptr,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 nullptr);

    if (filename != INVALID_HANDLE_VALUE) {
        SetFileTime(filename,
                    (LPFILETIME) nullptr,
                    (LPFILETIME) nullptr,
                    &fileTime);
        CloseHandle(filename);
        cout << "File time set successfully." << endl;
    } else {
        cout << "Failed to open file." << endl;
    }
}

void setFileAttributes() {

    cout << "Enter file path: ";
    string path;
    cin >> path;

    auto attributes = getFileAttributesInternal(path);

    cout << "Current attributes:\n";
    for (auto &el: attributes) {
        cout << format(" {}\n", el);
    }

    cout << "List of available attributes:\n";
    for (int i = 0; i < attributeEnums.size(); ++i) {
        cout << format(" {}){}\n", i + 1, attributeMap.at(attributeEnums[i]));
    }
    cout << endl;
    cout << "Enter indexes which you want to add: ";
    string indexesLine;

    // Disable skipping whitespace characters
    cin >> ws >> noskipws;

    // Read from stdin until the end of the line
    char c;
    while (cin >> c && c != '\n') {
        indexesLine += c;
    }

    // Enable skipping whitespace characters again
    cin >> skipws;
    stringstream ss(indexesLine);

    vector<int> indexes;

    int index;
    while (ss >> index) {
        indexes.push_back(index);
    }

    DWORD attributeMask = 0;
    for (auto el: indexes) {
        attributeMask |= attributeEnums[el - 1];
    }

    if (!SetFileAttributesA(path.c_str(), attributeMask)) {
        DWORD error = GetLastError();
        cout << format("error: {}\n", error);
        return;
    }
    cout << "Attributes added successfully!" << endl;

}

int main(void)
{
    setlocale(LC_ALL,"Russian");
    SetConsoleCP(1251); // Ввод с консоли в кодировке 1251
    SetConsoleOutputCP(1251);
    int check;
    do {
        system("cls");
        showMenu();
        cin>>check;
        switch(check)
        {
            case 1:
                showDrives();
                system("pause");
                break;
            case 2: {
                cout<<"Input disk: ";
                string diskName;
                cin >> diskName;
                getDiskInfo(diskName);
                system("pause");

                break;
            }
            case 3:
            {
                int param;
                cout<<"Input 1 to create and 2 to remove directory: ";
                cin>>param;
                createAndRemoveDirectory( param);
                system("pause");
                break;
            }
            case 4: {
                cout << "Input file name with directory: ";
                string temp_name;
                LPCSTR name;
                cin >> temp_name;
                name = temp_name.c_str();
                newFile(name);
                system("pause");
                break;

            }
            case 5: {
                int check;
                cout<<"Input 1 to copy, 2 to remove: ";
                cin>>check;
                copyAndMoveFile(check);
                system("pause");
                break;
            }
            case 6: {
                int check;
                cout<<"Input 1 to get 2 to set attributes: ";
                cin>> check;
                if(check == 1){
                    getFileAttributes();
                }
                else if(check==2){
                    setFileAttributes();
                }
                else{
                    cout<<"Incorrect input"<<endl;
                }
                system("pause");
                break;
            }
            case 7:
            {
                int check;
                cout<<"Input 1 to get, 2 to set file time"<<endl;
                cin>>check;
                if(check == 1){
                    getFileTime();
                }
                else if(check==2){
                    setFileTime();
                }
                else{
                    cout<<"Incorrect input"<<endl;
                }
                system("pause");
                break;
            }

        }

    }while (check!=0);

    return 0;
}
