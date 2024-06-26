#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <string>
#include <vector>
using namespace std;

int Serch_by_Word(const char* path_to_file) {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	int exit = sqlite3_open(path_to_file, &DB);
	if (exit != SQLITE_OK) {
		cerr << "\nНе вдалось відкрити БД(SW): " << sqlite3_errmsg(DB);
		return -1;
	}

	string sql = "SELECT ID, ORIGINAL_TEXT FROM MUSIC_FOR_TEST";

	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		cerr << "\nПідготовка запиту викликала помилку: " << sqlite3_errmsg(DB) << endl;
		sqlite3_close(DB);
		return -1;
	}
	
	string key_word;
	cout << "\nВведіть ключове слово: ";
	cin >> key_word;
	
	vector<int> array_id;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int id_value = sqlite3_column_int(stmt, 0);
		const unsigned char* original_text_value = sqlite3_column_text(stmt, 1);
		string original_text_str = reinterpret_cast<const char*>(original_text_value);

		if (original_text_str.find(key_word) != string::npos) {
			array_id.push_back(id_value);
		}
	}
	sqlite3_finalize(stmt);
	if (array_id.empty()) {
		cerr << "\nНе знайшлось ключове слово" << endl;
		sqlite3_close(DB);
		return -1;
	}

	sql = "SELECT ID, AUTHOR, ORIGINAL_TEXT FROM MUSIC_FOR_TEST";

	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		cerr << "\nПідготовка запиту викликала помилку: " << sqlite3_errmsg(DB) << endl;
		sqlite3_close(DB);
		return -1;
	}

	int counter_in = 0, counter_in_in = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int id_value = sqlite3_column_int(stmt, 0);
		
		if (counter_in < array_id.size()) {
			for (int i : array_id) {
				if (id_value == i) {
					const unsigned char* author_value = sqlite3_column_text(stmt, 1);
					const unsigned char* original_text_value = sqlite3_column_text(stmt, 2);

					cout << "\nID: " << id_value << endl;
					cout << "\nАвтор: " << author_value << endl;
					cout << "\nОригінальний текст: " << original_text_value << endl;
					counter_in++;
				}
			}
		}
	}

	sqlite3_finalize(stmt);
	sqlite3_close(DB);

	return 0;
}