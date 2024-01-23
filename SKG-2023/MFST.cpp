#include "pch.h"
#include "Mfst.h"
#include<iostream>
using namespace std;
namespace MFST
{
#pragma region CONSTRUCTORS
	MfstState::MfstState()
	{
		lentaPosition = 0;
		numRuleChain = -1;
		nrule = -1;
	};
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		lentaPosition = pposition;
		stack = pst;
		numRuleChain = pnrulechain;
	};

	MfstState::MfstState(short pposition, short pnrule, short pnrulechain)
	{
		lentaPosition = pposition;
		nrule = pnrule;
		numRuleChain = pnrulechain;
	};
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		lentaPosition = pposition;
		stack = pst;
		nrule = pnrule;
		numRuleChain = pnrulechain;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lentaPosition = -1;
		rcStep = SURPRISE;
		nrule = -1;
		nruleChain = -1;
	};
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lentaPosition = plenta_position;//позиция на ленте
		rcStep = prc_step;//код завершения шага
		nrule = pnrule; // номер правила
		nruleChain = pnrule_chain; // номер цепочки правила
	};

	Mfst::Mfst() { lenta = 0; lentaSize = lentaPosition = 0; };

	Mfst::Mfst(Lexis::LEX plex, GRB::Greibach pgrebach)
	{
		greibach = pgrebach;
		lex = plex;
		lenta = new short[lentaSize = lex.lextable.size];
		for (int k = 0; k < lentaSize; k++)
			lenta[k] = GRB::Rule::Chain::Terminal(lex.lextable.table[k].lexema);
		lentaPosition = 0;
		st.push(greibach.stBottomT);
		st.push(greibach.startN);
		nruleChain = -1;
	};
#pragma endregion
	Mfst::RC_STEP Mfst::step(std::ostream& out)
	{

		RC_STEP rc = SURPRISE;
		if (lentaPosition < lentaSize)
		{
			if (GRB::Rule::Chain::IsNotTerm(st.top()))
			{
				GRB::Rule rule;
				if ((nrule = greibach.GetRule(st.top(), rule)) >= 0)
				{
					GRB::Rule::Chain chain;
					if ((nruleChain = rule.GetNextChain(lenta[lentaPosition], chain, nruleChain + 1)) >= 0)
					{
						MFST_TRACE1
							SaveState(out); st.pop(); PushChain(chain); rc = NS_OK;
						MFST_TRACE2
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE")
							SaveDiagnosis(NS_NORULECHAIN);
						rc = ResetState(out) ? NS_NORULECHAIN : NS_NORULE;
					};
				}
				else rc = NS_ERROR;

			}
			else if ((st.top() == lenta[lentaPosition]))
			{
				lentaPosition++;
				st.pop();
				nruleChain = -1;
				rc = TS_OK;
				MFST_TRACE3
			}
			else
			{
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN")
					rc = ResetState(out) ? TS_NOK : NS_NORULECHAIN;
			};
		}
		else
		{
			rc = LENTA_END;
			MFST_TRACE4("LENTA_END")
		};
		return rc;
	};
	bool Mfst::PushChain(GRB::Rule::Chain chain)
	{
		for (int k = chain.size - 1; k >= 0; k--)
			st.push(chain.nt[k]);
		return true;
	};

	bool Mfst::SaveState(std::ostream& out)
	{
		storeState.push(MfstState(lentaPosition, st, nrule, nruleChain));
		MFST_TRACE6("SAVESTATE:", storeState.size());
		return true;
	};

	bool Mfst::ResetState(std::ostream& out)
	{
		bool rc = false;
		MfstState state;
		if (rc = (storeState.size() > 0))
		{
			state = storeState.top();
			lentaPosition = state.lentaPosition;
			st = state.stack;
			nrule = state.nrule;
			nruleChain = state.numRuleChain;
			storeState.pop();
			MFST_TRACE5("RESSTATE")
				MFST_TRACE2
		};
		return rc;
	};

	bool Mfst::SaveDiagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;

		while (k < MFST_DIAGN_NUMBER && lentaPosition <= diagnosis[k].lentaPosition)
			k++;

		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lentaPosition, prc_step, nrule, nruleChain);

			for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
				diagnosis[i].lentaPosition = -1;
		}

		return rc;
	};

	bool Mfst::Start(std::ostream& out, std::ostream& outlog)
	{
		out << "\n-----------Работа синтаксического анализатора-----------\n" << std::endl;
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE]{};
		rc_step = step(outlog);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step(outlog);

		switch (rc_step)
		{
		case LENTA_END:
		{
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lentaSize);
			out << std::setw(4) << std::left << "всего строк " << lentaSize << ", синтаксический анализ выполнен без ошибок" << std::endl;
			rc = true;
			break;
		}

		case NS_NORULE:
		{
			MFST_TRACE4("------>NS_NORULE")
				out << "------------------------------------------------------------------------------------------   ------" << std::endl;
			out << GetDiagnosis(0, buf) << std::endl;
			out << GetDiagnosis(1, buf) << std::endl;
			out << GetDiagnosis(2, buf) << std::endl;
			break;
		}

		case NS_NORULECHAIN:	MFST_TRACE4("------>NS_NORULECHAIN") break;
		case NS_ERROR:			MFST_TRACE4("------>NS_ERROR") break;
		case SURPRISE:			MFST_TRACE4("------>NS_SURPRISE") break;


		}
		return rc;
	};

	char* Mfst::GetCSt(char* buf)
	{
		short p;
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			p = st.c[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::AlphabetToChar(p);
		}
		buf[st.size()] = '\0';
		return buf;
	}
	char* Mfst::GetCLenta(char* buf, short pos, short n)
	{
		short i = 0, k = (pos + n < lentaSize) ? pos + n : lentaSize;

		for (int i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::AlphabetToChar(lenta[i]);

		buf[i - pos] = '\0';
		return buf;
	}
	char* Mfst::GetDiagnosis(short n, char* buf)
	{
		char* rc = new char[200] {};
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lentaPosition) >= 0)
		{
			errid = greibach.GetRule(diagnosis[n].nrule).idError;
			Error::ERROR err = Error::geterror(errid);
			cout   << "Error: " << err.id <<" строка: " << lex.lextable.table[lpos].strNumber << " " << err.message << endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,	%s", err.id, lex.lextable.table[lpos].strNumber, err.message);
			

			rc = buf;
		}
		return rc;
	}
	void Mfst::PrintRules(std::ostream& out)
	{
		out << "\nпринятые правила\n";
		MfstState state;
		GRB::Rule rule;
		for (unsigned short i = 0; i < storeState.size(); i++)
		{
			state = storeState.c[i];
			rule = greibach.GetRule(state.nrule);
			MFST_TRACE7
		};
	};

	bool Mfst::SaveDeducation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storeState.size()];
		deducation.nruleChains = new short[deducation.size];
		for (unsigned short k = 0; k < storeState.size(); k++)
		{
			state = storeState.c[k];
			deducation.nrules[k] = state.nrule;
			deducation.nruleChains[k] = state.numRuleChain;
		};
		return true;
	};
	void SyntacticAnalysis(Lexis::LEX lex, Log::LOG log, std::ostream& out)
	{
		out << "\nТрассировка:\n";
		MFST_TRACE_START
			MFST::Mfst mfst(lex, GRB::GetGreibach());
		if (!mfst.Start(*log.stream, out))
		{
			std::cout << Error::geterror(612).message;
			throw ERROR_THROW(612);
		}

		mfst.SaveDeducation();
		mfst.PrintRules(out);
	};
}