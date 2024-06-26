#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <string>
#include <vector>
#include "ShowDB.h"
using namespace std;

int reindexs(sqlite3* DB);

int Delete_CL(const char* path_to_file) {
    // ϳ���������� �� ���� �����
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open(path_to_file, &DB);
    if (exit != SQLITE_OK) {
        cerr << "\n�� ������� ������� ��" << sqlite3_errmsg(DB) << endl;;
        return -1;
    }
    else {
        cout << "\n�� �������!" << endl;
    }

    // �������� ���� ����� �� �������� �����������
    char quest;
    cout << "\n������ ��������� �� ���[1] ͳ[2]" << endl;
    cin >> quest;
    if (quest == '1') {
        ShowDB(path_to_file);
    }

    // ��������� ������������� ID � �������
    int max_id = 0;
    string sql = "SELECT MAX(ID) FROM MUSIC_FOR_TEST";
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        cerr << "\n�� ������� ���������� SQL-����� ��� ��������� ������������� ID: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        max_id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    // ����� ID ��� ��������� � �����������
    int id_for_delete;
    cout << "\n������ ID ����� ��� ���������: ";
    cin >> id_for_delete;

    // �������� �������� ��������� ID
    if (id_for_delete < 0 || id_for_delete > max_id) {
        cout << "�������� ID �� ������� � ������� �������������� � ��." << endl;
        sqlite3_close(DB);
        return -1;
    }

    // ϳ�������� DELETE-������
    sql = "DELETE FROM MUSIC_FOR_TEST WHERE ID = ?";
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        cerr << "\n�� ������� ���������� SQL-����� ��� ���������: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }

    // ��'�������� ��������� ID �� ��������� DELETE-������
    sqlite3_bind_int(stmt, 1, id_for_delete);
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        cerr << "\n�� ������� �������� �����: " << sqlite3_errmsg(DB) << endl;
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

    // ��������� ���� �������
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
        cerr << "\n�� ������� �������� ���� �������: " << sqlite3_errmsg(DB) << endl;
        sqlite3_free(msgErr);
        return -1;
    }
    else {
        cout << "OK" << endl;
    }

    // ����������� ����� � ����� ������� �� ����
    sql = "INSERT INTO NEW_TABLE (AUTHOR, NAME_SONG, ORIGINAL_TEXT, TRANSLATE_TEXT, YEAR) "
        "SELECT AUTHOR, NAME_SONG, ORIGINAL_TEXT, TRANSLATE_TEXT, YEAR FROM MUSIC_FOR_TEST;";
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &msgErr);

    if (exit != SQLITE_OK) {
        cerr << "\n�� ������� ��������� ���: " << sqlite3_errmsg(DB) << endl;
        sqlite3_free(msgErr);
        return -1;
    }
    else {
        cout << "OK" << endl;
    }

    // ��������� ����� �������
    sql = "DROP TABLE MUSIC_FOR_TEST;";
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &msgErr);
    if (exit != SQLITE_OK) {
        cerr << "\n�� ������� �������� ����� �������: " << sqlite3_errmsg(DB) << endl;
        sqlite3_free(msgErr);
        return -1;
    }
    else {
        cout << "OK" << endl;
    }

    // �������������� ���� ������� �� �����
    sql = "ALTER TABLE NEW_TABLE RENAME TO MUSIC_FOR_TEST;";
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &msgErr);
    if (exit != SQLITE_OK) {
        cerr << "\n�� ������� ������������� �������: " << sqlite3_errmsg(DB) << endl;
        sqlite3_free(msgErr);
        return -1;
    }
    else {
        cout << "OK" << endl;
    }

    return 0;
}
