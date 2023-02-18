#include "CfgDlgAdc4Global.h"
#include "../../EditValue.h"
#include <string> // to_wstring
using std::to_wstring;

LRESULT CfgDlgAdc4Global::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_PAINT:
		onPaint(wParam, lParam);
		return 0;
	case WM_COMMAND:
		onCommand(wParam, lParam);
		return 0;
	case WM_CREATE:
		onCreate(wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		return onClose(wParam, lParam);
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

BOOL CfgDlgAdc4Global::Show(int nCmdShow) {
	return ShowWindow(m_hwnd, nCmdShow) && UpdateWindow(m_hwnd);
}

void CfgDlgAdc4Global::onCreate(WPARAM wParamm, LPARAM lParam)
{
	btnApply.Create(this, 360, 30, &CfgDlgAdc4Global::cmd_btnApply, L"Apply");
	edtRangeStart.Create(this, 200, 20, &CfgDlgAdc4Global::cmd_edtRangeStart, L"", 100);
	edtRangeEnd.Create(this, 200, 50, &CfgDlgAdc4Global::cmd_edtRangeEnd, L"", 100);

	SetWindowText(edtRangeStart.hwnd, to_wstring(device->range_start).c_str());
	SetWindowText(edtRangeEnd.hwnd, to_wstring(device->range_end).c_str());
}

void CfgDlgAdc4Global::onCommand(WPARAM wParam, LPARAM lParam) {
	WID id = (WID)LOWORD(wParam);
	int evt = HIWORD(wParam);
	cmdProcess((WID)id, evt, lParam);
}

LRESULT CfgDlgAdc4Global::onClose(WPARAM wParam, LPARAM lParam) {
	DestroyWindow(m_hwnd);
	return 0;
}

void CfgDlgAdc4Global::onPaint(WPARAM wParam, LPARAM lParam) {
	static PAINTSTRUCT pc;
	HDC hdc = BeginPaint(m_hwnd, &pc);
	auto setLabel = [&hdc](int x, int y, const wchar_t text[]) {
		TextOut(hdc, x, y, text, (int)wcslen(text));
	};
	setLabel(20, edtRangeStart.y - edtRangeStart.h / 2, L"range_start (ns)");
	setLabel(20, edtRangeEnd.y - edtRangeEnd.h / 2, L"range_end (ns)");
	EndPaint(m_hwnd, &pc);
}

#include <iostream>
void CfgDlgAdc4Global::cmd_btnApply(WID id, int evt, LPARAM lParam) {
	bApply = true;
	std::cout << "range_start: " << device->range_start << "; range_end: " << device->range_end << "\n";
	DestroyWindow(m_hwnd);
	return;
}

void CfgDlgAdc4Global::cmd_edtRangeStart(WID id, int evt, LPARAM lParam) {
	if (evt == EN_KILLFOCUS) {
		EditValue ev((HWND)lParam, L"An integer input is required!");
		ev.get(device->range_start);
	}
}

void CfgDlgAdc4Global::cmd_edtRangeEnd(WID id, int evt, LPARAM lParam) {
	if (evt == EN_KILLFOCUS) {
		EditValue ev((HWND)lParam, L"An integer input is required!");
		ev.get(device->range_end);
	}
}
