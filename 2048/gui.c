/*
 * 2048 - Windows GUI, written in C with Win32 API + GDI.
 *
 * This is a COMPLETE GUI entry point. It reuses game.c and game.h from
 * the terminal version of the 2048 project. Do NOT compile main.c here.
 *
 * MinGW-w64 build (PowerShell):
 *   gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -finput-charset=UTF-8 \
 *       gui.c game.c -o 2048_gui.exe -mwindows -lgdi32 -luser32
 *
 * Controls: WASD / arrow keys to move; R or mouse button to restart.
 */

#include <windows.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include "game.h"

#define WINDOW_W       520
#define WINDOW_H       710
#define BOARD_X        40
#define BOARD_Y        160
#define CELL_SIZE      100
#define CELL_GAP       8
#define BOARD_PIXELS   (CELL_GAP + SIZE * (CELL_SIZE + CELL_GAP))
#define BUTTON_X       40
#define BUTTON_Y       618
#define BUTTON_W       176
#define BUTTON_H       49
#define RESULT_BUTTON_X 130
#define RESULT_BUTTON_Y 460
#define RESULT_BUTTON_W 260
#define RESULT_BUTTON_H 48
#define RECORD_TIMER    1

/* Only the game engine's public functions are in game.h. */
enum GameStatus { GAME_RUNNING, GAME_WON, GAME_LOST };
static int board[SIZE][SIZE] = {{0}};
static int score = 0;
static int bestScore = 0;
static int newRecord = 0;
static int recordNotice = 0;
static int saveFailed = 0;
static wchar_t scorePath[MAX_PATH];
static enum GameStatus status = GAME_RUNNING;

/* Per-user storage is independent of the executable and working directory. */
static void loadBestScore(void) {
    wchar_t directory[MAX_PATH];
    DWORD length = GetEnvironmentVariableW(L"LOCALAPPDATA", directory, MAX_PATH);
    bestScore = 0;
    scorePath[0] = L'\0';
    if (length == 0 || length >= MAX_PATH - 32) return;
    wcscat(directory, L"\\C2048");
    if (!CreateDirectoryW(directory, NULL) &&
        GetLastError() != ERROR_ALREADY_EXISTS) return;
    swprintf(scorePath, MAX_PATH, L"%ls\\scores.ini", directory);

    wchar_t text[64], *end;
    GetPrivateProfileStringW(L"2048", L"BestScore", L"0",
                            text, 64, scorePath);
    errno = 0;
    long value = wcstol(text, &end, 10);
    if (errno == 0 && end != text && *end == L'\0' &&
        value >= 0 && value <= INT_MAX) bestScore = (int)value;
}

static void saveBestScore(void) {
    wchar_t text[32];
    swprintf(text, 32, L"%d", bestScore);
    saveFailed = scorePath[0] == L'\0' ||
        !WritePrivateProfileStringW(L"2048", L"BestScore", text, scorePath);
}

static void updateBestScore(HWND hwnd) {
    if (score <= bestScore) return;
    bestScore = score;
    if (!newRecord) {
        recordNotice = 1;
        SetTimer(hwnd, RECORD_TIMER, 3500, NULL);
    }
    newRecord = 1;
    saveBestScore();
}

static void resetGame(void) {
    memset(board, 0, sizeof(board));
    score = 0;
    status = GAME_RUNNING;
    newRecord = 0;
    recordNotice = 0;
    addRandomTile(board);
    addRandomTile(board);
}

static void restartGame(HWND hwnd) {
    KillTimer(hwnd, RECORD_TIMER);
    resetGame();
    InvalidateRect(hwnd, NULL, FALSE);
}

static COLORREF tileBackground(int value) {
    switch (value) {
        case 0:    return RGB(205, 193, 180);
        case 2:    return RGB(238, 228, 218);
        case 4:    return RGB(237, 224, 200);
        case 8:    return RGB(242, 177, 121);
        case 16:   return RGB(245, 149, 99);
        case 32:   return RGB(246, 124, 95);
        case 64:   return RGB(246, 94, 59);
        case 128:  return RGB(237, 207, 114);
        case 256:  return RGB(237, 204, 97);
        case 512:  return RGB(237, 200, 80);
        case 1024: return RGB(237, 197, 63);
        case 2048: return RGB(237, 194, 46);
        default:   return RGB(95, 88, 80);
    }
}

static void fillRoundRect(HDC dc, int x, int y, int width, int height,
                          COLORREF background, int radius) {
    HBRUSH brush = CreateSolidBrush(background);
    if (brush == NULL) return;

    HGDIOBJ oldBrush = SelectObject(dc, brush);
    HGDIOBJ oldPen = SelectObject(dc, GetStockObject(NULL_PEN));
    RoundRect(dc, x, y, x + width, y + height, radius, radius);
    SelectObject(dc, oldPen);
    SelectObject(dc, oldBrush);
    DeleteObject(brush);
}

static void drawLabel(HDC dc, const wchar_t *text,
                      int x, int y, int width, int height,
                      int fontHeight, int fontWeight, COLORREF foreground,
                      UINT alignment) {
    HFONT font = CreateFontW(
        -fontHeight, 0, 0, 0, fontWeight,
        FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Microsoft YaHei"
    );
    HGDIOBJ oldFont = NULL;
    if (font != NULL) oldFont = SelectObject(dc, font);

    RECT bounds = { x, y, x + width, y + height };
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, foreground);
    DrawTextW(dc, text, -1, &bounds,
              alignment | DT_SINGLELINE | DT_VCENTER);

    if (font != NULL) {
        SelectObject(dc, oldFont);
        DeleteObject(font);
    }
}

static void drawResult(HDC dc) {
    COLORREF accent = newRecord ? RGB(181, 125, 29) : RGB(143, 122, 102);
    /* In-board modal card: the final board remains visible around its edges. */
    fillRoundRect(dc, 69, 245, 390, 300, RGB(143, 122, 102), 24);
    fillRoundRect(dc, 60, 236, 390, 300, RGB(255, 252, 244), 24);
    drawLabel(dc, status == GAME_WON ? L"恭喜达成 2048！" : L"游戏结束",
              80, 252, 350, 46, 30, FW_BOLD, RGB(119, 110, 101), DT_CENTER);
    drawLabel(dc, newRecord ? L"★ 历史新高！你刷新了自己的纪录" :
              (status == GAME_WON ? L"每一次合并，都是进步" : L"棋盘已满，没有可合并的方块"),
              75, 304, 360, 30, 16, FW_BOLD, accent, DT_CENTER);
    fillRoundRect(dc, 85, 350, 158, 84, RGB(238, 228, 218), 12);
    fillRoundRect(dc, 267, 350, 158, 84, RGB(237, 224, 200), 12);
    drawLabel(dc, L"本局得分", 85, 357, 158, 24, 14, FW_NORMAL,
              RGB(119, 110, 101), DT_CENTER);
    drawLabel(dc, L"历史最高分", 267, 357, 158, 24, 14, FW_NORMAL,
              RGB(119, 110, 101), DT_CENTER);
    wchar_t text[32];
    swprintf(text, 32, L"%d", score);
    drawLabel(dc, text, 85, 386, 158, 38, 26, FW_BOLD,
              RGB(119, 110, 101), DT_CENTER);
    swprintf(text, 32, L"%d", bestScore);
    drawLabel(dc, text, 267, 386, 158, 38, 26, FW_BOLD, accent, DT_CENTER);
    fillRoundRect(dc, RESULT_BUTTON_X, RESULT_BUTTON_Y,
                  RESULT_BUTTON_W, RESULT_BUTTON_H, accent, 14);
    drawLabel(dc, L"再玩一局  ·  Enter / R", RESULT_BUTTON_X, RESULT_BUTTON_Y,
              RESULT_BUTTON_W, RESULT_BUTTON_H, 18, FW_BOLD,
              RGB(255, 255, 255), DT_CENTER);
}

static void drawInterface(HDC dc, HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH background = CreateSolidBrush(RGB(250, 248, 239));
    if (background != NULL) {
        FillRect(dc, &rect, background);
        DeleteObject(background);
    }

    /* Title, subtitle, score panels. */
    drawLabel(dc, L"2048", 40, 26, 238, 76, 58, FW_BOLD,
              RGB(119, 110, 101), DT_LEFT);
    drawLabel(dc, L"合并相同数字，挑战 2048！", 40, 112, 280, 26,
              16, FW_NORMAL, RGB(119, 110, 101), DT_LEFT);

    fillRoundRect(dc, 286, 39, 89, 68, RGB(187, 173, 160), 12);
    fillRoundRect(dc, 383, 39, 97, 68, RGB(187, 173, 160), 12);
    drawLabel(dc, L"得分", 287, 42, 86, 22, 13, FW_BOLD,
              RGB(238, 228, 218), DT_CENTER);
    drawLabel(dc, L"历史最高分", 384, 42, 95, 22, 13, FW_BOLD,
              RGB(238, 228, 218), DT_CENTER);

    wchar_t valueText[32];
    swprintf(valueText, sizeof(valueText) / sizeof(valueText[0]),
             L"%d", score);
    drawLabel(dc, valueText, 287, 65, 86, 35, 24, FW_BOLD,
              RGB(255, 255, 255), DT_CENTER);
    swprintf(valueText, sizeof(valueText) / sizeof(valueText[0]),
             L"%d", bestScore);
    drawLabel(dc, valueText, 384, 65, 95, 35, 24, FW_BOLD,
              RGB(255, 255, 255), DT_CENTER);
    if (recordNotice) {
        fillRoundRect(dc, 326, 112, 154, 30, RGB(237, 224, 200), 10);
        drawLabel(dc, L"★ 历史新高！", 326, 112, 154, 30, 16, FW_BOLD,
                  RGB(161, 108, 24), DT_CENTER);
    }

    /* 4x4 board with colored tiles. */
    fillRoundRect(dc, BOARD_X, BOARD_Y, BOARD_PIXELS, BOARD_PIXELS,
                  RGB(187, 173, 160), 14);
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            int tile = board[row][col];
            int x = BOARD_X + CELL_GAP + col * (CELL_SIZE + CELL_GAP);
            int y = BOARD_Y + CELL_GAP + row * (CELL_SIZE + CELL_GAP);
            fillRoundRect(dc, x, y, CELL_SIZE, CELL_SIZE,
                          tileBackground(tile), 12);

            if (tile != 0) {
                wchar_t tileText[24];
                swprintf(tileText, sizeof(tileText) / sizeof(tileText[0]),
                         L"%d", tile);
                int fontSize = tile < 1000 ? 30 : (tile < 10000 ? 25 : 19);
                COLORREF textColor = tile <= 4
                    ? RGB(119, 110, 101) : RGB(255, 255, 255);
                drawLabel(dc, tileText, x, y, CELL_SIZE, CELL_SIZE,
                          fontSize, FW_BOLD, textColor, DT_CENTER);
            }
        }
    }

    /* Clickable restart button. */
    fillRoundRect(dc, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H,
                  RGB(143, 122, 102), 12);
    drawLabel(dc, L"重新开始 (R)", BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H,
              18, FW_BOLD, RGB(255, 255, 255), DT_CENTER);
    drawLabel(dc, L"WASD / 方向键移动", 226, BUTTON_Y,
              254, BUTTON_H, 15, FW_NORMAL,
              RGB(119, 110, 101), DT_RIGHT);

    const wchar_t *tip = L"移动并合并数字，努力达到 2048";
    if (status == GAME_WON) tip = L"恭喜获胜！按 R 再玩一局";
    if (status == GAME_LOST) tip = L"游戏结束！按 R 再试一次";
    if (saveFailed) tip = L"最高分暂未保存成功，请检查本地目录权限";
    drawLabel(dc, tip, 40, 675, 440, 25, 14, FW_NORMAL,
              RGB(119, 110, 101), DT_CENTER);
    if (status != GAME_RUNNING) drawResult(dc);
}

static void performMove(HWND hwnd, WPARAM key) {
    if (status != GAME_RUNNING) return;

    int oldBoard[SIZE][SIZE];
    memcpy(oldBoard, board, sizeof(oldBoard));

    switch (key) {
        case VK_LEFT:
        case 'A': moveLeft(board, &score); break;
        case VK_RIGHT:
        case 'D': moveRight(board, &score); break;
        case VK_UP:
        case 'W': moveUp(board, &score); break;
        case VK_DOWN:
        case 'S': moveDown(board, &score); break;
        default: return;
    }
    
    if (boardsEqual(oldBoard, board)) {
        if (!hasMoves(board)) {
            status = GAME_LOST;
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return; /* No spawn on invalid move. */
    }
    updateBestScore(hwnd);

    if (hasWon(board)) {
        status = GAME_WON;
    } else {
        addRandomTile(board);
        if (!hasMoves(board)) status = GAME_LOST;
    }

    InvalidateRect(hwnd, NULL, FALSE);
}

static LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg,
                                        WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            loadBestScore();
            resetGame();
            return 0;

        case WM_KEYDOWN:
            if (wParam == 'R' || (wParam == VK_RETURN && status != GAME_RUNNING)) {
                /* Ignore held restart keys so a new board is dealt only once. */
                if (!(lParam & (1L << 30))) restartGame(hwnd);
                return 0;
            }
            performMove(hwnd, wParam);
            return 0;

        case WM_LBUTTONDOWN: {
            int x = (short)LOWORD(lParam);
            int y = (short)HIWORD(lParam);
            if (status != GAME_RUNNING) {
                if (x >= RESULT_BUTTON_X && x < RESULT_BUTTON_X + RESULT_BUTTON_W &&
                    y >= RESULT_BUTTON_Y && y < RESULT_BUTTON_Y + RESULT_BUTTON_H)
                    restartGame(hwnd);
            } else if (x >= BUTTON_X && x < BUTTON_X + BUTTON_W &&
                y >= BUTTON_Y && y < BUTTON_Y + BUTTON_H) {
                restartGame(hwnd);
            }
            return 0;
        }

        case WM_TIMER:
            if (wParam == RECORD_TIMER) {
                recordNotice = 0;
                KillTimer(hwnd, RECORD_TIMER);
                InvalidateRect(hwnd, NULL, FALSE);
            }
            return 0;

        case WM_ERASEBKGND:
            return 1; /* Entire background is drawn in WM_PAINT. */

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hwnd, &ps);
            RECT area;
            GetClientRect(hwnd, &area);
            int width = area.right - area.left;
            int height = area.bottom - area.top;

            /* Double-buffer the frame so updates don't visibly flicker. */
            HDC memDC = CreateCompatibleDC(dc);
            if (memDC != NULL) {
                HBITMAP bitmap = CreateCompatibleBitmap(dc, width, height);
                if (bitmap != NULL) {
                    HGDIOBJ oldBitmap = SelectObject(memDC, bitmap);
                    drawInterface(memDC, hwnd);
                    BitBlt(dc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
                    SelectObject(memDC, oldBitmap);
                    DeleteObject(bitmap);
                } else {
                    drawInterface(dc, hwnd);
                }
                DeleteDC(memDC);
            } else {
                drawInterface(dc, hwnd);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            KillTimer(hwnd, RECORD_TIMER);
            if (saveFailed && bestScore > 0) saveBestScore();
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

/* WinMain is required by -mwindows. This file must be built without main.c. */
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous,
                   LPSTR commandLine, int showMode) {
    (void)previous;
    (void)commandLine;
    srand((unsigned int)time(NULL));

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = windowProcedure;
    wc.hInstance = instance;
    wc.lpszClassName = L"C2048CompleteWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); /* ANSI macro matches IDC_ARROW. */
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"无法注册游戏窗口。", L"2048 错误", MB_OK | MB_ICONERROR);
        return 1;
    }

    DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT dimensions = {0, 0, WINDOW_W, WINDOW_H};
    if (!AdjustWindowRect(&dimensions, windowStyle, FALSE)) return 1;
    int w = dimensions.right - dimensions.left;
    int h = dimensions.bottom - dimensions.top;
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    HWND hwnd = CreateWindowExW(
        0, wc.lpszClassName, L"2048 - C 语言图形版", windowStyle,
        x, y, w, h, NULL, NULL, instance, NULL
    );
    if (hwnd == NULL) {
        MessageBoxW(NULL, L"无法创建游戏窗口。", L"2048 错误", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, showMode);
        UpdateWindow(hwnd);

    MSG message;
    BOOL result;

    while ((result = GetMessageW(&message, NULL, 0, 0)) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    return result == -1 ? 1 : (int)message.wParam;
}
