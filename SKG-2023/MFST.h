#pragma once
#include "Greibach.h"
#include <stack>
#include "LexAnalysis.h"
#include <queue>
#include<iostream>
#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
class my_stack_SHORT :public std::stack<short> {
public:
	using std::stack<short>::c;
};
static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024];



#define MFST_TRACE_START out<< std::setw(4)<<std::left<<"���"<<":"\
								  << std::setw(20)<<std::left<<" �������"\
								  << std::setw(30)<<std::left<<" ������� �����"\
								  << std::setw(20)<<std::left<<" ����"\
								  << std::endl;

#define MFST_TRACE1		 out<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<rule.GetCRule(rbuf,nruleChain)\
								  << std::setw(30)<<std::left<<GetCLenta(lbuf,lentaPosition)\
								  << std::setw(20)<<std::left<<GetCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE2		 out<< std::setw(4)<<std::left<<FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<GetCLenta(lbuf,lentaPosition)\
								  << std::setw(20)<<std::left<<GetCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE3		 out<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<GetCLenta(lbuf,lentaPosition)\
								  << std::setw(20)<<std::left<<GetCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE4(c)		out<<std::setw(4)<<std::left << ++FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
#define MFST_TRACE5(c)		out<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
#define MFST_TRACE6(c,k)	out<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left << c << k <<std::endl;

#define MFST_TRACE7			out<<std::setw(4)<<std::left << state.lentaPosition << ": "\
							<<std::setw(20)<< std::left << rule.GetCRule(rbuf,state.numRuleChain)\
							<<std::endl;




typedef my_stack_SHORT MFSTSTSTACK; // ���� �������� 

namespace MFST
{
	struct MfstState // ��������� �������� (��� ����������)
	{
		short nrule;
		short lentaPosition; // ������� �� �����
		short numRuleChain; // ����� ������� �������, �������� �������
		MFSTSTSTACK stack;  // ���� ��������
		MfstState();
		MfstState
		(
			short position, // ������� �� ����� 
			MFSTSTSTACK pst, // ���� ��������
			short pnRuleChain // ����� ������� �������, �������� �������
		);
		MfstState(
			short position, // ������� �� ����� 
			short pnrule,
			short pnRuleChain // ����� ������� �������, �������� �������
		);
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);
	};
	struct Mfst // ���������� �������
	{
		enum RC_STEP // ��� �������� ������ step
		{
			NS_OK, // ������� ������� � �������, ������� �������� � ����
			NS_NORULE, // �� ������� ������� ���������� (������ ����������)
			NS_NORULECHAIN, // �� ������� ���������� ������� ������� (������ � �������� ����)
			NS_ERROR, // ����������� �������������� ������ ����������
			TS_OK, // ���. ������ ����� == ������� �����, ������������ �����, pop �����
			TS_NOK, // ���. ������ ����� != ������� �����, �������������� ���������
			LENTA_END,// ������� ������� ����� >= lentaSize
			SURPRISE// ����������� ��� ��������(������ � step)
		};

		struct MfstDiagnosis // �����������
		{
			short lentaPosition; // ������� � �����
			RC_STEP rcStep; // ��� ���������� ����
			short nrule; // ����� �������
			short nruleChain; // ����� ������� �������
			MfstDiagnosis();
			MfstDiagnosis( // �����������
				short plentaPosition, // ������� �� �����
				RC_STEP rcStep, // ��� ���������� ����
				short pnrule, // ����� �������
				short pnruleChain // ����� ������� �������
			);
		} diagnosis[MFST_DIAGN_NUMBER]; // ��������� ����� �������� ��������� 
		class my_stack_MfstState :public std::stack<MfstState> {
		public:
			using std::stack<MfstState>::c;
		};

		GRABALPHABET* lenta; // ��������������� (TS/NS) ����� (�� LEX)
		short lentaPosition; // ������� ������� �� �����
		short nrule; // ����� �������� �������
		short nruleChain; // ����� ������� �������, �������� �������
		short lentaSize; // ������ �����
		GRB::Greibach greibach; // ���������� �������
		Lexis::LEX lex; // ��������� ������ ������������ �����������
		MFSTSTSTACK st; // ���� ��������
		//std::stack<MfstState> storeState; // ���� ��� ���������� ���������
		my_stack_MfstState storeState;
		Mfst();
		Mfst(
			Lexis::LEX plex, // ��������� ������ ������������ �����������
			GRB::Greibach pgrebach // ���������� �������
		);
		char* GetCSt(char* buf); // �������� ���������� �����
		char* GetCLenta(char* buf, short pos, short n = 25); // �����: n �������� c pos
		char* GetDiagnosis(short n, char* buf); // �������� n-�� ������ ����������� ��� 0x00
		bool SaveState(std::ostream& out); // ��������� ��������� �������� 
		bool ResetState(std::ostream& out); // ������������ ��������� ��������
		bool PushChain(
			GRB::Rule::Chain chain // ������� �������
		);
		RC_STEP step(std::ostream& out); // ��������� ��� ��������
		bool Start(std::ostream& out, std::ostream& outlog); // ��������� �������
		bool SaveDiagnosis(
			RC_STEP pprcStep // ��� ���������� ����
		);
		void PrintRules(std::ostream& out); // ������� ������������������ ������ 
		struct Deducation // �����
		{
			short size; // ���������� ����� � ������
			short* nrules; // ������ ������ ����������
			short* nruleChains; // ������ ������� ������ ����������
			Deducation() { size = 0; nrules = 0; nruleChains = 0; };
		} deducation;
		bool SaveDeducation(); // ��������� ������ ������
	};
	void SyntacticAnalysis(Lexis::LEX lex, Log::LOG log, std::ostream& out);
}