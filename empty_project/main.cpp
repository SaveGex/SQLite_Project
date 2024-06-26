#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <string>
#include "AddText.h"
#include "ShowDB.h"
#include "DeleteCl.h"
#include "DropTable.h"
#include "SetValue.h"
#include "Serch_Word.h"
using namespace std;

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    //Обов'язково потрібно поміняти цей шлях і вказати .db
    //const char* path_file = "C:\\Date.db";
    const char* path_file = "C:\\bonk.db";

    sqlite3* DB;
    string sql = "CREATE TABLE IF NOT EXISTS MUSIC_FOR_TEST("
        "`ID` INTEGER PRIMARY KEY AUTOINCREMENT,"
        "`AUTHOR` TEXT NOT NULL,"
        "`NAME_SONG` VARCHAR(50) NOT NULL,"
        "`ORIGINAL_TEXT` TEXT NOT NULL,"
        "`TRANSLATE_TEXT` TEXT DEFAULT 1,"
        "`YEAR` INTEGER NOT NULL);";

    int exit = 0;
    exit = sqlite3_open(path_file, &DB);
    if (exit != SQLITE_OK) {
        cerr << "\nНе вдалось відкрити БД: " << sqlite3_errmsg(DB) << endl;  // Вивід повідомлення про помилку відкриття бази даних
        sqlite3_close(DB);
        return -1;
    }

    char* messageError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) {
        cerr << "Помилка створення таблиці: " << sqlite3_errmsg(DB) << endl;  // Вивід повідомлення про помилку створення таблиці
        sqlite3_close(DB);
        return -1;
    }
    else {
        // Головне меню програми
        while (true) {
            cout << "\nДодати пісню[1]\
\nПродивитись БД[2]\
\nВидалити рядок[3]\
\nЗмінити значення[4]\
\nПошук пісні по ключовій фразі[5]\
\nВидалити таблицю[9]\
\nПереіндексація[r] (якщо потрібно)\
\nВийти[0]\n";

            char quest;
            cin >> quest;
            cout << endl;

            switch (quest) {
            case '1':
                AddText(path_file);  // Виклик функції для додавання тексту
                break;
            case '2':
                ShowDB(path_file);  // Виклик функції для відображення бази даних
                break;
            case '3':
                Delete_CL(path_file);  // Виклик функції для видалення рядка
                break;
            case '4':
                ChangeValue(path_file);  // Виклик функції для зміни значення
                break;
            case '5':
                Serch_by_Word(path_file);  // Виклик функції для пошуку за ключовою фразою
                break;
            case '9':
                DropTable(path_file);  // Виклик функції для видалення таблиці
                break;
            case 'r':
                reindexs(DB);  // Виклик функції для переіндексації
                break;
            case '0':
                cout << "\nalrigth\n" << endl;  // Вивід повідомлення про вихід з програми
                sqlite3_close(DB);
                return 0;
            }
        }
    }
}
