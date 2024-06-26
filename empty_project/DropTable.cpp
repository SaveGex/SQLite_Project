#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

int DropTable(const char* path_to_file) {
    // Оголошуємо змінні для роботи з базою даних і SQL-запитами
    sqlite3* DB;
    sqlite3_stmt* stmt;

    // Відкриваємо базу даних
    int exit = sqlite3_open(path_to_file, &DB);
    if (exit != SQLITE_OK) {
        // Якщо не вдалося відкрити базу даних, виводимо повідомлення про помилку і завершуємо роботу
        cerr << "\nНе вдалось відкрити БД: " << sqlite3_errmsg(DB) << endl;
        return -1;
    }

    // Формуємо SQL-запит для видалення таблиці
    string sql = "DROP TABLE MUSIC_FOR_TEST";

    // Виконуємо SQL-запит
    char* messErr;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messErr);
    if (exit != SQLITE_OK) {
        // Якщо сталася помилка під час виконання запиту, виводимо повідомлення про помилку і завершуємо роботу
        cerr << "Помилка видалення таблиці: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }
    else {
        // Якщо запит виконано успішно, виводимо повідомлення про успішне видалення таблиці
        cout << "Таблиця успішно видалена" << endl;
    }

    // Закриваємо базу даних
    sqlite3_close(DB);
    return 0;
}
