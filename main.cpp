#include <windows.h>
#include <iostream>
#include <map>
#include <clocale>

using namespace std;

void showMenu(){
    cout<<"Options: "<<endl;
    cout<<"  Show disc list - 1"<<endl;
    cout<<"  Show info about disc - 2"<<endl;
    cout<<"  Creating and removing of directory - 3"<<endl;
    cout<<"  Creating files - 4"<<endl;
    cout<<"  Copying and removing files - 5"<<endl;
    cout<<"  Analyzing and changing file attributes -6"<<endl;
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
    if (check==1){
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

        // Преобразование ввода в BOOL
        bFailIfExists = (input == 0) ? FALSE : TRUE;

        if (CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists)) {
            cout << "File was successfully copied." << endl;
        } else {
            DWORD error = GetLastError();
            cerr << "Failed to copy file. Error code: " << error << std::endl;

        }
    }
}

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
            case 5:
                copyAndMoveFile(1);
                system("pause");
                break;
            /*case 6:
                analysing
*/
        }

    }while (check!=0);

    return 0;
}
