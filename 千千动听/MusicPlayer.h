#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include <mmsystem.h>
#include "Commctrl.h"
#pragma comment(lib,"winmm.lib")
#define ID_TIMER1  2000
#define MENUADD 3000
#define MENUDEL 3001
#define MENUCLS 3002
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Cls_OnHScroll(HWND hDlg, HWND hwndCtl, UINT code, int pos);
void Cls_OnLButtonDown(HWND hDlg, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);
void Cls_OnClose(HWND hDlg);
void Start();
void StartMusic(TCHAR *MusicName);
void StopMusic(TCHAR *MusicName);
void PauseMusic(TCHAR *MusicName);
void ResumeMusic(TCHAR *MusicName);
void PlayMusicFromPos(TCHAR *MusicName, int pos);
int GetMusicMsec(TCHAR *MusicName);
int GetMusicPosMsec(TCHAR *MusicName);
void DrawMusicBanner();
void AddMusic(HWND hwnd);
void LoadMusic(HWND hwnd);
void DelMusic(HWND hwnd);
void ClsMusic(HWND hwnd);
void ClsScreen(HWND hwnd);
void CutSuffix(TCHAR *p);


TCHAR appName[10] = L"MUSIC";

TCHAR MusicListFile[MAX_PATH] = L"C://musiclist.ini";
TCHAR musicShortPath[MAX_PATH];
static HICON hIcon;
static HWND hDlg_Com;
TCHAR CurrentMusic[MAX_PATH]=L"";
HMENU hMenu; //定义快捷菜单


