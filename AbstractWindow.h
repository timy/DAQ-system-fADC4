#pragma once

#include <Windows.h>
#include <map>
#include "typedef.h"

template <typename DERIVED_TYPE>
class AbstractWindow {

public:
	AbstractWindow() {}
	virtual ~AbstractWindow() {}

	// when message received, map the id of the widget to the process function
	// defined in the derived window class
	typedef void (DERIVED_TYPE::* CmdFunc)(WID, int, LPARAM);
	inline void bind(WID id, CmdFunc func) {
		mapCmdFunc.insert(std::make_pair(id, func));
	}
	// invoke process function according to the widget's id
	inline void cmdProcess(WID id, int evt, LPARAM lParam) {
		(static_cast<DERIVED_TYPE*>(this)->*mapCmdFunc[id])(id, evt, lParam);
	}

private:
	std::map<WID, CmdFunc> mapCmdFunc;
};