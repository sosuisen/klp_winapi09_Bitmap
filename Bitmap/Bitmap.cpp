#include <windows.h>
#include <string>

static const int MAX_WIDTH = 1000;
static const int MAX_HEIGHT = 500;

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    TCHAR szAppName[] = L"BitmapApp";
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    // ウィンドウクラスの属性を設定
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;

    // ウィンドウクラスを登録
    if (!RegisterClass(&wc)) return 0;

    // ウィンドウを作成
    hwnd = CreateWindow(
        szAppName, szAppName,
        WS_OVERLAPPEDWINDOW,
        50, 50,
        MAX_WIDTH, MAX_HEIGHT,
        NULL, NULL,
        hInstance, NULL);

    if (!hwnd) return 0;

    // ウィンドウを表示
    ShowWindow(hwnd, nCmdShow);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

COLORREF avr3x3Filter(HDC hMemDC, int x, int y) {
    COLORREF c00 = GetPixel(hMemDC, x - 1, y - 1);
    COLORREF c01 = GetPixel(hMemDC, x - 1, y);
    COLORREF c02 = GetPixel(hMemDC, x - 1, y + 1);
    COLORREF c10 = GetPixel(hMemDC, x, y - 1);
    COLORREF c11 = GetPixel(hMemDC, x, y);
    COLORREF c12 = GetPixel(hMemDC, x, y + 1);
    COLORREF c20 = GetPixel(hMemDC, x + 1, y - 1);
    COLORREF c21 = GetPixel(hMemDC, x + 1, y);
    COLORREF c22 = GetPixel(hMemDC, x + 1, y + 1);

    byte r = (GetRValue(c00) + GetRValue(c01) + GetRValue(c02)
        + GetRValue(c10) + GetRValue(c11) + GetRValue(c12)
        + GetRValue(c20) + GetRValue(c21) + GetRValue(c22)) / 9;
    byte g = (GetGValue(c00) + GetGValue(c01) + GetGValue(c02)
        + GetGValue(c10) + GetGValue(c11) + GetGValue(c12)
        + GetGValue(c20) + GetGValue(c21) + GetGValue(c22)) / 9;
    byte b = (GetBValue(c00) + GetBValue(c01) + GetBValue(c02)
        + GetBValue(c10) + GetBValue(c11) + GetBValue(c12)
        + GetBValue(c20) + GetBValue(c21) + GetBValue(c22)) / 9;
    return RGB(r, g, b);
}


COLORREF avr5x5Filter(HDC hMemDC, int x, int y) {
    COLORREF c00 = GetPixel(hMemDC, x - 2, y - 2);
    COLORREF c01 = GetPixel(hMemDC, x - 2, y - 1);
    COLORREF c02 = GetPixel(hMemDC, x - 2, y);
    COLORREF c03 = GetPixel(hMemDC, x - 2, y + 1);
    COLORREF c04 = GetPixel(hMemDC, x - 2, y + 2);
    COLORREF c10 = GetPixel(hMemDC, x - 1, y - 2);
    COLORREF c11 = GetPixel(hMemDC, x - 1, y - 1);
    COLORREF c12 = GetPixel(hMemDC, x - 1, y);
    COLORREF c13 = GetPixel(hMemDC, x - 1, y + 1);
    COLORREF c14 = GetPixel(hMemDC, x - 1, y + 2);
    COLORREF c20 = GetPixel(hMemDC, x, y - 2);
    COLORREF c21 = GetPixel(hMemDC, x, y - 1);
    COLORREF c22 = GetPixel(hMemDC, x, y);
    COLORREF c23 = GetPixel(hMemDC, x, y + 1);
    COLORREF c24 = GetPixel(hMemDC, x, y + 2);
    COLORREF c30 = GetPixel(hMemDC, x + 1, y - 2);
    COLORREF c31 = GetPixel(hMemDC, x + 1, y - 1);
    COLORREF c32 = GetPixel(hMemDC, x + 1, y);
    COLORREF c33 = GetPixel(hMemDC, x + 1, y + 1);
    COLORREF c34 = GetPixel(hMemDC, x + 1, y + 2);
    COLORREF c40 = GetPixel(hMemDC, x + 2, y - 2);
    COLORREF c41 = GetPixel(hMemDC, x + 2, y - 1);
    COLORREF c42 = GetPixel(hMemDC, x + 2, y);
    COLORREF c43 = GetPixel(hMemDC, x + 2, y + 1);
    COLORREF c44 = GetPixel(hMemDC, x + 2, y + 2);

    byte r = (GetRValue(c00) + GetRValue(c01) + GetRValue(c02) + GetRValue(c03) + GetRValue(c04)
        + GetRValue(c10) + GetRValue(c11) + GetRValue(c12) + GetRValue(c13) + GetRValue(c14)
        + GetRValue(c20) + GetRValue(c21) + GetRValue(c22) + GetRValue(c23) + GetRValue(c24)
        + GetRValue(c30) + GetRValue(c31) + GetRValue(c32) + GetRValue(c33) + GetRValue(c34)
        + GetRValue(c40) + GetRValue(c41) + GetRValue(c42) + GetRValue(c43) + GetRValue(c44)
        ) / 25;
    byte g = (GetGValue(c00) + GetGValue(c01) + GetGValue(c02) + GetGValue(c03) + GetGValue(c04)
        + GetGValue(c10) + GetGValue(c11) + GetGValue(c12) + GetGValue(c13) + GetGValue(c14)
        + GetGValue(c20) + GetGValue(c21) + GetGValue(c22) + GetGValue(c23) + GetGValue(c24)
        + GetGValue(c30) + GetGValue(c31) + GetGValue(c32) + GetGValue(c33) + GetGValue(c34)
        + GetGValue(c40) + GetGValue(c41) + GetGValue(c42) + GetGValue(c43) + GetGValue(c44)
        ) / 25;
    byte b = (GetBValue(c00) + GetBValue(c01) + GetBValue(c02) + GetBValue(c03) + GetBValue(c04)
        + GetBValue(c10) + GetBValue(c11) + GetBValue(c12) + GetBValue(c13) + GetBValue(c14)
        + GetBValue(c20) + GetBValue(c21) + GetBValue(c22) + GetBValue(c23) + GetBValue(c24)
        + GetBValue(c30) + GetBValue(c31) + GetBValue(c32) + GetBValue(c33) + GetBValue(c34)
        + GetBValue(c40) + GetBValue(c41) + GetBValue(c42) + GetBValue(c43) + GetBValue(c44)
        ) / 25;

    return RGB(r, g, b);
}

COLORREF mosaic5x5Filter(HDC hMemDC, int orgX, int orgY) {
    int x = orgX - orgX % 5;
    int y = orgY - orgY % 5;
    COLORREF c00 = GetPixel(hMemDC, x, y);
    COLORREF c01 = GetPixel(hMemDC, x, y + 1);
    COLORREF c02 = GetPixel(hMemDC, x, y + 2);
    COLORREF c03 = GetPixel(hMemDC, x, y + 3);
    COLORREF c04 = GetPixel(hMemDC, x, y + 4);
    COLORREF c10 = GetPixel(hMemDC, x + 1, y);
    COLORREF c11 = GetPixel(hMemDC, x + 1, y + 1);
    COLORREF c12 = GetPixel(hMemDC, x + 1, y + 2);
    COLORREF c13 = GetPixel(hMemDC, x + 1, y + 3);
    COLORREF c14 = GetPixel(hMemDC, x + 1, y + 4);
    COLORREF c20 = GetPixel(hMemDC, x + 2, y);
    COLORREF c21 = GetPixel(hMemDC, x + 2, y + 1);
    COLORREF c22 = GetPixel(hMemDC, x + 2, y + 2);
    COLORREF c23 = GetPixel(hMemDC, x + 2, y + 3);
    COLORREF c24 = GetPixel(hMemDC, x + 2, y + 4);
    COLORREF c30 = GetPixel(hMemDC, x + 3, y);
    COLORREF c31 = GetPixel(hMemDC, x + 3, y + 1);
    COLORREF c32 = GetPixel(hMemDC, x + 3, y + 2);
    COLORREF c33 = GetPixel(hMemDC, x + 3, y + 3);
    COLORREF c34 = GetPixel(hMemDC, x + 3, y + 4);
    COLORREF c40 = GetPixel(hMemDC, x + 4, y);
    COLORREF c41 = GetPixel(hMemDC, x + 4, y + 1);
    COLORREF c42 = GetPixel(hMemDC, x + 4, y + 2);
    COLORREF c43 = GetPixel(hMemDC, x + 4, y + 3);
    COLORREF c44 = GetPixel(hMemDC, x + 4, y + 4);

    byte r = (GetRValue(c00) + GetRValue(c01) + GetRValue(c02) + GetRValue(c03) + GetRValue(c04)
        + GetRValue(c10) + GetRValue(c11) + GetRValue(c12) + GetRValue(c13) + GetRValue(c14)
        + GetRValue(c20) + GetRValue(c21) + GetRValue(c22) + GetRValue(c23) + GetRValue(c24)
        + GetRValue(c30) + GetRValue(c31) + GetRValue(c32) + GetRValue(c33) + GetRValue(c34)
        + GetRValue(c40) + GetRValue(c41) + GetRValue(c42) + GetRValue(c43) + GetRValue(c44)
        ) / 25;
    byte g = (GetGValue(c00) + GetGValue(c01) + GetGValue(c02) + GetGValue(c03) + GetGValue(c04)
        + GetGValue(c10) + GetGValue(c11) + GetGValue(c12) + GetGValue(c13) + GetGValue(c14)
        + GetGValue(c20) + GetGValue(c21) + GetGValue(c22) + GetGValue(c23) + GetGValue(c24)
        + GetGValue(c30) + GetGValue(c31) + GetGValue(c32) + GetGValue(c33) + GetGValue(c34)
        + GetGValue(c40) + GetGValue(c41) + GetGValue(c42) + GetGValue(c43) + GetGValue(c44)
        ) / 25;
    byte b = (GetBValue(c00) + GetBValue(c01) + GetBValue(c02) + GetBValue(c03) + GetBValue(c04)
        + GetBValue(c10) + GetBValue(c11) + GetBValue(c12) + GetBValue(c13) + GetBValue(c14)
        + GetBValue(c20) + GetBValue(c21) + GetBValue(c22) + GetBValue(c23) + GetBValue(c24)
        + GetBValue(c30) + GetBValue(c31) + GetBValue(c32) + GetBValue(c33) + GetBValue(c34)
        + GetBValue(c40) + GetBValue(c41) + GetBValue(c42) + GetBValue(c43) + GetBValue(c44)
        ) / 25;

    return RGB(r, g, b);
}

COLORREF grayFilter(HDC hMemDC, int x, int y) {
    COLORREF col = GetPixel(hMemDC, x, y);
    //byte gray = (GetRValue(col) + GetGValue(col) + GetBValue(col)) / 3;

    // CIE XYZ 規格
    byte gray = 0.2126 * GetRValue(col) + 0.7152 * GetGValue(col) + 0.0722 * GetBValue(col);

    return RGB(gray, gray, gray);
}

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP  hBitmap;    // ビットマップ
    static HDC      hMemDC;     // オフスクリーン
    static BITMAP bmp_info = { 0 };

    static HDC hMemFilterDC;
    static HBITMAP hBitmapFilter;

    int FILTER_X = 600;
    int FILTER_Y = 100;
    int FILTER_WIDTH = 240;
    int FILTER_HEIGHT = 240;

    HDC hdc;

    switch (uMsg) {
    case WM_CREATE:
        // オフスクリーンをメモリデバイスコンテキストを用いて作成
        hdc = GetDC(hwnd);
        hMemDC = CreateCompatibleDC(hdc);
        // 画像をファイルから読み込む

        hBitmap = (HBITMAP)LoadImage(NULL, L"kyocotan_tate.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        // ビットマップの情報を取得し、bmp_infoに保管
        GetObject(hBitmap, (int)sizeof(BITMAP), &bmp_info);
        SelectObject(hMemDC, hBitmap);

        // フィルタ処理用のオフスクリーン
        hMemFilterDC = CreateCompatibleDC(hdc);
        hBitmapFilter = CreateCompatibleBitmap(hdc, FILTER_WIDTH, FILTER_HEIGHT);
        SelectObject(hMemFilterDC, hBitmapFilter);

        // いろいろなフィルタ処理
        for (int x = FILTER_X; x < FILTER_X + FILTER_WIDTH; x++) {
            for (int y = FILTER_Y; y < FILTER_Y + FILTER_HEIGHT; y++) {
                // 3x3平均
                //SetPixelV(hMemFilterDC, x - FILTER_X, y - FILTER_Y, avr3x3Filter(hMemDC, x, y));

                // 5x5平均
                // SetPixelV(hMemFilterDC, x - FILTER_X, y - FILTER_Y, avr5x5Filter(hMemDC, x, y));

                // グレースケール化
                // SetPixelV(hMemFilterDC, x - FILTER_X, y - FILTER_Y, grayFilter(hMemDC, x, y));

                // 基本課題：5x5のモザイク
                SetPixelV(hMemFilterDC, x - FILTER_X, y - FILTER_Y, mosaic5x5Filter(hMemDC, x, y));
            }
        }
        BitBlt(hMemDC, FILTER_X, FILTER_Y, FILTER_WIDTH, FILTER_HEIGHT,
            hMemFilterDC, 0, 0, SRCCOPY);

        ReleaseDC(hwnd, hdc);
        return 0;
    case WM_CLOSE:
        // オフスクリーンの破棄
        DeleteDC(hMemDC);
        DeleteObject(hBitmap);
        DeleteDC(hMemFilterDC);
        DeleteObject(hBitmapFilter);
        DestroyWindow(hwnd);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT paint;
        hdc = BeginPaint(hwnd, &paint);

        // ビットブロック転送
        RECT rc;
        GetClientRect(hwnd, &rc); // クライアント領域のサイズ
        
        /*
        BitBlt(
            hdc,
            0,
            0,
            rc.right - rc.left,
            rc.bottom - rc.top,
            hMemDC,
            0,
            0,
            SRCCOPY);
        */
        
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        int imgW = bmp_info.bmWidth;
        int imgH = bmp_info.bmHeight;

        int newImgW = 0;
        int newImgH = 0;
        int xOffset = 0;
        int yOffset = 0;
        // 画像の幅をウィンドウの幅に合わせた場合
        if ((double)width / (double)imgW * imgH > height) {
            // 画像の高さに合わせる必要あり
            newImgH = height;
            newImgW = (double)height / (double)imgH * imgW;
            xOffset = (width - newImgW) / 2;
        }
        else {
            // 画像の幅に合わせる必要あり
            newImgW = width;
            newImgH = (double)width / (double)imgW * imgH;
            yOffset = (height - newImgH) / 2;
        }

        StretchBlt(
            hdc,
            xOffset,
            yOffset,
            newImgW,
            newImgH,
            hMemDC,
            0,
            0,
            imgW,
            imgH,
            SRCCOPY
        );
        

        EndPaint(hwnd, &paint);

        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}