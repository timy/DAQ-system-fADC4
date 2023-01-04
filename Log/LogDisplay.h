#pragma once

#include <Windows.h>

class LogLines;

class LogDisplay {
public:
	LogDisplay(LogLines& _logLines) : logLines(_logLines),
		yLogStart(40),
		rectLog({ 610, 30, 1100, 600 }) {
		title = new wchar_t[256];
		wcscpy_s(title, 256, L"LOG");
	}
	virtual ~LogDisplay() {
		delete[] title;
	}

	// process event messages
	void onCreate(HWND _hwnd, LONG _chH) {
		hwnd = _hwnd;
		chH = _chH;
		// 新建并初始化滚动条
		iVScrollPos = 0, iMin = 0, iMax = 0; // iMax = NUMLINES - 1;
		SetScrollRange(hwnd, SB_VERT, iMin, iMax, FALSE);
		SetScrollPos(hwnd, SB_VERT, iVScrollPos, FALSE);
	}

	void onVScroll(WPARAM wParam, LPARAM lParam) {
		switch (LOWORD(wParam)) {
		case SB_LINEUP:
			iVScrollPos--;
			break;
		case SB_LINEDOWN:
			iVScrollPos++;
			break;
		case SB_PAGEUP:
			iVScrollPos -= clientH / chH;
			break;
		case SB_PAGEDOWN:
			iVScrollPos += clientH / chH;
			break;
			/*case SB_THUMBPOSITION: // 到达位置后，释放鼠标时产生消息
				iVScrollPos = HIWORD(wParam);
				break;*/
		case SB_THUMBTRACK: // 随拖动即时产生消息
			iVScrollPos = HIWORD(wParam);
			break;
		}
		if (iVScrollPos < iMin)
			iVScrollPos = iMin;
		else if (iVScrollPos > iMax)
			iVScrollPos = iMax;

		if (iVScrollPos != GetScrollPos(hwnd, SB_VERT)) {
			SetScrollPos(hwnd, SB_VERT, iVScrollPos, TRUE);
			InvalidateRect(hwnd, &rectLog, TRUE);
		}
	}

	void onPaint(HDC hdc) {
		TextOut(hdc, rectLog.left + (rectLog.right - rectLog.left) * 5 / 10, 6, title, lstrlen(title));
		Rectangle(hdc, rectLog.left, rectLog.top, rectLog.right, rectLog.bottom);
		size_t nLines = logLines.size();
		for (int i = 0; i < nLines; i++) {
			auto y = yLogStart + 5 + (i - iVScrollPos) * chH;
			if (y > yLogStart && y < clientH - yLogStart) {
				TextOut(hdc, rectLog.left + 10, y, logLines[i], lstrlen(logLines[i]));
				SetTextAlign(hdc, TA_LEFT | TA_TOP);
			}
		}
	}

	void onSize(int _clientH, int _clientW) {
		clientH = _clientH;
	}

	// functions

	void AddItem(char* logItem) {
		// 首先将 char* 类型的 logItem 转换为 wchar_t* 类型，再调用 appendLogItem
		// number of character of 'char' type from the src
		size_t sSize = strlen(logItem);
		// invoke MultiByteToWiderChar for the first time to retrieve
		// the number of character of 'wchar_t' type for the dest
		DWORD dSize = MultiByteToWideChar(CP_ACP, 0, logItem, (int)sSize, NULL, 0);
		
		wchar_t* dBuf = new wchar_t[dSize+1]; // dSize+1, with the extra wchar_t for '\0'
		wmemset(dBuf, 0, dSize+1);

		// invoke MultByteToWiderChar once more to start the actual conversion
		int nRet = MultiByteToWideChar(CP_ACP, 0, logItem, (int)sSize, dBuf, dSize);
		if (nRet > 0)
			appendLogItem(dBuf);
		
		delete[] dBuf;
	}

	void AddItem(wchar_t* logItem) {
		appendLogItem(logItem);
	}

	void ClearLog() {
		logLines.clear();
		iMax = 0; iVScrollPos = 0;
		updateRectLog();
	}

private:

	void appendLogItem(wchar_t* logItem) {

		logLines.append(logItem);
		iMax++;

		// 日志区矩形可容纳的日志条目数
		int nLines = (rectLog.bottom - rectLog.top - 5 - 5) / chH;
		// 当日志条目数 - 显示区首行位置 > 矩形可容纳行数，自动滚动
		if (iMax - iVScrollPos > nLines)
			iVScrollPos = iMax - nLines;
		updateRectLog();
	}

	void updateRectLog() {
		SetScrollRange(hwnd, SB_VERT, iMin, iMax, FALSE);
		SetScrollPos(hwnd, SB_VERT, iVScrollPos, FALSE);
		InvalidateRect(hwnd, &rectLog, TRUE);
	}

	int iVScrollPos, iMin, iMax;
	int yLogStart;
	WORD clientH;
	LONG chH;
	wchar_t* title;
	RECT rectLog;
	LogLines& logLines;
	HWND hwnd;
};