#include <iostream>
#include <vector>  // �������
#include <string>  // ������
#include <regex>   // ���������� ���������
#include <fstream> // ������ � �������
using namespace std;

class PARSER
{

public:

	void Set(vector <string> Tokens_Str, vector <int> Tokens_ID_Str)		//��������� ���������
	{
		Set_Liner();
		Tokens = Tokens_Str;
		Tokens_ID = Tokens_ID_Str;
	}

	vector <string> Init_ID()		//���������� �������������� ���
	{
		return ID_BUFFER;
	}

	vector <int> Init_INT()			//���������� ������� �������� ���
	{
		return INT_BUFFER;
	}

	int Init_N()
	{
		set_n(1);
		Init();
		return n;
	}

	vector <string> Output()		//���������� �����
	{
		set_n(Init_N() + 1);
		Program();
		return (liner);
	}

private:
	
	vector <string> Tokens, liner, ID_BUFFER, Char_Stack;
	vector <int> Tokens_ID, INT_BUFFER;

	int n;

	bool Err_Flag;

	bool LastClose = false;

	void set_n(int count) { n = count; }
	
	void Set_Liner() { liner.resize(1, "START"); Char_Stack.resize(1, "START"); ID_BUFFER.resize(1, "START"); INT_BUFFER.resize(1, 0); }

	void Program()
	{
		while ((Tokens_ID[n] != 5) || (LastClose == false)) { Statement(); }
		LastClose = false;
		return;
	}
	
	void Statement() //���� ���������
	{
		n++;
		switch (Tokens_ID[n])
		{
			// ; ����� � ������� ������ ������������
			case 1:
			case 3: return;

			case 4: //��������� ������ { ��������� ��������� ���������
				Program();
				return;

			case 5: //}	������ ������� �� ������������
				LastClose = true;
				return;

			case 9: //�������� if ��������� ������� ��� if
				IF(); 
				return;

			case 15: //�������� ID ��������� ������� ��� ID
				ID();
			default: 
				return; //���� �������� ����������� ������ ������ �������
		}
	}

	void ID() //�������� ID
	{
		liner.push_back(Tokens[n]);
		n++;
		if (Tokens_ID[n] == 8) PR();
		else
		{
			cout << "Syntax error in [ID]!" << '\n';
			n--;
		}
		return;
	}

	void PR() //��������� ������������ :=
	{
		Char_Stack.push_back(Tokens[n]); //������������ ����������� ��������� �� ����������
		n++;
		if ((Tokens_ID[n] == 14) || (Tokens_ID[n] == 15)|| (Tokens_ID[n] == 2))
		{ 
			OP(); 
		}
		else
		{
			cout << "Syntax error in [:=]!" << '\n';
			system("pause");
			exit(0);
		}
		return;
	}

	void IF() //�������� IF
	{
		int F0, Addr_F0;
		n++;
		if (Tokens_ID[n] == 2)
		{
			Logic(); // ��������� �� ���� (a<b)
			liner.push_back("$!");	//������� �� ���
			liner.push_back("F0");	//��������� �������� � ������ ������� �����
			Addr_F0 = liner.size() - 1; //��������� ����� ��� ������ �����
		}
		else		// ���� �������� �� ��� ������ - ������
		{
			cout << "Syntax error in ['(' Logic]!" << '\n';
			system("pause");
			exit(0);
		}

		Statement(); //���������� IF

		liner.push_back("@");
		F0 = liner.size() - 1;

		liner[Addr_F0] = to_string(F0);		//���������� ����� ����� � ����� ��� ��������
		return;
	}

	void Logic() // ������ �������� ���� (ID/INT > < = ID/INT)					//����������� ��������� ������������ ����� ����������� ���������
	{
			n++;
			if ((Tokens_ID[n] == 15) | (Tokens_ID[n] == 14)) liner.push_back(Tokens[n]);
			else 
			{
				cout << "Syntax error in [1st Operand Logic]!" << '\n';
				system("pause");
				exit(0);
			}
			n += 2;
			if ((Tokens_ID[n] == 15) | (Tokens_ID[n] == 14)) liner.push_back(Tokens[n]);
			else
			{
				cout << "Syntax error in [2d Operand Logic]!" << '\n';
				system("pause");
				exit(0);
			}
			n--;
			if (Tokens_ID[n] == 7) liner.push_back(Tokens[n]);
			else
			{
				cout << "Syntax error in [Operator Logic]!" << '\n';
				system("pause");
				exit(0);
			}
			n += 2;
			if (Tokens_ID[n] == 3); //liner.push_back(Tokens[n]); //(
			else
			{
				cout << "Syntax error in [')' Logic]!" << '\n';
				system("pause");
				exit(0);
			}
			return;
	}

	void OP() //�������� ��������
	{
		while (Tokens_ID[n] != 1)
		{
			switch (Tokens_ID[n])
			{
			case 2: // (	���������� � ���� STOP
			{
				Char_Stack.push_back("STOP");
				n++;
				OP();
				break;
			}
			case 3: // )	������� ��� �� ����� ���� �� �������� STOP
			{
				while (Char_Stack.back() != "STOP")
				{
					liner.push_back(Char_Stack.back());
					Char_Stack.pop_back();
				}
				Char_Stack.pop_back(); return;
			}
			case 6: // ������� + - ������� * � / �� �����
			{
				if ((Char_Stack.back() == "/") || (Char_Stack.back() == "*"))
				{
					liner.push_back(Char_Stack.back());
					Char_Stack.pop_back();
				}
				Char_Stack.push_back(Tokens[n]); break;
			}
			case 14: // INT - ������ ������������ �������� � �����
			{
				liner.push_back(Tokens[n]);
				break;
			}
			case 15: // ID - ������ ������������ �������� � �����
			{
				liner.push_back(Tokens[n]);
				break;
			}
			case 16: // * /	- ������ ��������� � ����
			{
				Char_Stack.push_back(Tokens[n]);
				break;
			}
			default: //����������� ������ - ������
			{
				cout << "Syntax error in [Dijkstra's algorithm!]" << '\n';
				system("pause");
				exit(0);
			}

			}
			n++;
		}

		{
		    if (Tokens_ID[n] == 1) // ������ ; ������� ��� ������� ��� �������� �� �����
			while (Char_Stack.back() != "START")
			{
				liner.push_back(Char_Stack.back());
				Char_Stack.pop_back();
			}
			else
			{
				cout << "Syntax error in [Dijkstra's algorithm!]" << '\n';
				system("pause");
				exit(0);
			}
		}
		n--;
		return;
	}

	void Init()			//������������� ���������� ���������� � ��������� �����, � ������ ��������� � ���� int{ID; ID=N;}
	{
	
		if (Tokens_ID[n] == 10) n++;
		else
		{
			cout << "Syntax error in [Init 'int']!" << '\n';
			system("pause");
			exit(0);
		}
		if (Tokens_ID[n] == 4)
		{
			n++;
			while (Tokens_ID[n] != 5)
			{
				if (Tokens_ID[n] == 15) 
				{
					ID_BUFFER.push_back(Tokens[n]);
					n++;
					switch (Tokens_ID[n])
					{
					case 1:
					{
						INT_BUFFER.push_back(0);
						break;
					}
					case 8:
					{
						n++;
						if (Tokens_ID[n] == 14) INT_BUFFER.push_back(stoi(Tokens[n]));
						else
						{
						cout << "Syntax error in [Init after ':=']!" << '\n';
						system("pause");
						exit(0);
						}
						n++;
						if (Tokens_ID[n] == 1){}
						else
						{
							cout << "Syntax error in [Init after in ';' after int]!" << '\n';
							system("pause");
							exit(0);
						}
						break;
					}
					default:
					{
						cout << "Syntax error in [Init after ID]!" << '\n';
						system("pause");
						exit(0);
					}
					}
					n++;
				}
			}
			return;
		}
		else;
		{
			cout << "Syntax error in [Init '{']!" << '\n';
			system("pause");
			exit(0);
		}

	}

};
