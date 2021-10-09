#include <Windows.h>
#include "CfgDlgAdc4.h"
#include "DeviceAdc4Params.h"

// 获得cmd消息的控件，其id映射到对应的处理函数
void CfgDlgAdc4::cmd_ckbCardEnabled(WID id, int evt, LPARAM lParam) {
	if (evt == BN_CLICKED)
		UpdateCardEnabled();
	return;
}
void CfgDlgAdc4::cmd_cbbADCMode(WID id, int evt, LPARAM lParam) {
	if (evt == CBN_SELCHANGE) {
		LRESULT idx = SendMessage((HWND)cbbADCMode.hwnd, (UINT)CB_GETCURSEL, 0, 0);
		card->mode = ADC_mode_list[idx].mode;
		UpdateChannelEnabled();
		//SendMessage((HWND)lParam, CB_GETLBTEXT, idx, (LPARAM)ListItem);
		//SetWindowText(hSttTest, ListItem);
	}
}
void CfgDlgAdc4::cmd_btnApply(WID id, int evt, LPARAM lParam) {
	// check if parameters are properly set
	// 在使用该卡的情况下，未使用的通道不可作为主 trigger
	if (card->isCardEnabled) {
		if (card->idxMasterChannel > -1) {
			if (!bChannelEnabled[card->idxMasterChannel]) {
				MessageBox(m_hwnd,
					L"Illegal master channel: "
					"the selected channel as the master trigger is not used.",
					L"Illegal setup", MB_OK);
				return;
			}
		}
	}
	else {
		// 在该卡禁用时，其各通道均不可作为 trigger，因此重置
		// 但不必令用户手动设置，无需 return，直接 apply 即可
		if (card->idxMasterChannel > -1)
			card->idxMasterChannel = -1;
	}
	// if appropiate, apply
	bApply = true;
	DestroyWindow(m_hwnd);
	return;
}
// 子区域上的控件处理函数
void CfgDlgAdc4::cmd_rdbMasterChannel(WID id, int evt, LPARAM lParam) {
	size_t i = indexOfChannelDetailsGroup("rdbMasterChannel", id);
	card->idxMasterChannel = (int)i;
	return;
}
void CfgDlgAdc4::cmd_edtPrecursor(WID id, int evt, LPARAM lParam) {
	size_t i = indexOfChannelDetailsGroup("edtPrecursor", id);
	if (evt == EN_KILLFOCUS) {
		RetrieveIntFromEdit((HWND)lParam, card->channel[i].precursor);
	}
	return;
}
void CfgDlgAdc4::cmd_edtLength(WID id, int evt, LPARAM lParam) {
	size_t i = indexOfChannelDetailsGroup("edtLength", id);
	if (evt == EN_KILLFOCUS) {
		RetrieveIntFromEdit((HWND)lParam, card->channel[i].length);
	}
	return;
}
void CfgDlgAdc4::cmd_ckbRetrigger(WID id, int evt, LPARAM lParam) {
	size_t i = indexOfChannelDetailsGroup("ckbRetrigger", id);
	if (evt == BN_CLICKED)
		card->channel[i].isRetrigger = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
	return;
}
void CfgDlgAdc4::cmd_ckbEdgeMode(WID id, int evt, LPARAM lParam) {
	size_t i = indexOfChannelDetailsGroup("ckbEdgeMode", id);
	if (evt == BN_CLICKED)
		card->channel[i].isEdgeMode = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
	return;
}
void CfgDlgAdc4::cmd_ckbRising(WID id, int evt, LPARAM lParam) {
	size_t i = indexOfChannelDetailsGroup("ckbRising", id);
	if (evt == BN_CLICKED)
		card->channel[i].isRising = (int)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
	return;
}
void CfgDlgAdc4::cmd_edtThreshold(WID id, int evt, LPARAM lParam) {
	size_t i = indexOfChannelDetailsGroup("edtThreshold", id);
	if (evt == EN_KILLFOCUS) {
		RetrieveIntFromEdit((HWND)lParam, card->channel[i].threshold);
	}
	return;
}
void CfgDlgAdc4::cmd_edtOffset(WID id, int evt, LPARAM lParam) {
	size_t i = indexOfChannelDetailsGroup("edtOffset", id);
	if (evt == EN_KILLFOCUS) {
		RetrieveIntFromEdit((HWND)lParam, card->channel[i].offset);
	}
	return;
}

LRESULT CfgDlgAdc4::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
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

BOOL CfgDlgAdc4::Show(int nCmdShow) {
	return ShowWindow(m_hwnd, nCmdShow) && UpdateWindow(m_hwnd);
}

LRESULT CfgDlgAdc4::onClose(WPARAM wParam, LPARAM lParam) {
	//if (MessageBoxEx(m_hwnd, L"确定退出？", L"", MB_YESNO,
	//	MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)) == IDYES) {
		DestroyWindow(m_hwnd);
	//}
	return 0;
}

void CfgDlgAdc4::onPaint(WPARAM wParam, LPARAM lParam) {
	static PAINTSTRUCT pc;
	HDC hdc;
	hdc = BeginPaint(m_hwnd, &pc);

	// Card enabled
	Rectangle(hdc, 10, 10, 140, 50);
	// ADC mode
	Rectangle(hdc, 150, 10, 300, 50);
	wchar_t lbADCMode[] = L"ADC Mode";
	TextOut(hdc, 225, 22, lbADCMode, (int)wcslen(lbADCMode));
	
	int dx, dy;

	// option frame and title
	Rectangle(hdc, 10, 60, 100, 420);
	dx = 80; // 100
	wchar_t lbCard[][2] = {L"A", L"B", L"C", L"D"};
	for (int i = 0; i < 4; i++) {
		// frame
		Rectangle(hdc, 110 + i*dx, 60, 180 + i * dx, 420);
		// title
		TextOut(hdc, 120+i*dx, 65, lbCard[i], (int)wcslen(lbCard[i]));
	}

	// opiton detailed labels
	dy = 40;
	wchar_t lbOptions[][16] = { L"is_master", L"precursor", L"length", L"retrigger", L"edge mode", L"rising",
								L"threshold", L"offset" };
	for (int i = 0; i < sizeof(lbOptions)/sizeof(lbOptions[0]); i ++)
		TextOut(hdc, 20, 100 + i*dy, lbOptions[i], (int)wcslen(lbOptions[i]));	
	// 


	EndPaint(m_hwnd, &pc);
}

void CfgDlgAdc4::onCreate(WPARAM wParamm, LPARAM lParam) {

	ckbCardEnabled.Create(this, 80, 30, &CfgDlgAdc4::cmd_ckbCardEnabled, L"Card enabled", 118, 38);
	cbbADCMode.Create(this, 190, 18, &CfgDlgAdc4::cmd_cbbADCMode);
	btnApply.Create(this, 360, 30, &CfgDlgAdc4::cmd_btnApply, L"Apply");

	// 为 Combobox 添加 mode 选项
	//wchar_t modesADC[][8] = { L"ABCD", L"A", L"B", L"C", L"D", L"AB", L"CD" };
	//for (int i = 0; i < sizeof(modesADC) / sizeof(modesADC[0]); i++)
	//	SendMessage(cbbADCMode.hwnd , CB_ADDSTRING, 0, (LPARAM)modesADC[i]);
	//SendMessage(cbbADCMode.hwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0); // WPARAM for the index of the selected item

	for (int i = 0; i < sizeof(ADC_mode_list) / sizeof(ADC_mode_list[0]); i++)
		SendMessage(cbbADCMode.hwnd, CB_ADDSTRING, 0, (LPARAM)ADC_mode_list[i].label);



	// detailed options
	// - radio button: channel enabled
	for (int i = 0; i < nChannels; i++)
		chd[i].Create(this, 145 + i * 80);

	// Combobox 选项
	SendMessage(cbbADCMode.hwnd, CB_SETCURSEL, (WPARAM)AdcModeIndex(card->mode), (LPARAM)0);

	// 采集卡 使能状态 复选框
	SendMessage(ckbCardEnabled.hwnd, BM_SETCHECK, card->isCardEnabled, 0);
	UpdateCardEnabled();


	// radiobutton 主通道单选框
	if (card->idxMasterChannel > -1)
		SendMessage(chd[card->idxMasterChannel].w["rdbMasterChannel"]->hwnd, BM_SETCHECK, TRUE, 0);

	// edit 设置初始值
	for (int i = 0; i < nChannels; i++) {
		UpdateEditFromInt(chd[i].w["edtPrecursor"]->hwnd, card->channel[i].precursor);
		UpdateEditFromInt(chd[i].w["edtLength"]->hwnd, card->channel[i].length);		
		UpdateEditFromInt(chd[i].w["edtThreshold"]->hwnd, card->channel[i].threshold);
		UpdateEditFromInt(chd[i].w["edtOffset"]->hwnd, card->channel[i].offset);
	}

	// checkbox 设置初始值
	for (int i = 0; i < nChannels; i++) {
		SendMessage(chd[i].w["ckbRetrigger"]->hwnd, BM_SETCHECK, card->channel[i].isRetrigger, 0);
		SendMessage(chd[i].w["ckbEdgeMode"]->hwnd, BM_SETCHECK, card->channel[i].isEdgeMode, 0);
		SendMessage(chd[i].w["ckbRising"]->hwnd, BM_SETCHECK, card->channel[i].isRising, 0);
	}
}

void CfgDlgAdc4::onCommand(WPARAM wParam, LPARAM lParam) {

	WID id = (WID)LOWORD(wParam);
	int evt = HIWORD(wParam);
	cmdProcess((WID)id, evt, lParam);

	//switch (id) {
	//case WG_BTN_TEST: { // 点击“测试”按钮
	//	//int len = SendMessage(hEdtTest, WM_GETTEXTLENGTH, 0, 0);
	//	//wchar_t* buffer = new wchar_t[len + 1];
	//	//SendMessage(hEdtTest, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
	//	//dev->id = _wtoi(buffer);
	//	////SetWindowText(hSttTest, buffer);

	//	//delete[] buffer;
	//}
	//break;
	//}
}

void CfgDlgAdc4::UpdateCardEnabled() {
	int iChecked = (int)SendMessage(ckbCardEnabled.hwnd, BM_GETCHECK, 0, 0);
	EnableWindow(cbbADCMode.hwnd, iChecked);
	if (iChecked == false) // card disabled
		for (int i = 0; i < nChannels; i++)
			chd[i].EnableAll(iChecked);
	else                   // card enabled
		UpdateChannelEnabled();
	card->isCardEnabled = iChecked;
}

void CfgDlgAdc4::UpdateChannelEnabled() {
	// update the flags
	switch (card->mode) {
	case ADC_MODE_ABCD:
		bChannelEnabled[0] = true;
		bChannelEnabled[1] = true;
		bChannelEnabled[2] = true;
		bChannelEnabled[3] = true;
		break;
	case ADC_MODE_A:
		bChannelEnabled[0] = true;
		bChannelEnabled[1] = false;
		bChannelEnabled[2] = false;
		bChannelEnabled[3] = false;
		break;
	case ADC_MODE_B:
		bChannelEnabled[0] = false;
		bChannelEnabled[1] = true;
		bChannelEnabled[2] = false;
		bChannelEnabled[3] = false;
		break;
	case ADC_MODE_C:
		bChannelEnabled[0] = false;
		bChannelEnabled[1] = false;
		bChannelEnabled[2] = true;
		bChannelEnabled[3] = false;
		break;
	case ADC_MODE_D:
		bChannelEnabled[0] = false;
		bChannelEnabled[1] = false;
		bChannelEnabled[2] = false;
		bChannelEnabled[3] = true;
		break;
	}
	// update the UI
	for (int i = 0; i < nChannels; i++) {
		chd[i].EnableAll(bChannelEnabled[i]);
	}
}

void CfgDlgAdc4::RetrieveIntFromEdit(HWND hWnd, unsigned int& value) {
	int len = (int)SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
	if (len == 0) {
		MessageBox(NULL, L"An integer input is required!", L"Input error", MB_OK);
		SetFocus(hWnd);
		return;
	}
	wchar_t* buffer = new wchar_t[len + 1]; // +1 for terminator '\0'
	SendMessage(hWnd, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
	value = _wtoi(buffer);
	delete[] buffer;
	UpdateEditFromInt(hWnd, value);
}

void CfgDlgAdc4::UpdateEditFromInt(HWND hWnd, unsigned int value) {
	wchar_t strVal[16];
	swprintf_s(strVal, L"%d", value);
	SetWindowText(hWnd, strVal);
}


#include <iostream>
bool DisplayModalDialog(HWND hParent, DeviceAdc4Params* card) {

	EnableWindow(hParent, FALSE);

	CfgDlgAdc4 dlg(card);
	wchar_t title[32];
	wsprintf(title, L"ADC Card %d Configuration", card->id);
	if (!dlg.Create(title, WS_CAPTION | WS_VISIBLE, 0,
		CW_USEDEFAULT, CW_USEDEFAULT, 450, 470, hParent)) {
		std::cout << "[ERR] Fail to create card configuratoin panel." << std::endl;
		return false;
	}
	dlg.Show();

	// Run the message loop.
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	EnableWindow(hParent, TRUE);
	SetForegroundWindow(hParent);

	return dlg.isApply();
}
