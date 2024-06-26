#include <iostream>
#include <sqlite3.h>
#include <Windows.h>
#include <string>
#include "AddText.h"
#include "ShowDB.h"
#include "DeleteCl.h"
#include "DropTable.h"
#include "SetValue.h"
#include "Serch_Word.h"
using namespace std; 



int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	//����'������ ������� ������� ��� ���� � ������� .db
	//const char* path_file = "C:\\Date.db";
	const char* path_file = directory;

	sqlite3* DB;
	string sql = "CREATE TABLE IF NOT EXISTS MUSIC_FOR_TEST("
		"`ID` INTEGER PRIMARY KEY AUTOINCREMENT,"
		"`AUTHOR` TEXT NOT NULL,"
		"`NAME_SONG` VARCHAR(50) NOT NULL,"
		"`ORIGINAL_TEXT` TEXT NOT NULL,"
		"`TRANSLATE_TEXT` TEXT DEFAULT 1,"
		"`YEAR` INTEGER NOT NULL);";

	int exit = 0;
	exit = sqlite3_open(path_file, &DB);
	if (exit != SQLITE_OK) {
		cerr << "\n�� ������� ������� ��" << sqlite3_errmsg(DB) << endl;
		sqlite3_close(DB);
		return -1;
	}
	
	char* messageError;
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

	if (exit != SQLITE_OK) {
		cerr << "������� ��������� ��: " << sqlite3_errmsg(DB) << endl;
		sqlite3_close(DB);
		return -1;
	}
	else {
		while (true) {
			cout << "\n������ ����[1]\
\n����������� ��[2]\
\n�������� �����[3]\
\n������ ��������[4]\
\n����� ��� �� ������� ����[5}\
\n�������� �������[9]\
\n�������������[r](���� �������)\
\n�����[0]\
";
			char quest;
			cin >> quest;
			cout << endl;
			switch (quest) {
			case '1':
				AddText(path_file);
				break;
			case '2':
				ShowDB(path_file);
				break;
			case '3':
				Delete_CL(path_file);
				break;
			case '4':
				ChangeValue(path_file);
				break;
			case '5':
				Serch_by_Word(path_file);
				break;
			case '9':
				DropTable(path_file);
				break;
			case 'r':
				reindexs(DB);
				break;
			case '0':
				cout << "\nalrigth\n" << endl;
				sqlite3_close(DB);
				return 0;
			}
		}
		
	}
}
