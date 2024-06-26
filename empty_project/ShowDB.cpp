#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

int Serch_by_Author(const char* path_to_file);

int ShowDB(const char* path_to_file, bool repeat_call = false, bool author_ticket_call = true) {
    // Якщо це не повторний виклик, запитуємо, чи користувач хоче здійснити пошук за автором
    if (!author_ticket_call) {
        char quest;
        cout << "\nХочете здійснити пошук за автором Так[1] Ні[2]\n";
        cin >> quest;
        if (quest == '1') {
            Serch_by_Author(path_to_file);
            return 0;
        }
    }

    // Підключення до бази даних
    sqlite3* DB;
    int exit = sqlite3_open(path_to_file, &DB);
    if (exit != SQLITE_OK) {
        cerr << "\nНе вдалось відкрити БД" << sqlite3_errmsg(DB) << endl;
        return -1;
    }
    else {
        cout << "\nБД відкрита!" << endl;
    }

    // Якщо це не повторний виклик, запитуємо, які дані користувач хоче бачити
    if (!repeat_call) {
        int id = 0;
        int index_author = 1;
        int name_song = 2;
        int index_originl_text = 3;
        int index_translate_text = 4;
        int index_year = 5;

        string amounts;
        cout << "\n(введіть всі цифри котрі вам потрібні)\nВивести \nid[1]\nавтора[2]\nназву пісні[3]\nоригінальний текст[4]\nперекладаний текс[5]\nрік[6]\n(Ввести в один рядок!)\n";
        cin.ignore();
        getline(cin, amounts);

        bool ticket1 = false, ticket2 = false, ticket3 = false, ticket4 = false, ticket5 = false, ticket6 = false;
        for (int i = 0; i < amounts.length(); i++) {
            if (amounts[i] == '1') {
                ticket1 = true;
            }
            else if (amounts[i] == '2') {
                ticket2 = true;
            }
            else if (amounts[i] == '3') {
                ticket3 = true;
            }
            else if (amounts[i] == '4') {
                ticket4 = true;
            }
            else if (amounts[i] == '5') {
                ticket5 = true;
            }
            else if (amounts[i] == '6') {
                ticket6 = true;
            }
        }

        // Підготовка SQL-запиту
        string sql = "SELECT * FROM MUSIC_FOR_TEST";
        sqlite3_stmt* stmt;

        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        if (exit != SQLITE_OK) {
            cerr << "\nНе вдалось підготувати SQL-запит: " << sqlite3_errmsg(DB) << endl;
            sqlite3_close(DB);
            return -1;
        }

        // Виконання SQL-запиту та виведення результатів
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id_value = sqlite3_column_int(stmt, id);
            const unsigned char* author_value = sqlite3_column_text(stmt, index_author);
            const unsigned char* name_song_value = sqlite3_column_text(stmt, name_song);
            const unsigned char* original_text_value = sqlite3_column_text(stmt, index_originl_text);
            const unsigned char* translate_text_value = sqlite3_column_text(stmt, index_translate_text);
            int year_value = sqlite3_column_int(stmt, index_year);
            cout << endl;
            if (ticket1)        cout << "\nID: " << id_value;
            if (ticket2)        cout << "\nАвтор: " << author_value;
            if (ticket3)        cout << "\nНазва пісні: " << name_song_value;
            if (ticket4)        cout << "\nОригінальний текст: " << original_text_value;
            if (ticket5)        cout << "\nПереклад пісні: " << translate_text_value;
            if (ticket6)        cout << "\nРік написання: " << year_value;
            cout << endl;
        }
        sqlite3_finalize(stmt);
    }
    else {
        // Повторний виклик для показу авторів
        string sql = "SELECT ID, AUTHOR FROM MUSIC_FOR_TEST";
        sqlite3_stmt* stmt;

        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        if (exit != SQLITE_OK) {
            cerr << "\nНе вдалось підготувати SQL-запит: " << sqlite3_errmsg(DB) << endl;
            sqlite3_close(DB);
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id_value = sqlite3_column_int(stmt, 0);
            const unsigned char* author_value = sqlite3_column_text(stmt, 1);
            cout << "\nID: " << id_value;
            cout << "\nАвтор: " << author_value;
        }
    }
    sqlite3_close(DB);

    return 0;
}

int Serch_by_Author(const char* path_to_file) {
    // Підключення до бази даних
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open(path_to_file, &DB);
    if (exit != SQLITE_OK) {
        cerr << "\nНе вдалось відкрити БД(SA): " << sqlite3_errmsg(DB);
        return -1;
    }

    // Виклик функції ShowDB для показу авторів
    ShowDB(path_to_file, true, true);

    // Запит індексу автора у користувача
    int index;
    cout << "\nВведіть індекс автора: ";
    cin >> index;
    string sql = "SELECT ID, AUTHOR FROM MUSIC_FOR_TEST";

    // Підготовка SQL-запиту
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        cerr << "\nПідготовка запиту викликала помилку: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }

    // Знаходження автора за індексом
    string my_author;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id_value = sqlite3_column_int(stmt, 0);
        const unsigned char* author_value = sqlite3_column_text(stmt, 1);
        if (id_value == index) {
            my_author = reinterpret_cast<const char*>(author_value);
            break;
        }
    }
    sqlite3_finalize(stmt);

    // Пошук та виведення пісень автора
    sql = "SELECT ID, AUTHOR, ORIGINAL_TEXT FROM MUSIC_FOR_TEST";

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        cerr << "\nПідготовка запиту викликала помилку: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* author_value = sqlite3_column_text(stmt, 1);
        if (strcmp(reinterpret_cast<const char*>(author_value), my_author.c_str()) == 0) {
            int id_value = sqlite3_column_int(stmt, 0);
            const unsigned char* original_text = sqlite3_column_text(stmt, 2);

            cout << "\nID: " << id_value;
            cout << "\nАвтор: " << author_value;
            cout << "\nОригінальний текст: " << original_text;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return 0;
}
