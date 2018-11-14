#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <mmsystem.h>
#include "Commctrl.h"
#include "resource.h"
#pragma comment(lib,"winmm.lib")
#define ID_TIMER1  2000
#define MENUADD 3000
#define MENUDEL 3001
#define MENUADDLRC 3002
#define MENUDELLRC 3003
#define MENUCLS 3004
#define MENURM 3005
#define LTLOOPPLY 0
#define SGLOOPPLY 1
#define RANDPLY 2
#define DEFAULT_VOICE_SIZE 500
#define MAX_PATH 10240

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Cls_OnHScroll(HWND hDlg, HWND hwndCtl, UINT code, int pos);
void Cls_OnLButtonDown(HWND hDlg, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void Cls_OnTimer(HWND hwnd, UINT id);
void Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);
void Cls_OnClose(HWND hDlg);

void Start(HWND hwnd);
void StartMusic(TCHAR *MusicName);
void StopMusic(TCHAR *MusicName);
void PauseMusic(TCHAR *MusicName);
void ResumeMusic(TCHAR *MusicName);
void PlayMusicFromPos(TCHAR *MusicName, int pos);
int GetMusicMsec(TCHAR *MusicName);
int GetMusicPosMsec(TCHAR *MusicName);
void SetMusicVoice(TCHAR *MusicName, long lVoiceSize);
void PlayModel(HWND hwnd, int iMod);
void ListLoopPlay(HWND hwnd);
void SingleLoopPlay(HWND hwnd);
void RandLoopPlay(HWND hwnd);

void DrawMusicBanner(hDlg, MusicMsec, MusicMsecTotal);

void AddMusic(HWND hwnd);
void LoadMusic(HWND hwnd);
void DelMusic(HWND hwnd);
void ClsMusic(HWND hwnd);
void ClsScreen(HWND hwnd);

void CutSuffix(TCHAR *p);
void ClickButton(HWND hwnd, UINT iButtonID);

TCHAR MusicListFile[MAX_PATH] = L"C://musiclist.ini";//定义音乐清单存放路径
TCHAR appName[10] = L"MUSIC"; //音乐清单中块的关键字
TCHAR musicShortPath[MAX_PATH]; //定义音乐短路径
int MusicPlayModel= LTLOOPPLY;//定义播放模式,SGLOOPPLY表示单曲循环，LTLOOPPLY表示列表循环，RANDPLY 表示随机播放
//char *PlayModelList[3] = { L"列表循环",L"单曲循环",L"随机播放" };
static HICON hIcon;  //定义Icon
static HWND hDlg_Com; //定义全局句柄
TCHAR CurrentMusic[MAX_PATH]=L""; //定义当前音乐的名字
HMENU hMenu; //定义快捷菜单


