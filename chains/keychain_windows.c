
#define UNICODE

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

int GetGenericPassword(char *service, char *account, char *out)
{
  char *target_name_c = (char *)malloc(sizeof(char) * (strlen(service) + strlen(account) + 1));
  sprintf(target_name_c, "%s/%s", service, account);
  LPWSTR target_name = utf8ToWideChar(target_name_c);
  if (target_name == NULL)
  {
    return -1;
  }

  CREDENTIAL *cred;
  boolean result = CredRead(target_name, CRED_TYPE_GENERIC, 0, &cred);
  free(target_name);
  free(target_name_c);
  if (!result)
  {
    DWORD code = GetLastError();
    if (code == ERROR_NOT_FOUND)
    {
      return -2;
    }
    else
    {
      return -3;
    }
  }

  sprintf(out, "%s", (char *)cred->CredentialBlob);
  int length = cred->CredentialBlobSize;
  CredFree(cred);
  return length;
}

int DeleteGenericPassword(char *service, char *account)
{
  char *target_name_c = (char *)malloc(strlen(service) + strlen(account) + 1);
  sprintf(target_name_c, "%s/%s", service, account);
  LPWSTR target_name = utf8ToWideChar(target_name_c);
  if (target_name_c == NULL)
  {
    return -1;
  }

  boolean result = CredDelete((LPWSTR)target_name, CRED_TYPE_GENERIC, 0);
  free(target_name);
  free(target_name_c);
  if (!result)
  {
    DWORD code = GetLastError();
    if (code == ERROR_NOT_FOUND)
    {
      return code;
    }
    else
    {
      return code;
    }
  }

  return 0;
}

int AddOrUpdateGenericPassword(char *service, char *account, char *password)
{
  char *target_name_c = (char *)malloc(sizeof(char) * (strlen(service) + strlen(account) + 1));
  sprintf(target_name_c, "%s/%s", service, account);
  LPWSTR target_name = utf8ToWideChar(target_name_c);
  if (target_name == NULL)
  {
    return -1;
  }

  LPWSTR user_name = utf8ToWideChar(account);
  if (user_name == NULL)
  {
    return -1;
  }

  CREDENTIAL cred = {0};
  cred.Type = CRED_TYPE_GENERIC;
  cred.TargetName = target_name;
  cred.UserName = user_name;
  cred.CredentialBlobSize = strlen(password);
  cred.CredentialBlob = (LPBYTE)password;
  cred.Persist = CRED_PERSIST_ENTERPRISE;

  boolean result = CredWrite(&cred, 0);
  free(target_name_c);
  free(target_name);
  if (!result)
  {
    return -2;
  }
  else
  {
    return 0;
  }
}

int AddGenericPassword(char *service, char *account, char *password)
{
  return AddOrUpdateGenericPassword(service, account, password);
}