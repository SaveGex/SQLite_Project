#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <string>
#include <vector>
#include "ShowDB.h"
using namespace std;

int reindexs(sqlite3* DB);

int Delete_CL(const char* path_to_file) {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	int exit = sqlite3_open(path_to_file, &DB);
	if (exit != SQLITE_OK) {
		cerr << "\nНе вдалось відкрити БД" << sqlite3_errmsg(DB) << endl;;
		return -1;
	}
	else {
		cout << "\nБД відкрита!" << endl;
	}

	char quest;
	cout << "\nХочете перевірити бд Так[1] Ні[2]" << endl;
	cin >> quest;
	if (quest == '1') {
		ShowDB(path_to_file);
	}

	int max_id = 0;
	string sql = "SELECT MAX(ID) FROM MUSIC_FOR_TEST";
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

	// Prompt user for ID to delete
	int id_for_delete;
	cout << "\nВведіть ID рядка для видалення: ";
	cin >> id_for_delete;

	// Validate user input
	if (id_for_delete < 0 || id_for_delete > max_id) {
		cout << "Введений ID не входить в діапазон ідентифікаторів в БД." << endl;
		sqlite3_close(DB);
		return -1;
	}

	// Prepare DELETE statement
	sql = "DELETE FROM MUSIC_FOR_TEST WHERE ID = ?";
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		cerr << "\nНе вдалось підготувати SQL-запит для видалення: " << sqlite3_errmsg(DB) << endl;
		sqlite3_close(DB);
		return -1;
	}

	// Bind ID parameter and execute DELETE statement
	sqlite3_bind_int(stmt, 1, id_for_delete);
	exit = sqlite3_step(stmt);
	if (exit != SQLITE_DONE) {
		cerr << "\nНе вдалося видалити рядок: " << sqlite3_errmsg(DB) << endl;
		sqlite3_finalize(stmt);
		sqlite3_close(DB);
		return -1;
	}

	sqlite3_finalize(stmt);
	reindexs(DB);
	sqlite3_close(DB);

	return 0;
}


int reindexs(sqlite3* DB) {
	sqlite3_stmt* stmt;
	string sql = "CREATE TABLE IF NOT EXISTS NEW_TABLE("
		"`ID` INTEGER PRIMARY KEY AUTOINCREMENT,"
		"`AUTHOR` TEXT NOT NULL,"
		"`NAME_SONG` VARCHAR(50) NOT NULL,"
		"`ORIGINAL_TEXT` TEXT NOT NULL,"
		"`TRANSLATE_TEXT` TEXT DEFAULT 1,"
		"`YEAR` INTEGER NOT NULL);";
	char* msgErr;
	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &msgErr);

	if (exit != SQLITE_OK) {
		cerr << "\nFailed to prepare SQL query: " << sqlite3_errmsg(DB) << endl;
		sqlite3_free(msgErr);
		return -1;
	}
	else {
		cout << "OK" << endl;
	}

	sql = "INSERT INTO NEW_TABLE (AUTHOR, NAME_SONG, ORIGINAL_TEXT, TRANSLATE_TEXT, YEAR) "
		"SELECT AUTHOR, NAME_SONG, ORIGINAL_TEXT, TRANSLATE_TEXT, YEAR FROM MUSIC_FOR_TEST;";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &msgErr);

	if (exit != SQLITE_OK) {
		cerr << "\nFailed to prepare SQL query: " << sqlite3_errmsg(DB) << endl;
		sqlite3_free(msgErr);
		return -1;
	}
	else {
		cout << "OK" << endl;
	}

	sql = "DROP TABLE MUSIC_FOR_TEST;";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &msgErr);
	if (exit != SQLITE_OK) {
		cerr << "\nНе вдалось підготувати SQL-запит2: " << sqlite3_errmsg(DB) << endl;
		sqlite3_free(msgErr);
		return -1;
	}
	else {
		cout << "OK" << endl;
	}

	sql = "ALTER TABLE NEW_TABLE RENAME TO MUSIC_FOR_TEST;";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &msgErr);
	if (exit != SQLITE_OK) {
		cerr << "\nНе вдалося перейменувати таблицю3: " << sqlite3_errmsg(DB) << endl;
		sqlite3_free(msgErr);
		return -1;
	}
	else {
		cout << "OK" << endl;
	}

	return 0;
	//
	//vector <string> array_author;

	//while (sqlite3_step(stmt) == SQLITE_ROW) {

	//	const unsigned char* author_text = sqlite3_column_text(stmt, 0);
	//	if (author_text) {
	//		string author = reinterpret_cast<const char*>(author_text);
	//		array_author.push_back(author);
	//	}
	//	else {
	//		cerr << "Помилка отримання значення AUTHOR." << endl;
	//	}
	//}

	//int size_array = array_author.size();

	//sql = "UPDATE MUSIC_FOR_TEST SET ID = ? WHERE ID = ?;";
	//exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);

	//if (exit != SQLITE_OK) {
	//	cerr << "\nНе вдалось підготувати SQL-запит для оновлення: " << sqlite3_errmsg(DB) << endl;
	//	sqlite3_close(DB);
	//	return -1;
	//}
	//
	//int index = 1;
	//vector<int> all;

	//for (int i = 0; i < size_array; i++) {
	//	sqlite3_bind_int(stmt, 1, index);

	//	// Виконуємо запит
	//	exit = sqlite3_step(stmt);

	//	if (exit != SQLITE_DONE) {
	//		cerr << "Не вдалось виконати SQL-запит: " << sqlite3_errmsg(DB) << endl;
	//		sqlite3_finalize(stmt);
	//		sqlite3_close(DB);
	//		return -1;
	//	}

	//	// Зберігаємо результат вектора all
	//	all.push_back(exit);

	//	// Скидаємо зв'язані параметри для повторного використання stmt
	//	sqlite3_reset(stmt);

	//	// Інкрементуємо індекс для наступного рядка
	//	index++;
	//}

	//bool ticket = true;
	//for (int i : all) {

	//	if (i != SQLITE_DONE) {
	//		ticket = false;
	//		break;
	//	}
	//}
	//if (exit != SQLITE_OK) {
	//	cerr << "Рядок вижив: " << sqlite3_errmsg(DB) << endl;
	//}
	//else {
	//	cout << "Рядок успішно видалено" << endl;
	//}
	return 0;
}
