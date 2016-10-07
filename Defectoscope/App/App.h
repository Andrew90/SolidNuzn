#pragma once
class Long{};
class Cross{};
class Thick{};
class App
{
public:
	static const int count_zones = 300;
	static const int count_cross_sensors = 12;
	static const int count_long_sensors = 4;
	static const int zone_length = 50;
	void Init();
	void Destroy();

	static void App::MainWindowBottomLabel(int n, wchar_t *text);
};

extern App app;