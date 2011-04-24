#include	"CList\\CList.h"
#include	"Sound.h"
#include	"Gfx.h"



#ifndef		GAME_BOY_CPP

#define		GAME_BOY_CPP	extern

#endif		//GAME_BOY_CPP



#define			EMU_STEPINTO	1
#define			EMU_RUNTO		2
#define			EMU_STEPOUT		3



struct EMULATIONINFO
{
	DWORD		Flags;
	WORD		RunToOffset;
	BYTE		RunToBank;
};



struct CHEATDATA
{
	WORD		Offset;
	BYTE		Bank;
	static union
	{
		BYTE	Value;
		BYTE	OldValue;
	};
	char		*pszCode;
};



enum
{
	TERMINATING_FALSE = 0,
	TERMINATING_STOP,
	TERMINATING_PAUSE
};



#define		GB_COLOR				0x00000001
//#define		GB_SUPER			0x00000002
#define		GB_ROM_COLOR			0x00000004
//#define		GB_ROM_SUPER		0x00000008
#define		GB_EXITLOOPDIRECTLY		0x00000010
#define		GB_EXITLOOP				0x00000020
#define		GB_INVALIDOPCODE		0x00000040
#define		GB_RAMENABLE			0x00000080
#define		GB_HALT					0x00010000
#define		GB_ENABLEIE				0x00020000
#define		GB_IE					0x00040000
#define		GB_HDMA					0x00100000
#define		GB_HASRUMBLEPACK		0x00200000
#define		GB_RUMBLE				0x00400000
#define		GB_SERIALBIT			0x00800000
#define		GB_LINKCABLE			0x01000000
#define		GB_ERROR				0x10000000
#define		GB_DEBUGRUNINFO			0x20000000
#define		GB_DEBUGINFO			0x40000000
#define		GB_DEBUG				0x80000000



#define		Flag_C					0x10
#define		Flag_H					0x20
#define		Flag_N					0x40
#define		Flag_Z					0x80


#define		MEM_READ				0x01	//Only initialized data may be read (set when writing to address)
#define		MEM_WRITE				0x02	//Only reserved space and some hardware addresses may be written to
#define		MEM_EXECUTE				0x04	//Valid address for PC (must be combined with MEM_READ)
#define		MEM_STACK				0x08	//Valid address for SP
#define		MEM_BREAKPOINT			0x10
#define		MEM_CHANGED				0x20
#define		MEM_FIXED				0x40	//Data cannot be changed (due to cheats)


#define		Offset_Reg_F			0
#define		Offset_Reg_A			1
#define		Offset_Reg_AF			0
#define		Offset_Reg_C			4
#define		Offset_Reg_B			5
#define		Offset_Reg_BC			4
#define		Offset_Reg_E			8
#define		Offset_Reg_D			9
#define		Offset_Reg_DE			8
#define		Offset_Reg_L			12
#define		Offset_Reg_H			13
#define		Offset_Reg_HL			12
#define		Offset_Reg_SP			16
#define		Offset_Reg_PC			20
#define		Offset_Flags			24

#define		Offset_ActiveRomBank	28
#define		Offset_ActiveRamBank	29
#define		Offset_MaxRomBank		30
#define		Offset_MaxRamBank		31

#define		Offset_MEM				32
#define		Offset_MEM_CPU			0x60
#define		Offset_MEM_ROM			0x009060
#define		Offset_MEM_RAM			0x009064
#define		Offset_MEM_VRAM			0x02B064

#define		Offset_MemStatus		0x02F064
#define		Offset_MemStatus_CPU	0x02F0A4
#define		Offset_MemStatus_ROM	0x0380A4
#define		Offset_MemStatus_RAM	0x0380A8
#define		Offset_MemStatus_VRAM	0x0380AC

#define		Offset_BGP				0x03C0AC //0x40
#define		Offset_OBP				0x03C0EC //0x40

#define		Offset_DirectionKeys	0x03C12C //1
#define		Offset_Buttons			0x03C12D //1
#define		Offset_LCD_Ticks		0x03C12E //1
#define		Offset_DIV_Ticks		0x03C12F //1
#define		Offset_TIMA_Ticks		0x03C130 //4
#define		Offset_Hz				0x03C134 //4

#define		Offset_nCycles			0x03C138 //4

#define		Offset_pGBBitmap		0x03C13C //4

#define		Offset_WindowY2			0x03C140 //1

//#define		Offset_DrawLineMask		0x03A142 //1

#define		Offset_SoundTicks		0x03C141 //1
#define		Offset_Sound1Enabled	0x03C142 //1
#define		Offset_Sound2Enabled	0x03C143 //1
#define		Offset_Sound3Enabled	0x03C144 //1
#define		Offset_Sound4Enabled	0x03C145 //1
#define		Offset_Sound1Stage		0x03C146 //1
#define		Offset_Sound2Stage		0x03C147 //1
#define		Offset_Sound3Stage		0x03C148 //1
#define		Offset_Sound4Bit		0x03C149 //1
#define		Offset_Sound1Volume		0x03C14A //1
#define		Offset_Sound2Volume		0x03C14B //1
#define		Offset_Sound4Volume		0x03C14C //1
#define		Offset_SoundL			0x03C150 //4
#define		Offset_SoundR			0x03C154 //4
#define		Offset_Sound1Ticks		0x03C158 //4
#define		Offset_Sound1TimeOut	0x03C15C //4
#define		Offset_Sound1Frequency	0x03C160 //4
#define		Offset_Sound1Envelope	0x03C164 //4
#define		Offset_Sound1Sweep		0x03C168 //4
#define		Offset_Sound2Ticks		0x03C16C //4
#define		Offset_Sound2TimeOut	0x03C170 //4
#define		Offset_Sound2Frequency	0x03C174 //4
#define		Offset_Sound2Envelope	0x03C178 //4
#define		Offset_Sound3Ticks		0x03C17C //4
#define		Offset_Sound3TimeOut	0x03C180 //4
#define		Offset_Sound3Frequency	0x03C184 //4
#define		Offset_Sound4Ticks		0x03C188 //4
#define		Offset_Sound4TimeOut	0x03C18C //4
#define		Offset_Sound4Frequency	0x03C190 //4
#define		Offset_Sound4Envelope	0x03C194 //4
#define		Offset_FastFwd			0x03C198 //1
#define		Offset_FramesToSkip		0x03C199 //1
#define		Offset_FrameSkip		0x03C19A //1
#define		Offset_pAVISoundBuffer	0x03C19C //4
#define		Offset_dwAVISoundPos	0x03C1A0 //4
#define		Offset_pPalette			0x03C1A4 //4

#define		Offset_RTC				0x03C1A8
#define		Offset_SEC				0x03C1A8 //1
#define		Offset_MIN				0x03C1A9 //1
#define		Offset_HRS				0x03C1AA //1
#define		Offset_DAY				Offset_DAYL
#define		Offset_DAYL				0x03C1AB //1
#define		Offset_DAYH				0x03C1AC //1
#define		Offset_Latch			0x03C1AD //1
#define		Offset_RTC_Reg			0x03C1AE //1
#define		Offset_RTC_Day0			0x03C1B0 //8

#define		Offset_SerialInput		0x03C1B8 //1
#define		Offset_SerialBit		0x03C1B9 //1
#define		Offset_SerialTicks		0x03C1BC //4
#define		Offset_LinkGameBoy		0x03C1C0 //4
#define		Offset_dwNetworkLinkCableNo	0x03C1C4 //4

#define		Offset_Terminating		0x03C1C8 //1

#define		Offset_SoundBuffer		0x03C1CC


#define		FF00_C(Offset)			MEM_CPU[0x8F00 + Offset]
#define		pFF00_C(GB, Offset)		GB->MEM_CPU[0x8F00 + Offset]
#define		FF00_ASM				(Offset_MEM_CPU + 0x8F00)

//#define		ROM(n)					(*((BYTE *)(n + (DWORD)MEM_ROM)))
//#define		pROM(n)					((BYTE *)(n + (DWORD)MEM_ROM))

#define		pMemStatus_ROM(n)		((BYTE *)(n + (DWORD)MemStatus_ROM))



class CGameBoy
{
public:
	static union
	{
		static struct
		{
			BYTE	Reg_F;
			BYTE	Reg_A;
			WORD	dummy;
		};
		WORD		Reg_AF;
	};
	static union
	{
		static struct
		{
			BYTE	Reg_C;
			BYTE	Reg_B;
			WORD	dummy;
		};
		WORD		Reg_BC;
	};
	static union
	{
		static struct
		{
			BYTE	Reg_E;
			BYTE	Reg_D;
			WORD	dummy;
		};
		WORD		Reg_DE;
	};
	static union
	{
		static struct
		{
			BYTE	Reg_L;
			BYTE	Reg_H;
			WORD	dummy;
		};
		WORD		Reg_HL;
	};
	static struct
	{
		WORD		Reg_SP;
		WORD		dummy;
	};
	static struct
	{
		WORD		Reg_PC;
		WORD		dummy;
	};
	DWORD			Flags;

	BYTE			ActiveRomBank;
	BYTE			ActiveRamBank;
	BYTE			MaxRomBank;
	BYTE			MaxRamBank;

	BYTE			*MEM[0x10];
	BYTE			MEM_CPU[0x9000];
	BYTE			*MEM_ROM;
	BYTE			MEM_RAM[0x022000];
	BYTE			MEM_VRAM[0x4000];

	BYTE			*MemStatus[0x10];
	BYTE			MemStatus_CPU[0x9000];
	BYTE			*MemStatus_ROM;
	BYTE			*MemStatus_RAM;
	BYTE			MemStatus_VRAM[0x4000];

	BYTE			BGP[0x40];
	BYTE			OBP[0x40];

	BYTE			DirectionKeys;
	BYTE			Buttons;

	BYTE			LCD_Ticks;
	BYTE			DIV_Ticks;
	DWORD			TIMA_Ticks;
	DWORD			Hz;

	DWORD			nCycles;

	WORD			*pGBBitmap;

	BYTE			WindowY2;

	//BYTE			DrawLineMask;


	BYTE			SoundTicks;
	BYTE			Sound1Enabled, Sound2Enabled, Sound3Enabled, Sound4Enabled;
	BYTE			Sound1Stage, Sound2Stage, Sound3Stage, Sound4Bit;

	BYTE			Sound1Volume, Sound2Volume, Sound4Volume;

	int				SoundL, SoundR;

	DWORD			Sound1Ticks, Sound1TimeOut, Sound1Frequency, Sound1Envelope, Sound1Sweep;
	DWORD			Sound2Ticks, Sound2TimeOut, Sound2Frequency, Sound2Envelope;
	DWORD			Sound3Ticks, Sound3TimeOut, Sound3Frequency;
	DWORD			Sound4Ticks, Sound4TimeOut, Sound4Frequency, Sound4Envelope;

	BYTE			FastFwd;
	BYTE			FramesToSkip, FrameSkip;

	void			*pAVISoundBuffer;
	DWORD			dwAVISoundPos;

	WORD			*pPalette;

	BYTE			RTC_SEC, RTC_MIN, RTC_HRS;
	BYTE			RTC_DAYL, RTC_DAYH;
#define				RTC_DAY		(*(WORD *)&RTC_DAYL)
	BYTE			RTC_Latch, RTC_Reg;
	FILETIME		RTC_Day0;

	BYTE			SerialInput, SerialBit;
	DWORD			SerialTicks;
	CGameBoy		*pLinkGameBoy;
	DWORD			dwNetworkLinkCableNo;

	BYTE			Terminating;

	SOUNDBUFFER		SoundBuffer;

	int				JoyLeft, JoyRight, JoyUp, JoyDown;
	BYTE			AutoButtonDown;

	BYTE			StateSlot;


	IDirect3DDevice8	*m_pd3dd;
	HWND			hGfxWnd;

	HWND			hGBWnd;
	HDC				hGBDC;
	HBITMAP			hGBBitmap, hOldBitmap;


#ifdef	CDEBUGINFO_H
	CDebugInfo		*pDebugInfo;
#else	//CDEBUGINFO_H
	void			*pDebugInfo;
#endif	//CDEBUGINFO_H


	DWORD			ThreadId;


private:
	DWORD			RefCount;

	HANDLE			hThread;

	char			Rom[MAX_PATH];
	char			Battery[MAX_PATH];
	unsigned int	SaveRamSize;
	BYTE			BatteryAvailable;
	BYTE			Emulating;

	LARGE_INTEGER	LastTimerCount;
	BOOL			SkipNextFrame;
	char			FramesSkipped;

	CList			*m_pCheatList;

	HWND			m_hSearchCheatWnd;
	HWND			m_hSize, m_hNewSearch, m_hValue, m_hDec;
	HWND			m_hRelValue, m_hRelLast, m_hEqual, m_hNotEqual, m_hLessEqual, m_hMoreEqual, m_hLess, m_hMore;
	HWND			m_hNameStatic, m_hName, m_hAddCheat;
	BYTE			m_SearchSize;

public:
	HWND			m_hHex, m_hList;
	BYTE			m_SortBy;


private:
	BOOL			StartThread();
	void			LinkExecuteLoop();
	void			ExecuteLoop();
	void			LinkDebugLoop();
	void			DebugLoop();
	void			StepLoop(EMULATIONINFO *pEmulationInfo);
	void			SetStartDelay();
	void			Delay();

	void			ClearDebugRunInfo();
	void			PrepareEmulation(BOOL Debug);
	void			MainLoop();
	void			DebugMainLoop();
	void			RefreshScreen();
	BOOL			RestoreSound();

	void			CloseAVI();
	BOOL			WriteAVI();

	char			*GetStateFilename(char *pszFilename);

	BYTE			GetRealByte(WORD Offset, BYTE Bank);
	BOOL			AddCheat(WORD Offset, BYTE Value, char *pszCode);
	BOOL			AddCheat(WORD Offset, BYTE Value, BYTE CompareValue, char *pszCode);
	BOOL			AddCheat(BYTE Bank, WORD Offset, BYTE Value, char *pszCode);
	void			ReApplyCheats();

	void			PerformSearch(UINT uID);


public:
	CGameBoy(BYTE Flags);
	~CGameBoy();

	void			AddRef();
	void			Release();
	BOOL			CanUnload();

	BOOL			Init(char *pszROMFilename, char *pszStateFilename, char *pszBatteryFilename);
	BOOL			LoadBattery(char *BatteryFilename);
	BOOL			SaveBattery(BOOL Prompt, BOOL SaveAs);
	void			GetBatteryFilename(char *Filename);
	char			*GetStateFilename(char *pszFilename, DWORD dwStateSlot);
	LRESULT			GameBoyWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void			Reset(DWORD Flags);
	void			Reset();
	void			CloseSound();

	DWORD			ThreadProc();

	BOOL			IsEmulating();
	BOOL			HasBattery();

	BOOL			Execute();
	BOOL			StartDebug();
	BOOL			Step(EMULATIONINFO *pEmulationInfo);
	void			Resume();
	void			Pause();
	void			Stop();

	void			SetFrameSkip(int nFrameSkip);

	BOOL			SaveState();
	BOOL			SaveState(char *pszFilename);
	BOOL			SaveStateAs();
	BOOL			LoadState();
	BOOL			LoadState(char *pszFilename, BOOL QuickLoad);
	BOOL			LoadStateAs();
	void			SetStateSlot(int nSlot);
	int				GetStateSlot();

	BOOL			SaveSnapshot();
	BOOL			SaveVideo();

	BOOL			SwitchRomBank(BYTE Bank);
	BOOL			SwitchRamBank(BYTE Bank);
	BOOL			SwitchVBK(BYTE Bank);
	BOOL			SwitchSVBK(BYTE Bank);

	void			RemoveCheats();
	int				VerifyCode(char *pszCode, BOOL CompareValue);
	BOOL			AddCheat(char *pszCode);
	BOOL			IsApplied(char *pszCode);
	void			SearchCheat();
	LRESULT			SearchCheatWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};



enum GameBoyMessages
{
	WM_APP_STEP = WM_APP_FIRSTFREEMESSAGE,
	//WM_APP_RESUME
};



GAME_BOY_CPP	BYTE		ZeroStatus[0x1000];

extern BYTE					RomSize(BYTE Byte148);
extern void __fastcall		SoundUpdate(CGameBoy *GB);

