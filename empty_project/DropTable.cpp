#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

int DropTable(const char* path_to_file) {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	int exit = sqlite3_open(path_to_file, &DB);
	if (exit != SQLITE_OK) {
		cerr << "\nНе вдалось відкрити БД" << sqlite3_errmsg(DB) << endl;;
		return -1;
	}
	
	string sql = "DROP TABLE MUSIC_FOR_TEST";

	char* messErr;
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messErr);
	if (exit != SQLITE_OK) {
		cerr << "Помилка видалення БД: " << sqlite3_errmsg(DB) << endl;
		sqlite3_close(DB);
		return -1;
	}
	else {
		cout << "Таблиця успішно видалена" << endl;
	}

	sqlite3_close(DB);
	return 0;
}