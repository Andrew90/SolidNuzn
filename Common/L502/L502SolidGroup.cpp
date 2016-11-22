#include "stdafx.h"
#include "L502SolidGroup.h"
#include "AppBase.h"
#include "Config.h"
#ifndef L502OFF
#include "e502api.h"
#include "DebugMess.h"

#include <stdio.h>

unsigned L502SolidGroup::inputBits;
L502SolidGroup::L502SolidGroup()
	: ADC_FREQ(Singleton<SolenoidParametersTable>::Instance().items.get<Frequency502>().value)
	, referenceV(Singleton<SolenoidParametersTable>::Instance().items.get<RangeReferenceSignal>().value)
	, dataV(Singleton<SolenoidParametersTable>::Instance().items.get<InputRangeSignal>().value)
	, READ_TIMEOUT(50)
{}

bool L502SolidGroup::Init()
{
	hnd = X502_Create();
	if (hnd==NULL)
	{
		dprint("\nerror module!");
	}
	else
	{
		t_x502_devrec rec;
		E502_UsbGetDevRecordsList(&rec,1, 0, NULL);
		int err =  X502_OpenByDevRecord((t_x502_hnd)hnd, &rec);
		if (err)
		{
			dprint("\nerror connection with module: %s!", X502_GetErrorString(err));
			X502_Free((t_x502_hnd)hnd);
			hnd = NULL;
		}
		X502_FreeDevRecordList(&rec, 1);
	}
	return NULL != hnd;
}

void L502SolidGroup::Destroy()
{
	 /* закрываем связь с модулем */
        X502_Close((t_x502_hnd)hnd);
        /* освобождаем описатель */
        X502_Free((t_x502_hnd)hnd);
}

namespace
{
	
}

int L502SolidGroup::SetupParams()
{
	ADCInputsParametersTable::TItems &adcParam = Singleton<ADCInputsParametersTable>::Instance().items;
	int f_channels[] = {
		adcParam.get<InputSignal>().value
		, adcParam.get<ReferenceSignal>().value
	};
	int f_ch_modes[] = {X502_LCH_MODE_COMM, X502_LCH_MODE_COMM};
	int f_ch_ranges[] = {referenceV, dataV};

	static const int count_inputs = dimention_of(f_channels);
	int err = X502_SetLChannelCount((t_x502_hnd)hnd, count_inputs);
	for (int i=0; (i < count_inputs) && !err; i++)
        err = X502_SetLChannel((t_x502_hnd)hnd, i, f_channels[i], f_ch_modes[i], f_ch_ranges[i], 0);
	/* устанавливаем частоты ввода для АЦП и цифровых входов */
	double f_adc = ADC_FREQ;
	double f_frame = (double)ADC_FREQ/count_inputs;
    if (!err)
    {
        err = X502_SetAdcFreq((t_x502_hnd)hnd, &f_adc, &f_frame);
    }

	 double f_din = 100;//ADC_FREQ;
	 if (!err)
    {
        err = X502_SetDinFreq((t_x502_hnd)hnd, &f_din);
    }

    /* записываем настройки в модуль */
    if (!err)
	{
		dprint("\nfrequency adc = %0.0f frequency chenell = %0.0f digital frequency = %0.0f", f_adc, f_frame, f_din);
        err = X502_Configure((t_x502_hnd)hnd, 0);
	}

    /* разрешаем синхронные потоки */
    if (!err)
    {
        err = X502_StreamsEnable((t_x502_hnd)hnd, X502_STREAM_ADC | X502_STREAM_DIN);
    }

    return err;
}
int L502SolidGroup::Start()
{
	int err = X502_StreamsStart((t_x502_hnd)hnd);
	if (err)
		dprint("\nerror collections date: %s!", X502_GetErrorString(err));
	return err;
}

int L502SolidGroup::Stop()
{
	/* останавливаем поток сбора данных (независимо от того, была ли ошибка) */
	int err = X502_StreamsStop((t_x502_hnd)hnd);
	if (err)
	{
		dprint("\nerror colecton date: %s", X502_GetErrorString(err));
	}
	return err;
}

int L502SolidGroup::Read(unsigned &startChennel, double *data, unsigned &count, unsigned &input)
{
	unsigned rcv_buf[buffer_length];
	int cnt = X502_Recv((t_x502_hnd)hnd, rcv_buf, buffer_length, READ_TIMEOUT);
	unsigned countDigit = 1;
	if(cnt > 0)
	{
		X502_GetNextExpectedLchNum((t_x502_hnd)hnd, &startChennel);
		int err = X502_ProcessData((t_x502_hnd)hnd, rcv_buf, cnt, X502_PROC_FLAGS_VOLT,
			data, &count, &input, &countDigit);
		inputBits = input;
		if (err)
		{
			dprint("\nerror computing date: %s", X502_GetErrorString(err));
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
	, READ_TIMEOUT(50)
{}
bool  L502SolidGroup::Init(){return 0;}
void  L502SolidGroup::Destroy(){}
int   L502SolidGroup::SetupParams(){return 0;}
int   L502SolidGroup::Start(){return 0;}
int   L502SolidGroup::Stop(){return 0;}
int L502SolidGroup::Read(unsigned &startChennel, double *data, unsigned &count){return 0;}
#endif