#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <cmath>

#include "DataVisualizerWindow.h"
#include "DataVisualizerData.h"
#include "Timer.h"

#include "Device/ADC4/DaqStatusAdc4Type.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")

namespace {
	enum WIDGET_ID {
		WG_EDT_TEST,
		WG_BTN_TEST,
		WG_STT_TEST,
	};
}

template <class T> void SafeRelease(T** ppT) {
	if (*ppT != NULL) {
		(*ppT)->Release();
		*ppT = NULL;
	}
}

DataVisualizerWindow::DataVisualizerWindow(DaqStatusBaseType* src_) : src(src_),
m_pD2dFactory(NULL), m_pRenderTarget(NULL),
m_pLightSlateGrayBrush(NULL), m_pCornflowerBlueBrush(NULL) {
}

DataVisualizerWindow::~DataVisualizerWindow() {

	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pD2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
	SafeRelease(&m_pBlackBrush);
}

BOOL DataVisualizerWindow::Create(
	PCWSTR lpWindowName,
	DWORD dwStyle,
	DWORD dwExStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu) {

	HRESULT hr = CreateDeviceIndependentResources();
	if (!SUCCEEDED(hr)) {
		return FALSE;
	}
	return BaseWindow<DataVisualizerWindow>::Create(
		lpWindowName,
		dwStyle,
		dwExStyle,
		x,
		y,
		nWidth,
		nHeight,
		hWndParent,
		hMenu);
}

LRESULT DataVisualizerWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_PAINT:
		onPaint(wParam, lParam);
		return 0;
	case WM_COMMAND:
		onCommand(wParam, lParam);
		return 0;
	case WM_SIZE:
		onSize(wParam, lParam);
		return 0;
	case WM_CREATE:
		onCreate(wParam, lParam);
		return 0;
	case WM_DESTROY:
		onDestroy(wParam, lParam);
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		return onClose(wParam, lParam);
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

BOOL DataVisualizerWindow::Show(int nCmdShow) {
	return ShowWindow(m_hwnd, nCmdShow) && UpdateWindow(m_hwnd);
}

LRESULT DataVisualizerWindow::onClose(WPARAM wParam, LPARAM lParam) {
	DestroyWindow(m_hwnd);
	return 0;
}

void DataVisualizerWindow::onPaint(WPARAM wParam, LPARAM lParam) {
	//static PAINTSTRUCT pc;
	//HDC hdc;
	//hdc = BeginPaint(m_hwnd, &pc);
	//EndPaint(m_hwnd, &pc);

	float lineWidth;
	ID2D1SolidColorBrush* brush;

	HRESULT hr = S_OK;
	hr = CreateDeviceResources();

	if (SUCCEEDED(hr)) {

		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		X0 = 0 + 40;
		Y0 = 0 + 10;
		X1 = rtSize.width - 10;
		Y1 = rtSize.height - 80;

		// canvas
		lineWidth = 0.5f;
		brush = m_pLightSlateGrayBrush;

		// left frame
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(0.f, 0.f),
			D2D1::Point2F(0.f, rtSize.height),
			brush,
			lineWidth);
		// right frame
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(rtSize.width, 0.f),
			D2D1::Point2F(rtSize.width, rtSize.height),
			brush,
			lineWidth);
		// top frame
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(0.f, 0.f),
			D2D1::Point2F(rtSize.width, 0.f),
			brush,
			lineWidth);
		// bottom frame
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(0.f, rtSize.height),
			D2D1::Point2F(rtSize.width, rtSize.height),
			brush,
			lineWidth);

		// data display
		lineWidth = 0.5f;
		brush = m_pBlackBrush;

		// left frame
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(X0, Y0),
			D2D1::Point2F(X0, Y1),
			brush,
			lineWidth);
		// right frame
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(X1, Y0),
			D2D1::Point2F(X1, Y1),
			brush,
			lineWidth);
		// top frame
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(X0, Y0),
			D2D1::Point2F(X1, Y0),
			brush,
			lineWidth);
		// bottom frame
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(X0, Y1),
			D2D1::Point2F(X1, Y1),
			brush,
			lineWidth);

		// x-axis
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(X0, 0.5f * (Y0 + Y1)),
			D2D1::Point2F(X1, 0.5f * (Y0 + Y1)),
			m_pLightSlateGrayBrush,
			2.f);

		float X, Y, X_old, Y_old;
		float x_min = data->xMin;
		float x_max = data->xMax;
		float y_min = data->yMin; // -1.f;
		float y_max = data->yMax; // +1.f;
		X_old = xTransform(data->x[0], x_min, x_max);
		Y_old = yTransform(data->y[0], y_min, y_max); // min(data_y), max(data_y)
		for (int i = 1; i < data->nData; i++) {
			X = xTransform(data->x[i], x_min, x_max);
			Y = yTransform(data->y[i], y_min, y_max);
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(X_old, Y_old),
				D2D1::Point2F(X, Y),
				m_pCornflowerBlueBrush,
				1.f);
			X_old = X;
			Y_old = Y;
		}

		// ticks of x-axis
		for (int i = 0; i < data->nTicksX; i++) {
			float xt = xTransform((float)_wtof(data->ticksX[i]), x_min, x_max);
			// float yt = yTransform(data->yMin, y_min, y_max);
			m_pRenderTarget->DrawText(
				data->ticksX[i],
				(unsigned int)wcslen(data->ticksX[i]),
				m_pTextFormat,
				D2D1::RectF(xt - 20, Y1, xt + 20, Y1 + 40),
				m_pBlackBrush
			);
			// grid (vertical)
			lineWidth = 0.5f;
			brush = m_pBlackBrush;
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(xt, Y0),
				D2D1::Point2F(xt, Y1),
				brush,
				lineWidth
			);

			//// frames of axis ticks
			//lineWidth = 0.5f;
			//brush = m_pBlackBrush;
			//// left
			//m_pRenderTarget->DrawLine(
			//	D2D1::Point2F(xt-20, Y1),
			//	D2D1::Point2F(xt-20, Y1+40),
			//	brush,
			//	lineWidth);
			//// right
			//m_pRenderTarget->DrawLine(
			//	D2D1::Point2F(xt+20, Y1),
			//	D2D1::Point2F(xt+20, Y1+40),
			//	brush,
			//	lineWidth);
			//// top
			//m_pRenderTarget->DrawLine(
			//	D2D1::Point2F(xt-20, Y1),
			//	D2D1::Point2F(xt+20, Y1),
			//	brush,
			//	lineWidth);
			//// bottom
			//m_pRenderTarget->DrawLine(
			//	D2D1::Point2F(xt-20, Y1+40),
			//	D2D1::Point2F(xt+20, Y1+40),
			//	brush,
			//	lineWidth);
		}

		// ticks of y-axis
		for (int i = 0; i < data->nTicksY; i++) {
			float yt = yTransform((float)_wtof(data->ticksY[i]), y_min, y_max);
			m_pRenderTarget->DrawText(
				data->ticksY[i],
				(unsigned int)wcslen(data->ticksY[i]),
				m_pTextFormat,
				D2D1::RectF(X0 - 40, yt - 20, X0, yt + 20),
				m_pBlackBrush
			);
			// grid (horizontal)
			lineWidth = 0.5f;
			brush = m_pBlackBrush;
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(X0, yt),
				D2D1::Point2F(X1, yt),
				brush,
				lineWidth
			);


			//// frames of axis ticks
			//lineWidth = 0.5f;
			//brush = m_pBlackBrush;
			//// left
			//m_pRenderTarget->DrawLine(
			//	D2D1::Point2F(X0 - 20, yt),
			//	D2D1::Point2F(X0 - 20, yt + 40),
			//	brush,
			//	lineWidth);
			//// right
			//m_pRenderTarget->DrawLine(
			//	D2D1::Point2F(X0 + 20, yt),
			//	D2D1::Point2F(X0 + 20, yt + 40),
			//	brush,
			//	lineWidth);
			//// top
			//m_pRenderTarget->DrawLine(
			//	D2D1::Point2F(X0 - 20, yt),
			//	D2D1::Point2F(X0 + 20, yt),
			//	brush,
			//	lineWidth);
			//// bottom
			//m_pRenderTarget->DrawLine(
			//	D2D1::Point2F(X0 - 20, yt + 40),
			//	D2D1::Point2F(X0 + 20, yt + 40),
			//	brush,
			//	lineWidth);
		}


		static const wchar_t xLabel[] = L"Time (ns)";
		static const wchar_t yLabel[] = L"Signal";
		m_pRenderTarget->DrawText(
			xLabel,
			(unsigned int)wcslen(xLabel),
			m_pTextFormat,
			D2D1::RectF((X0 + X1) / 2 - 50, Y1 + 40, (X0 + X1) / 2 + 50, Y1 + 70),
			m_pBlackBrush
		);

		//// Draw two rectangles.
		//D2D1_RECT_F rectangle1 = D2D1::RectF(
		//	rtSize.width / 2 - 50.0f,
		//	rtSize.height / 2 - 50.0f,
		//	rtSize.width / 2 + 50.0f,
		//	rtSize.height / 2 + 50.0f
		//);

		//D2D1_RECT_F rectangle2 = D2D1::RectF(
		//	rtSize.width / 2 - 100.0f,
		//	rtSize.height / 2 - 100.0f,
		//	rtSize.width / 2 + 100.0f,
		//	rtSize.height / 2 + 100.0f
		//);

		//// Draw a filled rectangle.
		//m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

		//// Draw the outline of a rectangle.
		//m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);

		//m_pRenderTarget->DrawRectangle(
		//	D2D1::RectF(
		//		rc.left + 100.0f,
		//		rc.top + 100.0f,
		//		rc.right - 100.0f,
		//		rc.bottom - 100.0f),
		//	m_pCornflowerBlueBrush);

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET) {
		hr = S_OK;
		DiscardDeviceResources();
	}
}

void DataVisualizerWindow::onCreate(WPARAM wParamm, LPARAM lParam) {

	initialize();

	//hEdtTest = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
	//	30, 30, 120, 40, m_hwnd, (HMENU)WG_EDT_TEST, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
	//hBtnTest = CreateWindow(L"button", L"Get value", WS_CHILD | WS_VISIBLE,
	//	160, 30, 120, 40, m_hwnd, (HMENU)WG_BTN_TEST, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
	//hCanvas = CreateWindow(L"static", L"", WS_CHILD | WS_VISIBLE,
	//	30, 80, 120, 40, m_hwnd, (HMENU)WG_STT_TEST, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	// ckbLocal.Create(this, 360, 280, &MainWindow::cmd_ckbLocal, L"Save", 60, 30);

	// btnTest.Create(this, canvasX0 + 50, 40, &DataVisualizerWindow::cmd_ckbDisplayChannels, L"TEST"); // NOTE!!!! only for test

	// NOTE!!!!! TODO: we should only repaint the canvas part, otherwise other widgets just disappear
	// so next we should define some rect to let the update part know exactly where the canvas should be repainted.


	sttCanvas.Create(this, (canvasX0 + canvasX1) / 2, (canvasY0 + canvasY1) / 2,
		&DataVisualizerWindow::cmd_sttCanvas, L"", 744, 480);

	for (int i = 0; i < countChannels; i++) {
		int x = canvasX0 + 120 + 90 * (i - 1);
		int y = 40;
		ckbDisplayChannels.push_back(new Widget<DataVisualizerWindow, ckb_t>);
		ckbDisplayChannels.back()->Create(this, x, y, &DataVisualizerWindow::cmd_ckbDisplayChannels,
			labelChannels[i], 60, 30);
		SendMessage(ckbDisplayChannels.back()->hwnd, BM_SETCHECK, isChannelSelected[i], 0);
	}

	//hCanvas = CreateWindow(L"static", L"", WS_CHILD | WS_VISIBLE,
	//	20, 80, 744, 480, m_hwnd, (HMENU)WG_STT_TEST, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	timer = new Timer;
	timer->start(1000, std::bind(&DataVisualizerWindow::updatePlotByTimer, this));
}

void DataVisualizerWindow::onDestroy(WPARAM wParam, LPARAM lParam) {
	
	timer->stop();
	delete timer;

	while (!ckbDisplayChannels.empty()) {
		delete ckbDisplayChannels.back();
		ckbDisplayChannels.pop_back();
	}
	finalize();
}

void DataVisualizerWindow::onCommand(WPARAM wParam, LPARAM lParam) {

	//int id = LOWORD(wParam);
	//int evt = HIWORD(wParam);

	//switch (id) {
	//case WG_BTN_TEST: // 点击“测试”按钮
	//	//int len = SendMessage(hEdtTest, WM_GETTEXTLENGTH, 0, 0);
	//	//wchar_t* buffer = new wchar_t[len + 1];
	//	//SendMessage(hEdtTest, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
	//	//dev->id = _wtoi(buffer);
	//	//SetWindowText(hCanvas, buffer);
	//	//
	//	//delete[] buffer;
	//	break;
	//}

	int id = LOWORD(wParam), evt = HIWORD(wParam);
	cmdProcess((WID)id, evt, lParam); // NOTE!!! require check!
}

void DataVisualizerWindow::onSize(WPARAM wParam, LPARAM lParam) {
	if (m_pRenderTarget) {
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		// m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

void DataVisualizerWindow::cmd_sttCanvas(WID id, int evt, LPARAM lParam) {}


void DataVisualizerWindow::cmd_ckbDisplayChannels(WID id, int evt, LPARAM lParam) {
	int i = (int)(id - ckbDisplayChannels[0]->id); // card id
	if (evt == BN_CLICKED)
		isChannelSelected[i] = (bool)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
}

HRESULT DataVisualizerWindow::CreateDeviceIndependentResources() {

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2dFactory);
	if (SUCCEEDED(hr)) {
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}
	if (SUCCEEDED(hr)) {
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Arial",	// font name
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.f,		// font size
			L"",
			&m_pTextFormat
		);
	}
	if (SUCCEEDED(hr)) {
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	return hr;
}

HRESULT DataVisualizerWindow::CreateDeviceResources() {
	HRESULT hr = S_OK;

	if (!m_pRenderTarget) {
		// GetClientRect(m_hwnd, &rc);
		// GetClientRect(hCanvas, &rc);
		GetClientRect(sttCanvas.hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// D2D1_SIZE_U size = D2D1::SizeU(300, 200);

		// Create a Direct2D render target.
		hr = m_pD2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			// D2D1::HwndRenderTargetProperties(m_hwnd, size), // NOTE!!! set the main window as canvas
			D2D1::HwndRenderTargetProperties(sttCanvas.hwnd, size), // NOTE!!!! set a static control as canvas
			&m_pRenderTarget
		);

		if (SUCCEEDED(hr)) {
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&m_pBlackBrush
			);
		}
		if (SUCCEEDED(hr)) {
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush
			);
		}
		if (SUCCEEDED(hr)) {
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
			);
		}
	}
	return hr;
}

void DataVisualizerWindow::DiscardDeviceResources() {
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}


void DataVisualizerWindow::initialize() {
	DaqStatusAdc4Type* st = static_cast<DaqStatusAdc4Type*>(src);
	// alloc for internal data of visualization
	data = new DataVisualizerData(st->nDataMax);

	// total number of channels in use
	countChannels = 0;
	for (int i = 0; i < st->nDevices; i++)
		countChannels += st->idxChannels[i].size();
	mapIdxChannel = new MapIndexChannel[countChannels];
	idxChannelSelected.reserve(countChannels);

	// alloc text buffers for labels of checkbox
	const size_t nChars = 64;
	labelChannels = new wchar_t* [countChannels];
	for (int i = 0; i < countChannels; i++)
		labelChannels[i] = new wchar_t[nChars];

	// set the label for each channel
	int count = 0;
	for (int i = 0; i < st->nDevices; i++) {
		for (int j : st->idxChannels[i]) {
			swprintf_s(labelChannels[count], nChars, L"%d:%d", i, j);
			mapIdxChannel[count].card = i;
			mapIdxChannel[count].channel = j;
			count++;
		}
	}

	// alloc and initialize for flags deciding the channels are selected
	isChannelSelected = new bool[countChannels];
	isChannelSelected[0] = true;
	for (int i = 1; i < countChannels; i++)
		isChannelSelected[i] = false;
}

void DataVisualizerWindow::finalize() {
	DaqStatusAdc4Type* st = static_cast<DaqStatusAdc4Type*>(src);

	delete[] isChannelSelected;

	// free text buffers for labels of checkbox
	for (int i = 0; i < countChannels; i++)
		delete[] labelChannels[i];
	delete[] labelChannels;

	delete[] mapIdxChannel;
	delete data;
}

#include <iostream> // only for debug

void DataVisualizerWindow::updatePlotByTimer() {
	DaqStatusAdc4Type* st = static_cast<DaqStatusAdc4Type*>(src);

	// NOTE!!!!: 原则上我们可以根据 isChannelSelected 中，被选中元素的下标，
	// 在给定 st->idxChannels 情况下，反推出 iCard 与 iChannel。

	int iCard = 0;
	int iChannel = 0;

	// fetch indexes of selected channels into idxChannelSelected
	idxChannelSelected.clear();
	for (int i = 0; i < countChannels; i++)
		if (isChannelSelected[i])
			idxChannelSelected.push_back(mapIdxChannel[i]);
	for (auto i : idxChannelSelected)
		std::cout << i.card << ":" << i.channel << std::endl; // NOTE!!! only for debug



	{
		std::unique_lock<std::mutex> lk(st->lock);

		st->bRetrieveWaveform = true; // inquire new retrieval
		st->isWaveformRetrieved = false; // reset flag

		// Wait until return true when wnd->data has been properly filled, or return false when timeout.
		// We set timeout to avoid the awkward situation that the visualization window does not respond as 
		// the thread keeps waiting forever when no signal feeds in.
		// 设置 timeout 是为了避免在无物理信号时，该线程永远处于等待状态，导致显示窗口对用户操作无响应
		// timeout 时间至少应大于 DaqStatusAdc4Type::retrieveWaveform 执行所需时间，否则无法取回数据；
		// 较为安全的设置是将 timeout 设为与物理信号到达周期一致
		data->isReady = st->cv.wait_for(lk,
			std::chrono::milliseconds(1000),
			[&st] { return st->isWaveformRetrieved; });

		//int iCh = 0;
		//while (iCh < wnd->countChannels) {
		//	for (int iChannel = 0; iChannel < st->idxChannels[iCard].size(); iChannel++) {
		//		if (wnd->isChannelSelected[iCh++]) {
		//			// here, we get indexes (iCard, iChannel), and do the work...
		//			std::cout << "iCard: " << iCard << "iChannel: " << st->idxChannels[iCard][iChannel] << std::endl;
		//		}
		//	}
		//	iCard++;
		//}

		if (data->isReady) {
			st->bRetrieveWaveform = false; // we have retrieved this batch, so stop further retrieval
			short* dat = (short*)st->waveforms[iCard][iChannel];
			data->nData = 4 * st->lengths[iCard][iChannel];
			float t0 = st->timestamp[iCard][iChannel] - 800.f * (st->lengths[iCard][iChannel] - 1) * st->nChannels;
			float dt = 200.f * st->nChannels; // 200 ps for single-channel mode
			for (int i = 0; i < data->nData; i++) {
				data->x[i] = (t0 + dt * i) / 1000.f; // ps to ns
				data->y[i] = *(dat++);
			}
		}

		lk.unlock();
		st->cv.notify_one();
	}

	if (data->isReady) {
		data->setupAxes();
		//RECT rect;
		//GetClientRect(wnd->m_hwnd, &rect);
		RECT rect = { canvasX0, canvasY0, canvasX1, canvasY1 };
		InvalidateRect(m_hwnd, &rect, TRUE);
	}
}