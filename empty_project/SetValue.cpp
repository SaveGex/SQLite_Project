#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <string>
#include "ShowDB.h"

using namespace std;

int ChangeValue(const char* path_to_file) {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	int exit = sqlite3_open(path_to_file, &DB);
	if (exit != SQLITE_OK) {
		cerr << "Помилка відкриття БД" << sqlite3_errmsg << endl;
	}
	
	char quest;
	cout << "\nХочете перевірити бд Так[1] Ні[2]" << endl;
	cin >> quest;
	if (quest == '1') {
		ShowDB(path_to_file);
	}
	
	int max_id = 0;
	string sql= "SELECT MAX(ID) FROM MUSIC_FOR_TEST";
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		cerr << "\nНе вдалось підготувати SQL-запит для отримання максимального ID: " << sqlite3_errmsg(DB) << endl;
		sqlite3_close(DB);
		return -1;
	}
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		max_id = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);

    int id;
    cout << "\nВведіть індекс рядка, який хочете змінити: ";
    cin >> id;

    if (id < 0 || id > max_id) {
        cerr << "\nВведений ID не входить в діапазон ідентифікаторів в БД." << endl;
        sqlite3_close(DB);
        return -1;
    }

    cin.ignore();  // Очищення буфера вводу перед отриманням рядка

    string row;
    cout << "\nЯкі дані ви хочете змінити?\n Автор[1] Назву пісні[2] Оригінальний текст[3] Переклад пісні[4] Рік[5]\n\033[31m(Введіть цифри в один рядок!)\033[0m\n";
    getline(cin, row);

    sql = "UPDATE MUSIC_FOR_TEST SET ";
    bool setClause = false;
    int paramIndex = 1;  // Індекс параметрів

    for (int i = 0; i < row.length(); i++) {
        char choice = row[i];
        if (choice == '1') {
            if (setClause) sql += ", ";
            sql += "AUTHOR = ?";
            setClause = true;
            paramIndex++;
        }
        else if (choice == '2') {
            if (setClause) sql += ", ";
            sql += "NAME_SONG = ?";
            setClause = true;
            paramIndex++;
        }
        else if (choice == '3') {
            if (setClause) sql += ", ";
            sql += "ORIGINAL_TEXT = ?";
            setClause = true;
            paramIndex++;
        }
        else if (choice == '4') {
            if (setClause) sql += ", ";
            sql += "TRANSLATE_TEXT = ?";
            setClause = true;
            paramIndex++;
        }
        else if (choice == '5') {
            if (setClause) sql += ", ";
            sql += "YEAR = ?";
            setClause = true;
            paramIndex++;
        }
    }

    sql += " WHERE ID = ?";
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        cerr << "Підготовка SQL-запиту викликала помилку: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }

    // Прив'язка значень до плейсхолдерів
    int bindIndex = 1;  // Індекс для прив'язки значень
    for (int i = 0; i < row.length(); i++) {
        char choice = row[i];
        if (choice == '1' || choice == '2' || choice == '3' || choice == '4' || choice == '5') {
            string newValue;
            cout << "Введіть нове значення для обраного поля: ";
            getline(cin, newValue);
            sqlite3_bind_text(stmt, bindIndex++, newValue.c_str(), -1, SQLITE_TRANSIENT);
        }
    }

    // Прив'язка ID для WHERE
    sqlite3_bind_int(stmt, paramIndex, id);

    // Виконання запиту
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        cerr << "Помилка при запиті на зміну: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return -1;
    }

    // Завершення підготовленого запиту і закриття БД
    sqlite3_finalize(stmt);
    sqlite3_close(DB);

    cout << "Запис успішно оновлено!" << endl;

    return 0;
}
