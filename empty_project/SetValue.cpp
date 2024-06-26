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
		cerr << "������� �������� ��" << sqlite3_errmsg << endl;
	}
	
	char quest;
	cout << "\n������ ��������� �� ���[1] ͳ[2]" << endl;
	cin >> quest;
	if (quest == '1') {
		ShowDB(path_to_file);
	}
	
	int max_id = 0;
	string sql= "SELECT MAX(ID) FROM MUSIC_FOR_TEST";
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

    int id;
    cout << "\n������ ������ �����, ���� ������ ������: ";
    cin >> id;

    if (id < 0 || id > max_id) {
        cerr << "\n�������� ID �� ������� � ������� �������������� � ��." << endl;
        sqlite3_close(DB);
        return -1;
    }

    cin.ignore();  // �������� ������ ����� ����� ���������� �����

    string row;
    cout << "\n�� ��� �� ������ ������?\n �����[1] ����� ���[2] ����������� �����[3] �������� ���[4] г�[5]\n\033[31m(������ ����� � ���� �����!)\033[0m\n";
    getline(cin, row);

    sql = "UPDATE MUSIC_FOR_TEST SET ";
    bool setClause = false;
    int paramIndex = 1;  // ������ ���������

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
        cerr << "ϳ�������� SQL-������ ��������� �������: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }

    // ����'���� ������� �� ������������
    int bindIndex = 1;  // ������ ��� ����'���� �������
    for (int i = 0; i < row.length(); i++) {
        char choice = row[i];
        if (choice == '1' || choice == '2' || choice == '3' || choice == '4' || choice == '5') {
            string newValue;
            cout << "������ ���� �������� ��� �������� ����: ";
            getline(cin, newValue);
            sqlite3_bind_text(stmt, bindIndex++, newValue.c_str(), -1, SQLITE_TRANSIENT);
        }
    }

    // ����'���� ID ��� WHERE
    sqlite3_bind_int(stmt, paramIndex, id);

    // ��������� ������
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        cerr << "������� ��� ����� �� ����: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return -1;
    }

    // ���������� ������������� ������ � �������� ��
    sqlite3_finalize(stmt);
    sqlite3_close(DB);

    cout << "����� ������ ��������!" << endl;

    return 0;
}
