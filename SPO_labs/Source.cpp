#include <iostream>
#include <vector>  // �������
#include <string>  // ������
#include <fstream> // ������ � �������

#include "Lexer_2.h" // ����� �������
#include "Parser_3.h" // ����� ������
#include "Interpr_4.h" // ����� ��������������

using namespace std;

void main()
{

	ifstream in("Primer.txt"); //��������� ���� ��� ���������� ���������� 
	string s1, s2;   //���������� ����� ����������� � ������
	while (in >> s2) s1 += s2; //��������� ���� �����
	in.close(); // ��������� ����

	LEXER lexer;
	vector <string> tok = lexer.Tokens(s1);
	vector <int> tok_id = lexer.Tokens_ID(s1);
	bool tok_err = lexer.Error(s1);

	cout << "Tokens:\n" << "ID | Lexems\n-----------\n";

	for (int n = 1; n < tok_id.size(); n++) //����� ������ � �������
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

	for (int n = 1; n < RAM_INT.size(); n++) //����� �������� � �������
	{
		cout << RAM_ID[n] << " | " << RAM_INT[n] << '\n';
	}

	cout <<"\nObratnaya Polskaya Zapis:\n";

	for (int n = 1; n < Pars_Test.size(); n++) //����� ������ � �������
	{
		cout << Pars_Test[n] << ' ';
	}

	STACK Output;
	vector <int> Mashine_RAM = Output.Output_RAM(Pars_Test,RAM_ID,RAM_INT);

	cout <<"\n\nOutput:\n";

	for (int n = 1; n < Mashine_RAM.size(); n++) //����� �������� � ������� ����� ��������
	{
		cout << RAM_ID[n] << " | " << Mashine_RAM[n] << '\n';
	}

	cout << "\nBiletova IVBO-08-14\n\n";
	
	system("pause");
	return;
}