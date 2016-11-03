#pragma once
template<class>struct Singleton;
class Compute
{
	friend Singleton<Compute>;
	Compute(){}
	Compute &operator =(const Compute &);
public:
	bool SubCompute(int(&tresholds)[8], int start, int stop, double *signal, double *reference);
	void Do();
	void Recalculation();
};