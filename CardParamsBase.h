#pragma once

class CfgFileItems;

class CardParamsBase {
public:
	CardParamsBase() : isEnabled(true) {}
	virtual ~CardParamsBase() {}
	virtual CardParamsBase& operator = (const CardParamsBase&) { return *this; }
	virtual void bindCfgFileItem(CfgFileItems* cfi) {};
	//virtual void importFromFile(const wchar_t*) {}
	//virtual void exportToFile(const wchar_t*) {}

	unsigned int id;
	bool isEnabled;
};


