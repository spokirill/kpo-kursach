#pragma once
#include<iostream>
namespace FST {

	struct RELATION
	{
		char symbol;//символ перехода
		short nnode;//номер смежной вершины
		RELATION(
			char c = 0x00,//символ перехода
			short ns = NULL//новое состояние
		);
	};

	struct NODE {//вершина графа переходов

		short n_relation;//кол-во инцедентных ребер
		RELATION* relations;//инцедентные ребра
		NODE();
		NODE(short n,
			RELATION rel, ... //список ребер
		);
	};

	struct FST//недетерминированный конечный автомат
	{
		char* string;// цепочка
		short position;//текущая позиция в цепочке
		short nstates;//кол-во состояний автомата
		NODE* nodes;//граф переходов
		short* rstates;//возможные состояния автомата
		FST(
			char* s,//цепочка
			short ns,//кол-во состояний автомата
			NODE n, ...//список состояний автомата
		);
	};

	bool step(FST& fst, short*& rstates);
	bool execute(FST fst);
}