#pragma once
#include "Greibach.h"
#define GRB_ERROR_SERIES 600
#define NS(n)	GRB::Rule::Chain::NTermenal(n)
#define TS(n)	GRB::Rule::Chain::Terminal(n)
#define ISNS(n)	GRB::Rule::Chain::ISNS(n)
namespace GRB
{


	Greibach greibach(

		NS('S'), TS('$'),
		13,
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,// неверная структура программы 
			5,
			Rule::Chain(6, TS('t'), TS('f'), TS('i'), NS('F'), NS('B'), NS('S')),
			Rule::Chain(5, TS('p'), TS('i'), NS('F'), NS('U'), NS('S')),
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(5, TS('t'), TS('f'), TS('i'), NS('F'), NS('B')),
			Rule::Chain(4, TS('p'), TS('i'), NS('F'), NS('U'))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1, // неверная структура фунцкии 
			18,
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(8, TS('u'), TS('('), NS('C'), TS(')'), TS('{'), NS('L'), TS('}'), NS('N')),
			Rule::Chain(8, TS('w'), TS('('), NS('C'), TS(')'), TS('{'), NS('L'), TS('}'), NS('N')),
			Rule::Chain(12, TS('w'), TS('('), NS('C'), TS(')'), TS('{'), NS('L'), TS('}'), TS('!'), TS('{'), NS('L'), TS('}'), NS('N')),
			Rule::Chain(4, TS('o'), NS('I'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), NS('K'), TS(';'), NS('N')),
			Rule::Chain(3, TS('b'), TS(';'), NS('N')),
			Rule::Chain(2, TS('b'), TS(';')),
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(7, TS('u'), TS('('), NS('C'), TS(')'), TS('{'), NS('L'), TS('}')),
			Rule::Chain(7, TS('w'), TS('('), NS('C'), TS(')'), TS('{'), NS('L'), TS('}')),
			Rule::Chain(11, TS('w'), TS('('), NS('C'), TS(')'), TS('{'), NS('L'), TS('}'), TS('!'), TS('{'), NS('L'), TS('}')),
			Rule::Chain(3, TS('o'), NS('I'), TS(';')),
			Rule::Chain(3, TS('i'), NS('K'), TS(';'))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2, // ошибка в выражении
			8,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('i'), NS('K')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('i'), NS('K'), NS('M'))
		),
		Rule(
			NS('F'), GRB_ERROR_SERIES + 3,// ошибка объявления функции или процедуры
			2,
			Rule::Chain(3, TS('('), NS('P'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),
		Rule(
			NS('P'), GRB_ERROR_SERIES + 4, // ошибка в параметрах функции или процедуры
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('P'))
		),
		Rule(
			NS('B'), GRB_ERROR_SERIES + 5, //  неверная структура метода 
			2,
			Rule::Chain(6, TS('{'), NS('N'), TS('r'), NS('I'), TS(';'), TS('}')),
			Rule::Chain(5, TS('{'), TS('r'), NS('I'), TS(';'), TS('}'))
		),
		Rule(
			NS('U'), GRB_ERROR_SERIES + 6, // неверная структура процедуры 
			1,
			Rule::Chain(3, TS('{'), NS('N'), TS('}'))
		),
		Rule(
			NS('W'), GRB_ERROR_SERIES + 7, // ошибка в параметрах вызываемой функции
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		),

		Rule(
			NS('C'), GRB_ERROR_SERIES + 8,// неверное условие цикла или уловного оператора  C - нетерменал условия 
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('i'), TS('v'), TS('i')),
			Rule::Chain(3, TS('i'), TS('v'), TS('l')),
			Rule::Chain(3, TS('l'), TS('v'), TS('i'))
		),
		Rule(
			NS('K'), GRB_ERROR_SERIES + 9,// ошибка при вызове функции 
			2,
			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

		Rule(
			NS('M'), GRB_ERROR_SERIES + 10, // ошибка в операторе 
			2,
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),
		Rule(
			NS('L'), GRB_ERROR_SERIES + 11, // ошибка в теле цикла 
			9,
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('o'), NS('I'), TS(';'), NS('N')),
			Rule::Chain(3, TS('b'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), NS('K'), TS(';'), NS('N')),

			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(3, TS('o'), NS('I'), TS(';')),
			Rule::Chain(2, TS('b'), TS(';')),
			Rule::Chain(3, TS('b'), TS(';'), NS('N')),
			Rule::Chain(3, TS('i'), NS('K'), TS(';'))
		),
		Rule(
			NS('I'), GRB_ERROR_SERIES + 20,
			2,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		)
	);

	//Greibach greibach(
	//	NS('S'), // стартовый символ
	//	TS('$'),// дно стека
	//	12, // кол-во правил 
	//	Rule(NS('S'), GRB_ERROR_SERIES + 0, // неверная структура программы +
	//		5,
	//		Rule::Chain(6, TS('t'), TS('f'), TS('i'), NS('F'), NS('B'), NS('S')),
	//		Rule::Chain(5, TS('p'), TS('i'), NS('F'), NS('U'), NS('S')),
	//		Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
	//		Rule::Chain(5, TS('t'), TS('f'), TS('i'), NS('F'), NS('B')),
	//		Rule::Chain(4, TS('p'), TS('i'), NS('F'), NS('U'))
	//	),
	//	Rule(NS('N'), GRB_ERROR_SERIES + 1, // неверная конструкция в main +
	//		16,
	//		Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
	//		Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
	//		Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
	//		Rule::Chain(8, TS('u'), TS('('), NS('R'), TS(')'), TS('{'), NS('L'), TS('}'), NS('N')),
	//		Rule::Chain(8, TS('w'), TS('('), NS('R'), TS(')'), TS('{'), NS('L'), TS('}'), NS('N')),
	//		Rule::Chain(12, TS('w'), TS('('), NS('R'), TS(')'), TS('{'), NS('L'), TS('}'), TS('!'), TS('{'), NS('L'), TS('}'), NS('N')),
	//		Rule::Chain(4, TS('o'), NS('I'), TS(';'), NS('N')),
	//		Rule::Chain(4, TS('i'), NS('K'), TS(';'), NS('N')),

	//		Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
	//		Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
	//		Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
	//		Rule::Chain(7, TS('u'), TS('('), NS('R'), TS(')'), TS('{'), NS('L'), TS('}')),
	//		Rule::Chain(7, TS('w'), TS('('), NS('R'), TS(')'), TS('{'), NS('L'), TS('}')),
	//		Rule::Chain(11, TS('w'), TS('('), NS('R'), TS(')'), TS('{'), NS('L'), TS('}'), TS('!'), TS('{'), NS('L'), TS('}')),
	//		Rule::Chain(3, TS('o'), NS('I'), TS(';')),
	//		Rule::Chain(3, TS('i'), NS('K'), TS(';'))
	//	),																									    
	//	Rule(NS('E'), GRB_ERROR_SERIES + 2, // ошибка в выражении +
	//		///E->{i|l|(E)|i(W)|iM|i(W)M
	//		8,
	//		Rule::Chain(1, TS('i')),
	//		Rule::Chain(1, TS('l')),
	//		Rule::Chain(3, TS('('), NS('E'), TS(')')),
	//		Rule::Chain(2, TS('i'), NS('K')),

	//		Rule::Chain(2, TS('i'), NS('M')),
	//		Rule::Chain(2, TS('l'), NS('M')),
	//		Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
	//		Rule::Chain(3, TS('i'), NS('K'), NS('M'))
	//	),
	//	Rule(
	//		NS('F'), GRB_ERROR_SERIES + 3,// ошибка объявления функции или процедуры ?? +
	//		2,
	//		Rule::Chain(3, TS('('), NS('P'), TS(')')),
	//		Rule::Chain(2, TS('('), TS(')'))
	//	),
	//	Rule(
	//		NS('P'), GRB_ERROR_SERIES + 4,// ошибка в параметрах функции или процедуры  +
	//		2,
	//		Rule::Chain(2, TS('t'), TS('i')),
	//		Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('P'))
	//	),
	//	Rule(
	//		NS('B'), GRB_ERROR_SERIES + 5, // ошибка в теле метода +
	//		2,
	//		Rule::Chain(6, TS('{'), NS('N'), TS('r'), NS('I'), TS(';'), TS('}')),
	//		Rule::Chain(5, TS('{'), TS('r'), NS('I'), TS(';'), TS('}'))
	//	),
	//	Rule(
	//		NS('I'), GRB_ERROR_SERIES + 6,// +
	//		2,
	//		Rule::Chain(1, TS('i')),
	//		Rule::Chain(1, TS('l'))
	//	),
	//	Rule(
	//		NS('W'), GRB_ERROR_SERIES + 7,    // ошибка в параметрах вызываемой функции +
	//		4,    ///W-> i|l|iW|l,W                            
	//		Rule::Chain(1, TS('i')),
	//		Rule::Chain(1, TS('l')),
	//		Rule::Chain(3, TS('i'), TS(','), NS('W')),
	//		Rule::Chain(3, TS('l'), TS(','), NS('W'))
	//	),
	//	Rule(
	//		NS('M'), GRB_ERROR_SERIES + 8,    // оператор +
	//		2,								  //
	//		Rule::Chain(2, TS('v'), NS('E')),
	//		Rule::Chain(3, TS('v'), NS('E'), NS('M'))
	//	),
	//	Rule(
	//		NS('R'), GRB_ERROR_SERIES + 9, // ошибка в условии if +
	//		4,
	//		Rule::Chain(1, TS('i')),
	//		Rule::Chain(3, TS('i'), TS('v'), TS('i')),
	//		Rule::Chain(3, TS('i'), TS('v'), TS('l')),
	//		Rule::Chain(3, TS('l'), TS('v'), TS('i'))
	//	),
	//	Rule(
	//		NS('U'), GRB_ERROR_SERIES + 10, // неверная структура процедуры +
	//		1,
	//		Rule::Chain(3, TS('{'), NS('N'), TS('}'))
	//	),
	//	Rule(
	//		NS('K'), GRB_ERROR_SERIES + 11, // ошибка при вызове функции или метода 
	//		2,
	//		Rule::Chain(3, TS('('), NS('W'), TS(')')),
	//		Rule::Chain(2, TS('('), TS(')'))
	//	),
	//	Rule(
	//		NS('L'), GRB_ERROR_SERIES + 12, // ошибка в теле цикла или условного оператора  
	//		8,
	//		Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
	//		Rule::Chain(4, TS('o'), NS('I'), TS(';'), NS('N')),
	//		Rule::Chain(3, TS('b'), TS(';'), NS('N')),
	//		Rule::Chain(4, TS('i'), NS('K'), TS(';'), NS('N')),

	//		Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
	//		Rule::Chain(3, TS('o'), NS('I'), TS(';')),
	//		Rule::Chain(2, TS('b'), TS(';')),
	//		Rule::Chain(3, TS('i'), NS('K'), TS(';'))
	//	)
	//);







	//Greibach greibach(
	//	NS('S'), // стартовый символ
	//	TS('$'),// дно стека
	//	6, // кол-во правил 
	//	Rule(NS('S'), GRB_ERROR_SERIES + 0, // неверная структура программы 
	//		///S->m{NrE;}; | tfi(F){NrE;};S
	//		4,
	//		Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
	//		Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
	//		Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'),TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
	//		Rule::Chain(15, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'))
	//	),
	//	Rule(NS('N'), GRB_ERROR_SERIES + 1, // конструкции в функциях 
	//		///N-> pE;|i=E;|dti;N|pE;N|i=E;N|dtfi(F);N|dti(F);N
	//		12,                                                                                           																								   
	//		Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
	//		Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),													// !Терменалы! {                                                                          
	//		Rule::Chain(3, TS('w'), NS('E'), TS(';')),														    //t-тип данных, i-идентификатор,l-литерал,f-functionf function                         
	//		Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),													//d-declare, r-return, p-print, m-main }
	//		Rule::Chain(4, TS('w'), NS('E'), TS(';'), NS('N')),													//
	//		Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),									    //!Нетерменалы!{
	//		Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')),    //S-Стартовый символ,B-Тело программной конструкции		
	//		Rule::Chain(8, TS('d'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')), 
	//		Rule::Chain(4, TS('w'), TS('i'), TS(';'), NS('N')),													//С-Программная конструкция(функция или главная функция),F-Параметры
	//		Rule::Chain(4, TS('w'), TS('l'), TS(';'), NS('N')),
	//		Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
	//		Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
	//		Rule::Chain(3, TS('w'), TS('i'), TS(';')),
	//		Rule::Chain(3, TS('w'), TS('l'), TS(';'))
	//	),																									    //N-Последовательность операторов программы,M-Оператор программы
	//																											//E-Выражение,W-Подвыражение}
	//	Rule(NS('E'), GRB_ERROR_SERIES + 2, // ошибка в выражении
	//		///E->{i|l|(E)|i(W)|iM|i(W)M
	//		8,
	//		Rule::Chain(1, TS('i')),
	//		Rule::Chain(1, TS('l')),
	//		Rule::Chain(3, TS('('), NS('E'), TS(')')),
	//		Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
	//		Rule::Chain(2, TS('i'), NS('M')),
	//		Rule::Chain(2, TS('l'), NS('M')),
	//		Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
	//		Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
	//	),
	//	Rule(
	//		NS('F'), GRB_ERROR_SERIES + 3,    // ошибка в параметрах функции
	//		2,                                // 
	//		Rule::Chain(2, TS('t'), TS('i')),
	//		Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
	//	),
	//	Rule(
	//		NS('W'), GRB_ERROR_SERIES + 4,    // ошибка в параметрах вызываемой функции 
	//		4,    ///W-> i|l|iW|l,W                            
	//		Rule::Chain(1, TS('i')),
	//		Rule::Chain(1, TS('l')),
	//		Rule::Chain(3, TS('i'), TS(','), NS('W')),
	//		Rule::Chain(3, TS('l'), TS(','), NS('W'))
	//	),
	//	Rule(
	//		NS('M'), GRB_ERROR_SERIES + 5,    // оператор
	//		2,								  //
	//		Rule::Chain(2, TS('v'), NS('E')),
	//		Rule::Chain(3, TS('v'), NS('E'), NS('M'))
	//	)
	//);


}