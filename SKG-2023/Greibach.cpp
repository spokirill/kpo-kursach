#include "pch.h"
#include <cstring>
#include "Rule.h"
#include "Greibach.h"
namespace GRB
{
	Rule::Chain::Chain(short psize, GRABALPHABET s, ...)///Предст. цепочки (правая сторона правила)
	{
		nt = new GRABALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRABALPHABET)p[i];
	};
	Rule::Rule(GRABALPHABET pnn, int piderror, short psize, Chain c, ...)//Правило в грамматике Грейбах
	{
		nn = pnn;
		idError = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++)
			chains[i] = p[i];
	};
	Greibach::Greibach(GRABALPHABET pstartN, GRABALPHABET pstBottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stBottomT = pstBottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++)
			rules[i] = p[i];
	};
	Greibach GetGreibach() { return greibach; };///Получить грамматику

	short Greibach::GetRule(GRABALPHABET pnn, Rule& prule)//Получить № правила или -1
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
			k++;
		if (k < size) prule = rules[rc = k];
		return rc;
	};

	Rule Greibach::GetRule(short n)//Получить правило по номеру
	{
		Rule rc;
		if (n < size)
			rc = rules[n];
		return rc;
	};

	char* Rule::GetCRule(char* b, short nchain)
	{
		char bchain[200];
		b[0] = Chain::AlphabetToChar(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].GetCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};

	short Rule::GetNextChain(GRABALPHABET t, Rule::Chain& pchain, short j) // Вернуть № след.за j цепочки или - 1
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)++j;
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};

	char* Rule::Chain::GetCChain(char* b)//получить цепочку в симв. виде
	{
		for (int i = 0; i < size; i++) b[i] = Chain::AlphabetToChar(nt[i]);
		b[size] = 0x00;
		return b;
	};
}