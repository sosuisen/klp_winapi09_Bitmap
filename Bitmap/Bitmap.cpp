#include <windows.h>
#include <string>
#include <chrono>

static const int WIN_WIDTH = 1000;
static const int WIN_HEIGHT = 500;

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
        WIN_WIDTH, WIN_HEIGHT,
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

DWORD mosaic5x5(int shift, LPDWORD src, int index, int width) {
    return ((src[index] >> shift & 0xff)
        + (src[index + 1] >> shift & 0xff)
        + (src[index + 2] >> shift & 0xff)
        + (src[index + 3] >> shift & 0xff)
        + (src[index + 4] >> shift & 0xff)
        + (src[index + width] >> shift & 0xff)
        + (src[index + 1 + width] >> shift & 0xff)
        + (src[index + 2 + width] >> shift & 0xff)
        + (src[index + 3 + width] >> shift & 0xff)
        + (src[index + 4 + width] >> shift & 0xff)
        + (src[index + width * 2] >> shift & 0xff)
        + (src[index + 1 + width * 2] >> shift & 0xff)
        + (src[index + 2 + width * 2] >> shift & 0xff)
        + (src[index + 3 + width * 2] >> shift & 0xff)
        + (src[index + 4 + width * 2] >> shift & 0xff)
        + (src[index + width * 3] >> shift & 0xff)
        + (src[index + 1 + width * 3] >> shift & 0xff)
        + (src[index + 2 + width * 3] >> shift & 0xff)
        + (src[index + 3 + width * 3] >> shift & 0xff)
        + (src[index + 4 + width * 3] >> shift & 0xff)
        + (src[index + width * 4] >> shift & 0xff)
        + (src[index + 1 + width * 4] >> shift & 0xff)
        + (src[index + 2 + width * 4] >> shift & 0xff)
        + (src[index + 3 + width * 4] >> shift & 0xff)
        + (src[index + 4 + width * 4] >> shift & 0xff))/25;
}

void mosaic5x5Filter(LPDWORD src, LPDWORD dst, RECT rect, int index, int width) {
    int orgX = index % width;
    int orgY = index / width;

    if (orgX < rect.left || orgX > rect.right || orgY < rect.top || orgY > rect.bottom) {
        dst[index] = src[index];
        return;
    }

    // 以下、境界処理が必要ですが、省略。
    int x = orgX - orgX % 5;
    int y = orgY - orgY % 5;
    DWORD r = mosaic5x5(16, src, x + y * width, width);
    DWORD g = mosaic5x5(8, src, x + y * width, width);
    DWORD b = mosaic5x5(0, src, x + y * width, width);

    dst[index] = r << 16 | g << 8 | b;
}

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP  hBitmap;    // ビットマップ
    static HDC      hMemDC;     // オフスクリーン

    // 【別解】ビットマップを配列で処理する場合
    static LPDWORD  lpRGB;
    static BITMAPINFO bi = { 0 };


    static HDC hMemFilterDC;
    static HBITMAP hBitmapFilter;

    RECT rectFilter = { 600, 100, 840, 340 };

    HDC hdc;

    // benchmark
    std::chrono::system_clock::time_point start, end;
    long microsec = 0;

    switch (uMsg) {
    case WM_CREATE: {
        // ベンチマーク開始（発展課題用）
        start = std::chrono::system_clock::now();

        // オフスクリーンをメモリデバイスコンテキストを用いて作成
        hdc = GetDC(hwnd);

        HDC hMemDCTmp = CreateCompatibleDC(hdc);
        // 画像をファイルから読み込む
        HBITMAP hBitmapTmp = (HBITMAP)LoadImage(NULL, L"kyocotan.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        // ビットマップの情報を取得し、bmp_infoに保管
        BITMAP bmp_info = { 0 };
        GetObject(hBitmapTmp, (int)sizeof(BITMAP), &bmp_info);
        SelectObject(hMemDCTmp, hBitmapTmp);
        
        hMemDC = CreateCompatibleDC(hdc);
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = bmp_info.bmWidth;
        bi.bmiHeader.biHeight = bmp_info.bmHeight;
        bi.bmiHeader.biPlanes = bmp_info.bmPlanes;
        bi.bmiHeader.biBitCount = bmp_info.bmBitsPixel;
        bi.bmiHeader.biCompression = BI_RGB;
        hBitmap = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, (LPVOID*)&lpRGB, NULL, 0);
        SelectObject(hMemDC, hBitmap);
        // 画像ファイルの読込は手間なのでLoadImageしてBitBltしました。
        BitBlt(hMemDC, 0, 0, bmp_info.bmWidth, bmp_info.bmHeight, hMemDCTmp, 0, 0, SRCCOPY);

        DeleteDC(hMemDCTmp);
        DeleteObject(hBitmapTmp);
        ReleaseDC(hwnd, hdc);


        // フィルタ処理用の配列
        LPDWORD lpRGBFilter = new DWORD[bmp_info.bmWidth * bmp_info.bmHeight];

        // DIBの配列では左下が原点座標なので注意
        int top = bmp_info.bmHeight - rectFilter.bottom;
        int bottom = top + (rectFilter.bottom - rectFilter.top);
        RECT rectDIB = { rectFilter.left, top, rectFilter.right, bottom };

        for (int i = 0; i < bmp_info.bmWidth * bmp_info.bmHeight; i++) {
            mosaic5x5Filter(lpRGB, lpRGBFilter, rectDIB, i, bmp_info.bmWidth);
        }
        memcpy(lpRGB, lpRGBFilter, bmp_info.bmWidth * bmp_info.bmHeight * sizeof(DWORD));
        delete[] lpRGBFilter;

        // ベンチマーク終了
        end = std::chrono::system_clock::now();
        microsec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        OutputDebugString((std::to_wstring(microsec) + L"\n").c_str());

        return 0;
    }
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

        /*
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        int imgW = bi.bmiHeader.biWidth;
        int imgH = bi.bmiHeader.biHeight;

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
        */

        EndPaint(hwnd, &paint);

        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}