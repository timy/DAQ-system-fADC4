#pragma once

#include "LogLines.h"
#include "LogDisplay.h"
#include "LogStream.h"

class LogSystem {
public:
	LogSystem() : display(logLines) {
		m_pLogBuf = new std::basic_streambuf_gui<char>(display);
		m_pOldBuf = std::cout.rdbuf(m_pLogBuf);
	}
	virtual ~LogSystem() {
		std::cout.rdbuf(m_pOldBuf);
		delete m_pLogBuf;
	}
	LogDisplay* getDisplay() { return &display; }

private:
	std::basic_streambuf_gui<char>* m_pLogBuf;
	std::basic_streambuf<char>* m_pOldBuf;
	LogDisplay display;
	LogLines logLines;
};