#pragma once

class L502SolidGroup
{
	void *hnd;
public:
  static const int buffer_length = 1024;
  int &ADC_FREQ;
  int &referenceV;
  int &dataV;
  int READ_TIMEOUT;
  static unsigned inputBits;
public:
	L502SolidGroup();
	bool Init();
	void Destroy();
	int SetupParams();
	int Start();
	int Stop();
	int Read(unsigned &startChennel, double *data, unsigned &count, unsigned &input);
};
