# 2048（C / Win32）

## 现有功能

- 4×4 棋盘，方向键或 WASD 移动；相同数字合并并计分。
- 每次有效移动后随机生成 2（90%）或 4（10%）；无效移动不生成数字。
- 合成 2048 获胜，无可用移动时结束。
- 彩色方块、双缓冲绘制，支持 R 或鼠标重新开始。
- 历史最高分自动保存到 `%LOCALAPPDATA%\C2048\scores.ini`，退出后仍保留。
- 本局第一次超过历史最高分时显示 3.5 秒“历史新高”提示；持平不算破纪录。
- 胜利或失败时显示棋盘内结算卡片，包含本局得分、历史最高分和破纪录标识。
  点击“再玩一局”或按 Enter / R 重开；结算期间不能继续移动。
- 保存失败时在底部提示，并在后续刷新纪录及退出时重试。

## 编译与运行（MinGW-w64 / PowerShell）

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -finput-charset=UTF-8 gui.c game.c -o 2048_gui.exe -mwindows -lgdi32 -luser32
.\2048_gui.exe
```

图形版只编译 `gui.c` 和 `game.c`；`main.c` 是独立的终端版入口，
历史最高分和结算卡片功能位于图形版。

## 验证

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic test.c game.c -o test.exe
.\test.exe
gcc -std=c11 -Wall -Wextra -Wpedantic -finput-charset=UTF-8 test_gui.c game.c -o test_gui.exe -lgdi32 -luser32
.\test_gui.exe
```

图形版测试使用隐藏窗口与独立临时目录，验证最高分读写、异常数据、
破纪录提示、胜负结算及键盘/鼠标重开，不使用玩家的历史记录。
