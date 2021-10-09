#pragma once

class CfgFileItems;

class DeviceItemParams {
public:
	DeviceItemParams() : isCardEnabled(true) {}
	virtual ~DeviceItemParams() {}
	virtual DeviceItemParams& operator = (const DeviceItemParams&) { return *this; }
	virtual void attachCfgFileItem(CfgFileItems* cfi) {};
	//virtual void importFromFile(const wchar_t*) {}
	//virtual void exportToFile(const wchar_t*) {}

	int id;
	bool isCardEnabled;
};


