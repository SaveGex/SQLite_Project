#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

int AddText(const char* path) {
    // Встановлення кодування консолі для підтримки кирилиці
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Запит у користувача, звідки зчитувати текст
    char quest;
    cout << "\nЗчитати текст з файлу[1]\
\nНаписати текст вручну[2]\n";
    cin >> quest;

    // Якщо користувач вибрав зчитування з файлу
    if (quest == '1') {
        // Зчитування шляху до файлу
        string path_to_add;
        cout << "(Будь який файл.txt)\nНапишіть шлях до файлу: \n";
        cin.ignore();
        getline(cin, path_to_add);

        path_to_add += ".txt";
        cout << path_to_add << endl;
        ifstream File(path_to_add);
        if (!File.is_open()) {
            cout << "Файл не існує\n";
            return -1;
        }

        // Зчитування тексту з файлу
        string word;
        string text_original = "";
        while (File) {
            getline(File, word);
            text_original += word;
            if (File.eof()) {
                break;
            }
        }

        cout << "\nЗчитування пройшло\n";
        // Підключення до бази даних
        sqlite3* DB;
        int exit = sqlite3_open(path, &DB);
        if (exit != SQLITE_OK) {
            cerr << "\nПомилка відкриття: " << sqlite3_errmsg(DB) << endl;
            return -1;
        }
        else {
            cout << "\nБаза відкрита";
        }

        // Запит на введення перекладу
        char quest_translate;
        cout << "\n\033[36mВвести переклад Так[1] Ні[2]\033[0m\n";
        cin >> quest_translate;
        cin.ignore();

        // Введення автора та назви пісні
        string author;
        cout << "\nВведіть автора: ";
        getline(cin, author);

        string song_name;
        cout << "\nВведіть назву: ";
        getline(cin, song_name);

        // Підготовка SQL-запиту
        bool add_translate = false;
        string sql, translate_text;
        if (quest_translate == '1') {
            sql = "INSERT INTO MUSIC_FOR_TEST (AUTHOR, NAME_SONG, ORIGINAL_TEXT, TRANSLATE_TEXT, YEAR) VALUES(?, ?, ?, ?, ?)";
            cout << "Введіть переклад пісні: ";
            getline(cin, translate_text);
            add_translate = true;
        }
        else {
            sql = "INSERT INTO MUSIC_FOR_TEST (AUTHOR, NAME_SONG, ORIGINAL_TEXT, YEAR) VALUES(?, ?, ?, ?)";
        }

        int year;
        cout << "Введіть рік створення пісні: ";
        cin >> year;

        sqlite3_stmt* for_stmt;

        // Підготовка SQL-запиту
        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &for_stmt, 0);
        if (exit != SQLITE_OK) {
            cerr << "\nПомилка підготовки запиту до sql: " << sqlite3_errmsg(DB);
            return -1;
        }

        // Зв'язування параметрів запиту з даними
        sqlite3_bind_text(for_stmt, 1, author.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(for_stmt, 2, song_name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(for_stmt, 3, text_original.c_str(), -1, SQLITE_STATIC);
        if (add_translate) {
            sqlite3_bind_text(for_stmt, 4, translate_text.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(for_stmt, 5, year);
        }
        else {
            sqlite3_bind_int(for_stmt, 4, year);
        }

        // Виконання запиту
        exit = sqlite3_step(for_stmt);

        if (exit != SQLITE_DONE) {
            cerr << "\nНе вийшло виконати запит: " << sqlite3_errmsg(DB) << endl;
            return -1;
        }
        else {
            cout << "\nДодавання данних виконано успішно";
        }

        // Завершення роботи з запитом та закриття бази даних
        sqlite3_finalize(for_stmt);
        sqlite3_close(DB);
        return 0;
    }
    // Якщо користувач вибрав введення тексту вручну
    else if (quest == '2') {
        // Запит на введення перекладу
        char quest_translate;
        cout << "\n\033[36mВвести переклад Так[1] Ні[2]\033[0m\n";
        cin >> quest_translate;
        cin.ignore();

        // Введення автора та назви пісні
        string author;
        cout << "\nВведіть автора: ";
        getline(cin, author);

        string song_name;
        cout << "\nВведіть назву: ";
        getline(cin, song_name);

        // Введення тексту пісні
        string text_original;
        cout << "Введіть текст пісні: ";
        cin.ignore();
        getline(cin, text_original);

        // Підготовка SQL-запиту
        bool add_translate = false;
        string sql, translate_text;
        if (quest_translate == '1') {
            sql = "INSERT INTO MUSIC_FOR_TEST (AUTHOR, NAME_SONG, ORIGINAL_TEXT, TRANSLATE_TEXT, YEAR) VALUES(?, ?, ?, ?, ?)";
            cout << "\nВведіть переклад пісні: ";
            getline(cin, translate_text);
            add_translate = true;
        }
        else {
            sql = "INSERT INTO MUSIC_FOR_TEST (AUTHOR, NAME_SONG, ORIGINAL_TEXT, YEAR) VALUES(?, ?, ?, ?)";
        }

        int year;
        cout << "Введіть рік створення пісні: ";
        cin >> year;

        // Підключення до бази даних
        sqlite3* DB;
        int exit = sqlite3_open(path, &DB);
        if (exit != SQLITE_OK) {
            cerr << "\nПомилка відкриття: " << sqlite3_errmsg(DB) << endl;
            return -1;
        }
        else {
            cout << "\nБаза відкрита";
        }

        sqlite3_stmt* for_stmt;

        // Підготовка SQL-запиту
        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &for_stmt, 0);
        if (exit != SQLITE_OK) {
            cerr << "\nПомилка підготовки запиту до sql: " << sqlite3_errmsg(DB);
            return -1;
        }

        // Зв'язування параметрів запиту з даними
        sqlite3_bind_text(for_stmt, 1, author.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(for_stmt, 2, song_name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(for_stmt, 3, text_original.c_str(), -1, SQLITE_STATIC);
        if (add_translate) {
            sqlite3_bind_text(for_stmt, 4, translate_text.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(for_stmt, 5, year);
        }
        else {
            sqlite3_bind_int(for_stmt, 4, year);
        }

        // Виконання запиту
        exit = sqlite3_step(for_stmt);

        if (exit != SQLITE_DONE) {
            cerr << "\nНе вийшло виконати запит: " << sqlite3_errmsg(DB) << endl;
            return -1;
        }
        else {
            cout << "\nДодавання данних виконано успішно";
        }

        // Завершення роботи з запитом та закриття бази даних
        sqlite3_finalize(for_stmt);
        sqlite3_close(DB);
        return 0;
    }
}
