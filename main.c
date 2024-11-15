/*
 * This program makes use of experimental header for IThemeManager2 interface.
 * Tested only the methods:
 *     ITheme:
 *     - GetDisplayName
 *
 *     IThemeManager2:
 *     - Init
 *     - GetThemeCount
 *     - GetTheme
 *     - GetCurrentTheme
 *
 *  On Windows versions:
 *     - Win10 22h2 - 10.0.19045
 *     - Win11 22h2 - 10.0.22621
 *
 *  I can't see any reason why it would not work in any version in between.
*/

#pragma comment(lib, "ole32")
#pragma comment(lib, "oleaut32")

#include <stdio.h>
#include <assert.h>
#include "ThemeManager2.h"


#define PRINTF_AND_RETURN(code, mask, ...) \
    do {                                   \
        printf(mask, __VA_ARGS__);         \
        return code;                       \
    } while (0)


const char *usage =
    "Usage: wtheme <cmd> [idx]\n"
    "\tCommands: dump: print theme names to stdout with its index\n"
    "\t          set: set current theme by index\n"
    "\n`wtheme dump` or `wtheme set <idx>`\n";


int get_theme_manager(IThemeManager2 **theme_manager)
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        PRINTF_AND_RETURN(3, "Could not initilize COM\n");

    hr = CoCreateInstance(
            &CLSID_ThemeManager2,
            NULL,
            CLSCTX_INPROC_SERVER,
            &IID_ThemeManager2,
            (void **)theme_manager
    );
    if (FAILED(hr))
        PRINTF_AND_RETURN(4, "Could not create IThemeManager2 instace. HR: %lu\n", hr);

    hr = (*theme_manager)->lpVtbl->Init(*theme_manager, ThemeInitNoFlags);
    if (FAILED(hr))
        PRINTF_AND_RETURN(5, "Could not init IThemeManager2 instace. HR: %lu\n", hr);

    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) PRINTF_AND_RETURN(1, usage);

    char *command = argv[1];
    IThemeManager2 *theme_manager = NULL;
    HRESULT hr;
    int err;

    if (strcmp(command, "dump") == 0) {
        if (argc != 2) printf("Warning: Ignoring args after `dump`\n");

        err = get_theme_manager(&theme_manager);
        if (err) return err;

        int theme_count = 0;
        hr = theme_manager->lpVtbl->GetThemeCount(theme_manager, &theme_count);
        if (FAILED(hr)) PRINTF_AND_RETURN(6, "Could not get theme count\n");

        int current_theme = -1;
        hr = theme_manager->lpVtbl->GetCurrentTheme(theme_manager, &current_theme);
        if (FAILED(hr)) printf("Could not get current theme\n");

        for (int i = 0; i < theme_count; ++i) {
            ITheme *theme = NULL;
            hr = theme_manager->lpVtbl->GetTheme(theme_manager, i, &theme);
            if (FAILED(hr)) {
                printf("Could not get theme of idx %i\n", i);
                continue;
            }

            LPWSTR name_wide = NULL;
            hr = theme->lpVtbl->GetDisplayName(theme, &name_wide);
            if (FAILED(hr)) {
                printf("Could not get display name for theme with idx %i\n", i);
                continue;
            }

            int needed = WideCharToMultiByte(CP_UTF8, 0, name_wide, -1, NULL, 0, NULL, NULL);
            char *name = malloc(needed);
            int out = WideCharToMultiByte(CP_UTF8, 0, name_wide, -1, name, needed, NULL, NULL);
            assert(out == needed);

            printf("%i: %s", i, name);
            if (i == current_theme) printf("   <current>");
            printf("\n");

            SysFreeString(name_wide);
            free(name);
            theme->lpVtbl->Release(theme);
        }

    } else if (strcmp(command, "set") == 0) {
        if (argc < 3) PRINTF_AND_RETURN(7, "No theme index is provided\n%s", usage);
        if (argc > 3) printf("Warning: Ignoring args after <idx>\n");

        char *arg = argv[2];
        int idx = atoi(arg);
        if (!idx && strcmp(arg, "0") != 0)
            PRINTF_AND_RETURN(8, "Could not decode provided index `%s`\n", arg);

        err = get_theme_manager(&theme_manager);
        if (err) return err;

        hr = theme_manager->lpVtbl->SetCurrentTheme(theme_manager, NULL, idx, 1, 0, 0);
        if (FAILED(hr)) PRINTF_AND_RETURN(9, "Could not set theme by index %i\n", idx);

    } else {
        PRINTF_AND_RETURN(2, "Unknown command `%s`.\n%s", command, usage);
    }

    theme_manager->lpVtbl->Release(theme_manager);
    CoUninitialize();
    return 0;
}
