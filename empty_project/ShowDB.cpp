#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

int Serch_by_Author(const char* path_to_file);

int ShowDB(const char* path_to_file, bool repeat_call = false, bool author_ticket_call = true) {
    // ���� �� �� ��������� ������, ��������, �� ���������� ���� �������� ����� �� �������
    if (!author_ticket_call) {
        char quest;
        cout << "\n������ �������� ����� �� ������� ���[1] ͳ[2]\n";
        cin >> quest;
        if (quest == '1') {
            Serch_by_Author(path_to_file);
            return 0;
        }
    }

    // ϳ��������� �� ���� �����
    sqlite3* DB;
    int exit = sqlite3_open(path_to_file, &DB);
    if (exit != SQLITE_OK) {
        cerr << "\n�� ������� ������� ��" << sqlite3_errmsg(DB) << endl;
        return -1;
    }
    else {
        cout << "\n�� �������!" << endl;
    }

    // ���� �� �� ��������� ������, ��������, �� ��� ���������� ���� ������
    if (!repeat_call) {
        int id = 0;
        int index_author = 1;
        int name_song = 2;
        int index_originl_text = 3;
        int index_translate_text = 4;
        int index_year = 5;

        string amounts;
        cout << "\n(������ �� ����� ���� ��� ������)\n������� \nid[1]\n������[2]\n����� ���[3]\n����������� �����[4]\n������������ ����[5]\n��[6]\n(������ � ���� �����!)\n";
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

        // ϳ�������� SQL-������
        string sql = "SELECT * FROM MUSIC_FOR_TEST";
        sqlite3_stmt* stmt;

        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        if (exit != SQLITE_OK) {
            cerr << "\n�� ������� ���������� SQL-�����: " << sqlite3_errmsg(DB) << endl;
            sqlite3_close(DB);
            return -1;
        }

        // ��������� SQL-������ �� ��������� ����������
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id_value = sqlite3_column_int(stmt, id);
            const unsigned char* author_value = sqlite3_column_text(stmt, index_author);
            const unsigned char* name_song_value = sqlite3_column_text(stmt, name_song);
            const unsigned char* original_text_value = sqlite3_column_text(stmt, index_originl_text);
            const unsigned char* translate_text_value = sqlite3_column_text(stmt, index_translate_text);
            int year_value = sqlite3_column_int(stmt, index_year);
            cout << endl;
            if (ticket1)        cout << "\nID: " << id_value;
            if (ticket2)        cout << "\n�����: " << author_value;
            if (ticket3)        cout << "\n����� ���: " << name_song_value;
            if (ticket4)        cout << "\n����������� �����: " << original_text_value;
            if (ticket5)        cout << "\n�������� ���: " << translate_text_value;
            if (ticket6)        cout << "\nг� ���������: " << year_value;
            cout << endl;
        }
        sqlite3_finalize(stmt);
    }
    else {
        // ��������� ������ ��� ������ ������
        string sql = "SELECT ID, AUTHOR FROM MUSIC_FOR_TEST";
        sqlite3_stmt* stmt;

        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        if (exit != SQLITE_OK) {
            cerr << "\n�� ������� ���������� SQL-�����: " << sqlite3_errmsg(DB) << endl;
            sqlite3_close(DB);
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id_value = sqlite3_column_int(stmt, 0);
            const unsigned char* author_value = sqlite3_column_text(stmt, 1);
            cout << "\nID: " << id_value;
            cout << "\n�����: " << author_value;
        }
    }
    sqlite3_close(DB);

    return 0;
}

int Serch_by_Author(const char* path_to_file) {
    // ϳ��������� �� ���� �����
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open(path_to_file, &DB);
    if (exit != SQLITE_OK) {
        cerr << "\n�� ������� ������� ��(SA): " << sqlite3_errmsg(DB);
        return -1;
    }

    // ������ ������� ShowDB ��� ������ ������
    ShowDB(path_to_file, true, true);

    // ����� ������� ������ � �����������
    int index;
    cout << "\n������ ������ ������: ";
    cin >> index;
    string sql = "SELECT ID, AUTHOR FROM MUSIC_FOR_TEST";

    // ϳ�������� SQL-������
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        cerr << "\nϳ�������� ������ ��������� �������: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }

    // ����������� ������ �� ��������
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

    // ����� �� ��������� ����� ������
    sql = "SELECT ID, AUTHOR, ORIGINAL_TEXT FROM MUSIC_FOR_TEST";

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        cerr << "\nϳ�������� ������ ��������� �������: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return -1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* author_value = sqlite3_column_text(stmt, 1);
        if (strcmp(reinterpret_cast<const char*>(author_value), my_author.c_str()) == 0) {
            int id_value = sqlite3_column_int(stmt, 0);
            const unsigned char* original_text = sqlite3_column_text(stmt, 2);

            cout << "\nID: " << id_value;
            cout << "\n�����: " << author_value;
            cout << "\n����������� �����: " << original_text;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return 0;
}
