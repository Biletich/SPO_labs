#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <fstream> // Работа с файлами

#include "Lexer_2.h" // Класс лексера
#include "Parser_3.h" // Класс парсер
#include "Interpr_4.h" // Класс интерпретатора

using namespace std;

void main()
{

	ifstream in("Primer.txt"); //Открываем файл для считывания информации 
	string s1, s2;   //Переменная будет считываться в строку
	while (in >> s2) s1 += s2; //Считываем пока можем
	in.close(); // Закрываем файл

	LEXER lexer;
	vector <string> tok = lexer.Tokens(s1);
	vector <int> tok_id = lexer.Tokens_ID(s1);
	bool tok_err = lexer.Error(s1);

	cout << "Tokens:\n" << "ID | Lexems\n-----------\n";

	for (int n = 1; n < tok_id.size(); n++) //Вывод лексем в консоль
	{
		cout << tok_id[n];
		if (tok_id[n] < 10) cout << "  | ";
		else cout << " | ";
		cout << tok[n] << '\n';
	}

	if (tok_err == true)
		cout << "Lexical error!" << '\n';

	PARSER Parser;
	Parser.Set(tok, tok_id);
	vector <string> Pars_Test = Parser.Output();
	vector <int> RAM_INT = Parser.Init_INT();
	vector <string>  RAM_ID = Parser.Init_ID();


	cout << "\nInitializacia:\n";

	for (int n = 1; n < RAM_INT.size(); n++) //Вывод значений в консоль
	{
		cout << RAM_ID[n] << " | " << RAM_INT[n] << '\n';
	}

	cout <<"\nObratnaya Polskaya Zapis:\n";

	for (int n = 1; n < Pars_Test.size(); n++) //Вывод лексем в консоль
	{
		cout << Pars_Test[n] << ' ';
	}

	STACK Output;
	vector <int> Mashine_RAM = Output.Output_RAM(Pars_Test,RAM_ID,RAM_INT);

	cout <<"\n\nOutput:\n";

	for (int n = 1; n < Mashine_RAM.size(); n++) //Вывод значений в консоль после расчетов
	{
		cout << RAM_ID[n] << " | " << Mashine_RAM[n] << '\n';
	}

	cout << "\nBiletova IVBO-08-14\n\n";
	
	system("pause");
	return;
}