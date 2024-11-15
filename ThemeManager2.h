/*
 * This thing is a C translation (mostly copy-paste) from 
 * https://github.com/Spiritreader/SecureUxTheme/blob/b99225316ee125d391e717c13f5a49d0abbdf8f5/ThemeTool/interfaces.h
 *
 * It is a reverse-engineered COM interface for IThemeManager2, which is undocumented
 * and makes this header experimental. I only tested to the extension of the use
 * in main.c, there will be the Windows versions tested.
*/

#include <combaseapi.h>


// TODO: should be unsigned, test with ThemeInitCurrentThemeOnly
typedef enum {
    ThemeInitNoFlags            = 0,
    ThemeInitCurrentThemeOnly   = 1 << 0,
    ThemeInitFlagUnk1           = 1 << 1,
    ThemeInitFlagUnk2           = 1 << 2,
} THEME_MANAGER_INITIALIZATION_FLAGS;

typedef enum {
    THEME_APPLY_FLAG_IGNORE_BACKGROUND    = 1 << 0,
    THEME_APPLY_FLAG_IGNORE_CURSOR        = 1 << 1,
    THEME_APPLY_FLAG_IGNORE_DESKTOP_ICONS = 1 << 2,
    THEME_APPLY_FLAG_IGNORE_COLOR         = 1 << 3,
    THEME_APPLY_FLAG_IGNORE_SOUND         = 1 << 4,
    THEME_APPLY_FLAG_IGNORE_SCREENSAVER   = 1 << 5,
    THEME_APPLY_FLAG_UNKNOWN              = 1 << 6, // something about window metrics
    THEME_APPLY_FLAG_UNKNOWN2             = 1 << 7,
    THEME_APPLY_FLAG_NO_HOURGLASS         = 1 << 8
} THEME_APPLY_FLAGS;

typedef enum {
    THEMEPACK_FLAG_UNKNOWN1     = 1 << 0, // setting this seems to supress hourglass
    THEMEPACK_FLAG_UNKNOWN2     = 1 << 1, // setting this seems to supress hourglass
    THEMEPACK_FLAG_SILENT       = 1 << 2, // hides all dialogs and prevents sound
    THEMEPACK_FLAG_ROAMED       = 1 << 3, // something about roaming
} THEMEPACK_FLAGS;

typedef interface ITheme ITheme;
typedef interface IThemeManager2 IThemeManager2;

#undef  INTERFACE
#define INTERFACE ITheme 

DECLARE_INTERFACE_(ITheme, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObject);
    STDMETHOD_(ULONG, AddRef)(THIS);
    STDMETHOD_(ULONG, Release)(THIS);

    /* ITheme methods */
    STDMETHOD_(HRESULT, GetDisplayName)(THIS_ LPWSTR* name);
    STDMETHOD_(HRESULT, PutDisplayName)(THIS_ LPWSTR name);

    /* More methods but they are different in Windows versions */

    END_INTERFACE
};


#undef  INTERFACE
#define INTERFACE IThemeManager2 

DECLARE_INTERFACE_(IThemeManager2, IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObject);
    STDMETHOD_(ULONG, AddRef)(THIS);
    STDMETHOD_(ULONG, Release)(THIS);

    /* IThemeManager2 methods */
    STDMETHOD_(HRESULT, Init)           (THIS_ THEME_MANAGER_INITIALIZATION_FLAGS flags);
    STDMETHOD_(HRESULT, InitAsync)      (THIS_ HWND hwnd, int flag);
    STDMETHOD_(HRESULT, Refresh)        (THIS);
    STDMETHOD_(HRESULT, RefreshAsync)   (THIS_ HWND hwnd, int flag);
    STDMETHOD_(HRESULT, RefreshComplete)(THIS);
    STDMETHOD_(HRESULT, GetThemeCount)  (THIS_ int* dest);
    STDMETHOD_(HRESULT, GetTheme)       (THIS_ int idx, ITheme** theme);
    STDMETHOD_(HRESULT, IsThemeDisabled)(THIS_ int idx, int* out);
    STDMETHOD_(HRESULT, GetCurrentTheme)(THIS_ int* out);
    STDMETHOD_(HRESULT, SetCurrentTheme)(
        THIS_
        HWND parent,
        int theme_idx,
        int apply_now_not_only_registry,  // 1 when called in Windows
        THEME_APPLY_FLAGS apply_flags,    // 0 when called in Windows
        THEMEPACK_FLAGS pack_flags        // 0 when called in Windows
    );
    STDMETHOD_(HRESULT, GetCustomTheme)               (THIS_ int* idx_maybe);
    STDMETHOD_(HRESULT, GetDefaultTheme)              (THIS_ int* idx_maybe);
    STDMETHOD_(HRESULT, CreateThemePack)              (THIS_ HWND hwnd, LPCWSTR name_maybe, THEMEPACK_FLAGS pack_flags);
    STDMETHOD_(HRESULT, CloneAndSetCurrentTheme)      (THIS_ HWND hwnd, LPCWSTR name_maybe, LPWSTR* unknown);
    STDMETHOD_(HRESULT, InstallThemePack)(
            THIS_ HWND, LPCWSTR name_maybe, int idx_maybe, THEMEPACK_FLAGS pack_flags, LPWSTR* unknow, ITheme** theme
    );
    STDMETHOD_(HRESULT, DeleteTheme)                  (THIS_ LPCWSTR name_maybe);
    STDMETHOD_(HRESULT, OpenTheme)                    (THIS_ HWND hwnd, LPCWSTR name_maybe, THEMEPACK_FLAGS pack_flags);
    STDMETHOD_(HRESULT, AddAndSelectTheme)            (THIS_ HWND hwnd, LPCWSTR name_maybe, THEME_APPLY_FLAGS apply_flags, THEMEPACK_FLAGS pack_flags);
    STDMETHOD_(HRESULT, SQMCurrentTheme)              (THIS);
    STDMETHOD_(HRESULT, ExportRoamingThemeToStream)   (THIS_ IStream* stream, int idx_maybe);
    STDMETHOD_(HRESULT, ImportRoamingThemeFromStream) (THIS_ IStream* stream, int idx_maybe);
    STDMETHOD_(HRESULT, UpdateColorSettingsForLogonUI)(THIS);
    STDMETHOD_(HRESULT, GetDefaultThemeId)            (THIS_ GUID* guid);
    STDMETHOD_(HRESULT, UpdateCustomTheme)            (THIS);

    END_INTERFACE
};

const GUID CLSID_ThemeManager2 = {
    0x9324da94, 0x50ec, 0x4a14, { 0xa7, 0x70, 0xe9, 0x0c, 0xa0, 0x3e, 0x7c, 0x8f }
};
const GUID IID_ThemeManager2 = {
    0xc1e8c83e, 0x845d, 0x4d95, { 0x81, 0xdb, 0xe2, 0x83, 0xfd, 0xff, 0xc0, 0x00 }
};
