/* GUI integration checks using a hidden window and isolated temporary storage.
 * gcc -std=c11 -Wall -Wextra -Wpedantic test_gui.c game.c -o test_gui.exe -lgdi32 -luser32
 */
#include <assert.h>
#include <stdio.h>
#include "gui.c"

int main(void) {
    wchar_t temporary[MAX_PATH], isolated[MAX_PATH], savedPath[MAX_PATH];
    assert(GetTempPathW(MAX_PATH, temporary));
    assert(GetTempFileNameW(temporary, L"248", 0, isolated));
    assert(DeleteFileW(isolated));
    assert(CreateDirectoryW(isolated, NULL));
    assert(SetEnvironmentVariableW(L"LOCALAPPDATA", isolated));

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = windowProcedure;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = L"C2048TestWindow";
    assert(RegisterClassW(&wc));
    HWND hwnd = CreateWindowW(wc.lpszClassName, L"Test", WS_OVERLAPPED,
                              0, 0, WINDOW_W, WINDOW_H, NULL, NULL,
                              wc.hInstance, NULL);
    assert(hwnd);
    assert(bestScore == 0);
    wcscpy(savedPath, scorePath);

    score = 100;
    updateBestScore(hwnd);
    assert(bestScore == 100 && newRecord && recordNotice && !saveFailed);
    SendMessageW(hwnd, WM_TIMER, RECORD_TIMER, 0);
    assert(!recordNotice && newRecord);
    score = 104;
    updateBestScore(hwnd);
    assert(bestScore == 104 && !recordNotice); /* One notice per round. */
    restartGame(hwnd);
    assert(score == 0 && bestScore == 104 && !newRecord);
    score = 104;
    updateBestScore(hwnd);
    assert(!newRecord); /* Matching a record is not beating it. */
    bestScore = 0;
    loadBestScore();
    assert(bestScore == 104);
    puts("PASS: persistence, reset, tie, and one record notice per round");

    const wchar_t *invalid[] = {L"-12", L"abc", L"2147483648", L"12junk"};
    for (unsigned int i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
        assert(WritePrivateProfileStringW(L"2048", L"BestScore", invalid[i], scorePath));
        loadBestScore();
        assert(bestScore == 0);
    }
    scorePath[0] = L'\0';
    score = 200;
    updateBestScore(hwnd);
    assert(saveFailed && bestScore == 200);
    wcscpy(scorePath, savedPath);
    saveBestScore();
    assert(!saveFailed);
    puts("PASS: invalid stored scores and recoverable save failure");

    int blocked[SIZE][SIZE] = {
        {2, 4, 2, 4}, {4, 2, 4, 2}, {2, 4, 2, 4}, {4, 2, 4, 2}
    };
    restartGame(hwnd);
    memcpy(board, blocked, sizeof(board));
    performMove(hwnd, VK_LEFT);
    assert(status == GAME_LOST);
    performMove(hwnd, VK_RIGHT);
    assert(boardsEqual(board, blocked));
    SendMessageW(hwnd, WM_KEYDOWN, VK_RETURN, 0);
    assert(status == GAME_RUNNING && score == 0);
    memset(board, 0, sizeof(board));
    board[0][0] = board[0][1] = 1024;
    performMove(hwnd, VK_LEFT);
    assert(status == GAME_WON && score == 2048 && newRecord);

    HDC dc = GetDC(hwnd);
    HDC memory = CreateCompatibleDC(dc);
    HBITMAP bitmap = CreateCompatibleBitmap(dc, WINDOW_W, WINDOW_H);
    assert(memory && bitmap);
    HGDIOBJ old = SelectObject(memory, bitmap);
    drawInterface(memory, hwnd);
    SelectObject(memory, old);
    DeleteObject(bitmap);
    DeleteDC(memory);
    ReleaseDC(hwnd, dc);
    SendMessageW(hwnd, WM_LBUTTONDOWN, 0,
                 MAKELPARAM(RESULT_BUTTON_X + 10, RESULT_BUTTON_Y + 10));
    assert(status == GAME_RUNNING && score == 0 && bestScore == 2048);
    puts("PASS: loss/win, frozen final board, result rendering, keyboard/mouse restart");

    DestroyWindow(hwnd);
    assert(DeleteFileW(savedPath));
    wchar_t *separator = wcsrchr(savedPath, L'\\');
    assert(separator);
    *separator = L'\0';
    assert(RemoveDirectoryW(savedPath));
    assert(RemoveDirectoryW(isolated));
    return 0;
}
