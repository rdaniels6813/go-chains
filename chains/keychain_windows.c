
#define UNICODE

#include "keychain.h"
#include <windows.h>
#include <wincred.h>
#include <stdio.h>
#include <string.h>

LPWSTR utf8ToWideChar(char *utf8)
{
  int wide_char_length = MultiByteToWideChar(CP_UTF8,
                                             0,
                                             utf8,
                                             -1,
                                             NULL,
                                             0);
  if (wide_char_length == 0)
  {
    return NULL;
  }

  LPWSTR result = malloc(sizeof(WCHAR) * wide_char_length);
  if (MultiByteToWideChar(CP_UTF8,
                          0,
                          utf8,
                          -1,
                          result,
                          wide_char_length) == 0)
  {
    free(result);
    return NULL;
  }

  return result;
}

char *wideCharToAnsi(LPWSTR wide_char)
{
  if (wide_char == NULL)
  {
    return "";
  }

  int ansi_length = WideCharToMultiByte(CP_ACP,
                                        0,
                                        wide_char,
                                        -1,
                                        NULL,
                                        0,
                                        NULL,
                                        NULL);
  if (ansi_length == 0)
  {
    return "";
  }

  char *buffer = malloc(sizeof(char) * ansi_length);
  if (WideCharToMultiByte(CP_ACP,
                          0,
                          wide_char,
                          -1,
                          buffer,
                          ansi_length,
                          NULL,
                          NULL) == 0)
  {
    free(buffer);
    return "";
  }

  return buffer;
}

char *wideCharToUtf8(LPWSTR wide_char)
{
  if (wide_char == NULL)
  {
    return "";
  }

  int utf8_length = WideCharToMultiByte(CP_UTF8,
                                        0,
                                        wide_char,
                                        -1,
                                        NULL,
                                        0,
                                        NULL,
                                        NULL);
  if (utf8_length == 0)
  {
    return "";
  }

  char *buffer = malloc(sizeof(char) * utf8_length);
  if (WideCharToMultiByte(CP_UTF8,
                          0,
                          wide_char,
                          -1,
                          buffer,
                          utf8_length,
                          NULL,
                          NULL) == 0)
  {
    free(buffer);
    return "";
  }

  return buffer;
}

char *getErrorMessage(DWORD errorCode)
{
  LPWSTR errBuffer;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL, errorCode, 0, (LPWSTR)&errBuffer, 0, NULL);
  char *errMsg = wideCharToAnsi(errBuffer);
  LocalFree(errBuffer);
  return errMsg;
}

char *mallocString(char *message)
{
  char *errorPointer = (char *)malloc(strlen(message) + 1);
  sprintf(errorPointer, "%s", message);
  return errorPointer;
}

char *GetGenericPassword(char *service, char *account, int *resultCode)
{
  char *target_name_utf8 = (char *)malloc(strlen(service) + strlen(account) + 2);
  sprintf(target_name_utf8, "%s/%s", service, account);
  LPWSTR target_name = utf8ToWideChar(target_name_utf8);
  if (target_name == NULL)
  {
    *resultCode = -1;
    return mallocString("Error converting service & account to target_name");
  }

  CREDENTIAL *cred;
  boolean result = CredRead(target_name, CRED_TYPE_GENERIC, 0, &cred);
  free(target_name);
  free(target_name_utf8);
  if (!result)
  {
    DWORD code = GetLastError();
    if (code == ERROR_NOT_FOUND)
    {
      *resultCode = code;
      return getErrorMessage(code);
    }
    else
    {
      *resultCode = code;
      return getErrorMessage(code);
    }
  }
  char *password = malloc(sizeof(char) * cred->CredentialBlobSize);
  sprintf(password, "%s", (char *)cred->CredentialBlob);
  CredFree(cred);
  *resultCode = 0;
  return password;
}

char *DeleteGenericPassword(char *service, char *account, int *resultCode)
{
  char *target_name_utf8 = (char *)malloc(strlen(service) + strlen(account) + 2);
  sprintf(target_name_utf8, "%s/%s", service, account);
  LPWSTR target_name = utf8ToWideChar(target_name_utf8);
  if (target_name_utf8 == NULL)
  {
    *resultCode = -1;
    return mallocString("Error converting service & account to target_name");
  }

  boolean result = CredDelete((LPWSTR)target_name, CRED_TYPE_GENERIC, 0);
  free(target_name);
  free(target_name_utf8);
  if (!result)
  {
    DWORD code = GetLastError();
    if (code == ERROR_NOT_FOUND)
    {
      *resultCode = code;
      return getErrorMessage(code);
    }
    else
    {
      *resultCode = code;
      return getErrorMessage(code);
    }
  }

  *resultCode = 0;
  return NULL;
}

char *AddOrUpdateGenericPassword(char *service, char *account, char *password, int *resultCode)
{
  char *target_name_utf8 = (char *)malloc(strlen(service) + strlen(account) + 2));
  sprintf(target_name_utf8, "%s/%s", service, account);
  LPWSTR target_name = utf8ToWideChar(target_name_utf8);
  if (target_name == NULL)
  {
    *resultCode = -1;
    return mallocString("Error converting service & account to target_name");
  }

  LPWSTR user_name = utf8ToWideChar(account);
  if (user_name == NULL)
  {
    *resultCode = -1;
    return mallocString("account must not be null");
  }

  CREDENTIAL cred = {0};
  cred.Type = CRED_TYPE_GENERIC;
  cred.TargetName = target_name;
  cred.UserName = user_name;
  cred.CredentialBlobSize = strlen(password) + 1;
  cred.CredentialBlob = (LPBYTE)password;
  cred.Persist = CRED_PERSIST_ENTERPRISE;

  boolean result = CredWrite(&cred, 0);
  free(target_name_utf8);
  free(target_name);
  if (!result)
  {
    DWORD code = GetLastError();
    *resultCode = code;
    return getErrorMessage(code);
  }
  *resultCode = 0;
  return NULL;
}

char *AddGenericPassword(char *service, char *account, char *password, int *resultCode)
{
  return AddOrUpdateGenericPassword(service, account, password, resultCode);
}