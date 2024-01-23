#pragma once
#include "Error.h"
typedef short GRABALPHABET; // ������� �������� � ���������� ��������� >0, ����������� <0
namespace GRB
{
	struct Rule // ������� � ���������� �������
	{
		GRABALPHABET nn; // ����������(����� ������ �������) < 0
		int idError; // ������������� ���������������� ��������� 
		short size; // ���-�� ������� - ������ ������ ������� 

		struct Chain // ������� (������ ����� �������)
		{
			short size; // ����� ������� 
			GRABALPHABET* nt; // ������� ����������(>0) � ������������ (<0)
			Chain() { size = 0; nt = 0; };
			Chain(
				short psize, // ���-�� �������� � �������
				GRABALPHABET s, ... // ������� (�������� ��� ����������)
			);
			char* GetCChain(char* b); // �������� ������ ������� ������� 
			static GRABALPHABET Terminal(char t) { return GRABALPHABET(t); }; // �������� 
			static GRABALPHABET NTermenal(char n) { return -GRABALPHABET(n); }; // ����������
			static GRABALPHABET ISNS(char n) { return GRABALPHABET(n); };
			static bool IsTerm(GRABALPHABET s) { return s > 0; }; // ��������?
			static bool IsNotTerm(GRABALPHABET s) { return !IsTerm(s); }; // ����������?
			static char AlphabetToChar(GRABALPHABET s) { return IsTerm(s) ? char(s) : char(-s); }; // GRABALPHABET -> char
		}*chains; // ������ ������� - ������ ������ �������
		Rule() { nn = 0x00; size = 0; idError = 0; }
		Rule(
			GRABALPHABET pnn, // ���������� (<0)
			int idError, // ������������� ���������������� ��������� (Error)
			short psize, // ���-�� ������� - ������ ������ ������� 
			Chain c, ... // ��������� ������� - ������ ������ ������� 
		);

		char* GetCRule( // �������� ������� ��� ���������� 
			char* b, // ����� 
			short nchain // ����� ������� 
		);
		short GetNextChain( // �������� ��������� �� j ���������� �������, ������� ����� ��� -1
			GRABALPHABET t, //  ������ ������ �������
			Rule::Chain& pchain, // ������������ ������� 
			short j // ����� ������� 
		);
	};
	struct Greibach // ���������� ������� 
	{
		short size; // ���-�� ������ 
		GRABALPHABET startN; // ��������� ������ 
		GRABALPHABET stBottomT; // ��� ����� 
		Rule* rules; // ��������� ������ 
		Greibach() { size = 0; startN = 0; stBottomT = 0; rules = 0; };
		Greibach(
			GRABALPHABET pstartN, // ��������� ������ 
			GRABALPHABET pstBottomT, // ��� ����� 
			short psize, // ���-�� ������ 
			Rule r, ... // �������
		);
		short GetRule( // ������ ����� ������� ��� -1 
			GRABALPHABET pnn, // ����� ������ ������� 
			Rule& prule // ������� 
		);
		Rule GetRule(short n); // �� ������ - ������ 
	};
	Greibach GetGreibach(); // �������� ���������� 

}
