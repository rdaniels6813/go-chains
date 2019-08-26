#include "keychain.h"
#include <Security/Security.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <string.h>

char *mallocString(const char *message)
{
  char *errorPointer = (char *)malloc(sizeof(char) * (strlen(message) + 1));
  sprintf(errorPointer, "%s", message);
  return errorPointer;
}

char *errorStatusToString(OSStatus status)
{
  char *errorStr;
  CFStringRef errorMessageString = SecCopyErrorMessageString(status, NULL);

  const char *errorCStringPtr = CFStringGetCStringPtr(errorMessageString,
                                                      kCFStringEncodingUTF8);
  if (errorCStringPtr)
  {
    errorStr = mallocString(errorCStringPtr);
  }
  else
  {
    errorStr = mallocString("An unknown error occurred.");
  }

  CFRelease(errorMessageString);
  return errorStr;
}

char *GetGenericPassword(char *service, char *account, int *resultCode)
{
  void *data;
  UInt32 length;
  OSStatus status = SecKeychainFindGenericPassword(
      NULL, strlen(service), service, strlen(account), account, &length, &data,
      NULL);

  if (status == errSecItemNotFound)
  {
    *resultCode = status;
    return errorStatusToString(status);
  }
  else if (status != errSecSuccess)
  {
    *resultCode = status;
    return errorStatusToString(status);
  }
  char *password = malloc(sizeof(char) * length);
  sprintf(password, "%s", ((const char *)data));
  SecKeychainItemFreeContent(NULL, data);
  *resultCode = length;
  return password;
}

char *AddGenericPassword(char *service, char *account, char *password, int *resultCode)
{
  OSStatus status = SecKeychainAddGenericPassword(
      NULL, strlen(service), service, strlen(account), account,
      strlen(password), password, NULL);

  if (status == errSecDuplicateItem)
  {
    *resultCode = status;
    return errorStatusToString(status);
  }
  else if (status != errSecSuccess)
  {
    *resultCode = status;
    return errorStatusToString(status);
  }

  *resultCode = errSecSuccess;
  return NULL;
}

char *DeleteGenericPassword(char *service, char *account, int *resultCode)
{
  SecKeychainItemRef item;
  OSStatus status = SecKeychainFindGenericPassword(NULL, strlen(service),
                                                   service, strlen(account),
                                                   account, NULL, NULL, &item);
  if (status == errSecItemNotFound)
  {
    // Item could not be found, so already deleted.
    *resultCode = status;
    return errorStatusToString(status);
  }
  else if (status != errSecSuccess)
  {
    *resultCode = status;
    return errorStatusToString(status);
  }

  status = SecKeychainItemDelete(item);
  CFRelease(item);
  if (status != errSecSuccess)
  {
    *resultCode = status;
    return errorStatusToString(status);
  }

  *resultCode = errSecSuccess;
  return NULL;
}

char *AddOrUpdateGenericPassword(char *service, char *account, char *password, int *resultCode)
{
  char *result = AddGenericPassword(service, account, password, resultCode);
  if (*resultCode == errSecDuplicateItem)
  {
    // This password already exists, delete it and try again.
    char *delResult = DeleteGenericPassword(service, account, resultCode);
    if (*resultCode != errSecSuccess)
    {
      return delResult;
    }
    else
      return AddGenericPassword(service, account, password, resultCode);
  }
  else if (*resultCode != errSecSuccess)
  {
    return result;
  }

  *resultCode = errSecSuccess;
  return NULL;
}