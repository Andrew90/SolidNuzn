#pragma once

class L502SolidGroup
{
	void *hnd;
public:
  //static const int count_inputs = 2;
  static const int buffer_length = 1024;
  int &ADC_FREQ;
  int &referenceV;
  int &dataV;
  int READ_TIMEOUT;
  /*
  int f_channels[count_inputs];
  int f_ch_modes[count_inputs];
  int f_ch_ranges[count_inputs];
  */
public:
	L502SolidGroup();
	bool Init();
	void Destroy();
	int SetupParams();
	int Start();
	int Stop();
	int Read(unsigned &startChennel, double *data, unsigned &count);
};
extern L502SolidGroup l502SolidGroup;