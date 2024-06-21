#include <Windows.h>
#include <stdio.h>

int c_get_appearence()
{
  BOOL isDarkMode = FALSE;
  DWORD value;
  DWORD dataSize = sizeof(DWORD);
  LONG result = RegGetValueA(HKEY_CURRENT_USER,
                             "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                             "AppsUseLightTheme", RRF_RT_REG_DWORD, NULL, &value, &dataSize);
  if (ERROR_SUCCESS == result) {
    isDarkMode = value == 0;
  } else
    printf("error getting get_appearance");

  if (isDarkMode) {
    printf("is dark mode\n");
  } else {
    printf("is light mode\n");
  }

  return isDarkMode;
}
