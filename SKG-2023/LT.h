#pragma once
#include "Error.h"
#include "Parm.h"
#define LEXEMA_FIXSIZE 1	//������������� ������ ������
#define LT_MAXSIZE 4096		//���� ���������� ����� � �������
#define LT_TI_NULLIDX 0xffffffff	//��� �������� ��
#define LEX_INTEGER		't'	//������� ��� integer
#define LEX_BOOL        't' //������� ��� bool
#define LEX_STRING		't'	//������� ��� string
#define LEX_CHAR		't'	//������������ ������ ���� ������ 
#define LEX_LITERAL		'l'	//������� ��� ��������
#define LEX_ID			'i'	//������� ��� ��������������
#define LEX_FUNCTION	'f'	//������� ��� method
#define LEX_DECLARE		'd'	//������� ��� var
#define LEX_RETURN		'r'	//return
#define LEX_WRITE		'o'	//write
#define LEX_MAINFUNC	'm'	//������� ��� primary
#define LEX_CYCLE       'u' // ������ ��� cycle
#define LEX_IF          'w' // ������ ��� condition 
#define LEX_ELSE        '!' // ������ ��� otherwize
#define LEX_PROCEDURE	'p'	// ������� ��� procedure
#define LEX_BREAKL      'b' // hortab
#define LEX_SEMICOLON	';'	//;
#define LEX_COMMA		','	//,
#define LEX_LEFTBRACE	'{'	//{
#define LEX_BRACELET	'}'	//}
#define LEX_LEFTHESIS	'('	//(
#define LEX_RIGHTHESIS	')'	//)
#define LEX_OPERATOR    'v' // ������� ��� ���������
#define LEX_PLUS		'v'	//+
#define LEX_MINUS		'v'	//-
#define LEX_STAR		'v'	//*
#define LEX_VOPROS      'v' // ?
#define LEX_DIRSLASH	'v'	// /
#define LEX_RAV      	'='	//=
#define LEX_Zero            '0'//0

namespace LT	//������� ������
{
	enum operations {
		NOT = -1,
		PLUSOPER,
		MINUSOPER,
		MULOPER,
		DIVOPER,
		DIVMODOPER,
		MOREOPER,
		LESSOPER,
		EQUOPER,
		NEQUOPER
	};
	struct Entry	//������ ������� ������
	{
		char lexema;	///�������
		int strNumber;			///� ������ � ���. ������
		int idxTI;	///������ � �� ��� � LT_TI_NULLIDX
		int priority;
		operations op;
		Entry(char lexema, int strNumber, int idxTI)
		{
			this->lexema = lexema;
			this->strNumber = strNumber;
			this->idxTI = idxTI;
		}
		Entry(char lexema, int strNumber)
		{
			this->lexema = lexema;
			this->strNumber = strNumber;
		}
		Entry()
		{

		}
		~Entry()
		{

		}
	};

	struct LexTable		//��������� ������� ������
	{
		int maxSize;	///������� �� < LT_MAXSIZE
		int size;		///���. ������ �� < maxsize
		Entry* table;	///������ ����� ��
	};

	LexTable Create( // ������� ������� ������
		int size	/// ������� ������� ������ < LT_MAXSIZE
	);
	void Add(				//�������� ������ � ������� ������
		LexTable& lexTable, ///�������� ������� ������
		Entry entry			///������ ������� ������
	);

	Entry GetEntry(			//�������� ������ ������� ������
		LexTable& lexTable,	///�������� ������� ������
		int number				///����� ���������� ������
	);

	void ShowTable(LexTable& lextable, std::ostream& out);
	void Delete(LexTable& lexTable); //������� ������� ������ (���������� �����)
};
