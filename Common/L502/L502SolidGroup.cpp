#include "stdafx.h"
#include "L502SolidGroup.h"
#include "App/AppBase.h"
#include "App/Config.h"
#ifndef DEBUG_ITEMS
#include "l502api.h"
#pragma comment(lib, "l502api")

#include <stdio.h>

int xprint(...);
#define dprint sizeof xprint

L502SolidGroup::L502SolidGroup()
	: ADC_FREQ(Singleton<SolenoidParametersTable>::Instance().items.get<Frequency502>().value)
	, referenceV(Singleton<SolenoidParametersTable>::Instance().items.get<RangeReferenceSignal>().value)
	, dataV(Singleton<SolenoidParametersTable>::Instance().items.get<InputRangeSignal>().value)
	, READ_TIMEOUT(1500)
{
	/*
	for(int i = 0; i < count_inputs; ++i)
	{
		f_channels[i] = i;
		f_ch_modes[i] = L502_LCH_MODE_COMM;
		f_ch_ranges[i] = L502_ADC_RANGE_10;
	}
	*/
}

bool L502SolidGroup::Init()
{
	hnd = L502_Create();
	if (hnd==NULL)
	{
		dprint("error module!");
	}
	else
	{
		int err = L502_Open((t_l502_hnd)hnd, NULL);
		if (err)
		{
			dprint("error connection with module: %s!", L502_GetErrorString(err));
			L502_Free((t_l502_hnd)hnd);
			hnd = NULL;
		}
	}
	return NULL != hnd;
}

void L502SolidGroup::Destroy()
{
	 /* ��������� ����� � ������� */
        L502_Close((t_l502_hnd)hnd);
        /* ����������� ��������� */
        L502_Free((t_l502_hnd)hnd);
}

int L502SolidGroup::SetupParams()
{
	int f_channels[] = {0, 1};
	int f_ch_modes[] = {L502_LCH_MODE_COMM, L502_LCH_MODE_COMM};
	int f_ch_ranges[] = {referenceV, dataV};

	static const int count_inputs = dimention_of(f_channels);
	int err = L502_SetLChannelCount((t_l502_hnd)hnd, count_inputs);
	for (int i=0; (i < count_inputs) && !err; i++)
        err = L502_SetLChannel((t_l502_hnd)hnd, i, f_channels[i], f_ch_modes[i], f_ch_ranges[i], 0);
	/* ������������� ������� ����� ��� ��� � �������� ������ */
	double f_adc = ADC_FREQ;
	double f_frame = (double)ADC_FREQ/count_inputs;
    if (!err)
    {
        err = L502_SetAdcFreq((t_l502_hnd)hnd, &f_adc, &f_frame);
    }

    /* ���������� ��������� � ������ */
    if (!err)
	{
		dprint("frequency adc = %0.0f frequency chenell = %0.0f", f_adc, f_frame);
        err = L502_Configure((t_l502_hnd)hnd, 0);
	}

    /* ��������� ���������� ������ */
    if (!err)
    {
        err = L502_StreamsEnable((t_l502_hnd)hnd, L502_STREAM_ADC);
    }

    return err;
}
int L502SolidGroup::Start()
{
	int err = L502_StreamsStart((t_l502_hnd)hnd);
	if (err)
		dprint("error collections date: %s!", L502_GetErrorString(err));
	return err;
}

int L502SolidGroup::Stop()
{
	/* ������������� ����� ����� ������ (���������� �� ����, ���� �� ������) */
	int err = L502_StreamsStop((t_l502_hnd)hnd);
	if (err)
	{
		dprint("error colecton date: %s", L502_GetErrorString(err));
	}
	return err;
}

int L502SolidGroup::Read(unsigned &startChennel, double *data, unsigned &count)
{
	unsigned rcv_buf[buffer_length];
	int cnt = L502_Recv((t_l502_hnd)hnd, rcv_buf, buffer_length, READ_TIMEOUT);
	if(cnt > 0)
	{
		L502_GetNextExpectedLchNum((t_l502_hnd)hnd, &startChennel);
		int err = L502_ProcessData((t_l502_hnd)hnd, rcv_buf, cnt, L502_PROC_FLAGS_VOLT,
			data, &count, NULL, NULL);
		if (err)
		{
			dprint("error computing date: %s", L502_GetErrorString(err));
		}
		return err;
	}
	return cnt;
}
#else
L502SolidGroup::L502SolidGroup()
	: ADC_FREQ(Singleton<SolenoidParametersTable>::Instance().items.get<Frequency502>().value)
	, referenceV(Singleton<SolenoidParametersTable>::Instance().items.get<RangeReferenceSignal>().value)
	, dataV(Singleton<SolenoidParametersTable>::Instance().items.get<InputRangeSignal>().value)
	, READ_TIMEOUT(1500)
{}
bool  L502SolidGroup::Init(){return 0;}
void  L502SolidGroup::Destroy(){}
int   L502SolidGroup::SetupParams(){return 0;}
int   L502SolidGroup::Start(){return 0;}
int   L502SolidGroup::Stop(){return 0;}
int L502SolidGroup::Read(unsigned &startChennel, double *data, unsigned &count){return 0;}
#endif