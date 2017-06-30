#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <regex>   // Регулярные выражения
#include <fstream> // Работа с файлами
using namespace std;

class PARSER
{

public:

	void Set(vector <string> Tokens_Str, vector <int> Tokens_ID_Str)		//Начальные установки
	{
		Set_Liner();
		Tokens = Tokens_Str;
		Tokens_ID = Tokens_ID_Str;
	}

	vector <string> Init_ID()		//Возвращает идентификаторы ОЗУ
	{
		return ID_BUFFER;
	}

	vector <int> Init_INT()			//Возвращает интовые значения ОЗУ
	{
		return INT_BUFFER;
	}

	int Init_N()
	{
		set_n(1);
		Init();
		return n;
	}

	vector <string> Output()		//Возвращает полиз
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
	
	void Statement() //Тело программы
	{
		n++;
		switch (Tokens_ID[n])
		{
			// ; точка с запятой просто пропускается
			case 1:
			case 3: return;

			case 4: //Встречена скобка { открывает вложенную программу
				Program();
				return;

			case 5: //}	просто выходит из подпрограммы
				LastClose = true;
				return;

			case 9: //Встречен if выполняет функцию для if
				IF(); 
				return;

			case 15: //Встречен ID выполняет функцию для ID
				ID();
			default: 
				return; //Если встречен неизвестный символ просто выходит
		}
	}

	void ID() //Встречен ID
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

	void PR() //Встречено присваивание :=
	{
		Char_Stack.push_back(Tokens[n]); //Присваивание извлекается последним по приоритету
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

	void IF() //Встречен IF
	{
		int F0, Addr_F0;
		n++;
		if (Tokens_ID[n] == 2)
		{
			Logic(); // Выражения пр типу (a<b)
			liner.push_back("$!");	//Переход по лжи
			liner.push_back("F0");	//Временное значения в ячейке будущей метки
			Addr_F0 = liner.size() - 1; //Сохраняем адрес для записи метки
		}
		else		// Если встречен не тот символ - ошибка
		{
			cout << "Syntax error in ['(' Logic]!" << '\n';
			system("pause");
			exit(0);
		}

		Statement(); //Содержимое IF

		liner.push_back("@");
		F0 = liner.size() - 1;

		liner[Addr_F0] = to_string(F0);		//Записываем адрес метки в полиз для перехода
		return;
	}

	void Logic() // Только операции типа (ID/INT > < = ID/INT)					//Посимвольно проверяет соответствие форме логического выражения
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

	void OP() //Алгоритм Дейкстры
	{
		while (Tokens_ID[n] != 1)
		{
			switch (Tokens_ID[n])
			{
			case 2: // (	Записывает в стек STOP
			{
				Char_Stack.push_back("STOP");
				n++;
				OP();
				break;
			}
			case 3: // )	Выносит все из стека пока не встретит STOP
			{
				while (Char_Stack.back() != "STOP")
				{
					liner.push_back(Char_Stack.back());
					Char_Stack.pop_back();
				}
				Char_Stack.pop_back(); return;
			}
			case 6: // Символы + - выносят * и / из стека
			{
				if ((Char_Stack.back() == "/") || (Char_Stack.back() == "*"))
				{
					liner.push_back(Char_Stack.back());
					Char_Stack.pop_back();
				}
				Char_Stack.push_back(Tokens[n]); break;
			}
			case 14: // INT - просто записывается напрямую в вывод
			{
				liner.push_back(Tokens[n]);
				break;
			}
			case 15: // ID - просто записываются напрямую в вывод
			{
				liner.push_back(Tokens[n]);
				break;
			}
			case 16: // * /	- просто заносятся в стек
			{
				Char_Stack.push_back(Tokens[n]);
				break;
			}
			default: //Неожиданный символ - ошибка
			{
				cout << "Syntax error in [Dijkstra's algorithm!]" << '\n';
				system("pause");
				exit(0);
			}

			}
			n++;
		}

		{
		    if (Tokens_ID[n] == 1) // Символ ; выносит все символы что остались из стека
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

	void Init()			//Инициализация переменной происходит в отдельнмо блоке, в начале программы в виду int{ID; ID=N;}
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
