////////////////////////////////////////////////////////////////////////////////
// TrackMouseEvent() API re-implementation
// for using on the very first release of Windows 95
// Coder: Dandy Cheung
// 

#ifndef __TME_H__
#define __TME_H__

#define SPI_GETMOUSEHOVERWIDTH	0x0062
#define SPI_GETMOUSEHOVERHEIGHT	0x0064
#define SPI_GETMOUSEHOVERTIME	0x0066

#ifndef TME_HOVER

#define TME_HOVER       0x00000001
#define TME_LEAVE       0x00000002
#define TME_NONCLIENT   0x00000010
#define TME_QUERY       0x40000000
#define TME_CANCEL      0x80000000

#define HOVER_DEFAULT 0xFFFFFFFF

typedef struct tagTRACKMOUSEEVENT
{
    DWORD cbSize;
    DWORD dwFlags;
    HWND  hwndTrack;
    DWORD dwHoverTime;
} TRACKMOUSEEVENT, *LPTRACKMOUSEEVENT;

#endif // !TME_HOVER

BOOL WINAPI TrackMouseEvent(TRACKMOUSEEVENT* ptme);

#endif // __TME_H__
