#include <Security/Security.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <string.h>

int GetGenericPassword(char *service, char *account, char *out)
{
  void *data;
  UInt32 length;
  OSStatus status = SecKeychainFindGenericPassword(
      NULL, strlen(service), service, strlen(account), account, &length, &data,
      NULL);

  if (status == errSecItemNotFound)
  {
    return status;
  }
  else if (status != errSecSuccess)
  {
    return status;
  }
  sprintf(out, "%s", ((const char *)data));
  SecKeychainItemFreeContent(NULL, data);
  return length;
}

int AddGenericPassword(char *service, char *account, char *password)
{
  OSStatus status = SecKeychainAddGenericPassword(
      NULL, strlen(service), service, strlen(account), account,
      strlen(password), password, NULL);

  if (status == errSecDuplicateItem)
  {
    return status;
  }
  else if (status != errSecSuccess)
  {
    return status;
  }

  return errSecSuccess;
}

int DeleteGenericPassword(char *service, char *account)
{
  SecKeychainItemRef item;
  OSStatus status = SecKeychainFindGenericPassword(NULL, strlen(service),
                                                   service, strlen(account),
                                                   account, NULL, NULL, &item);
  if (status == errSecItemNotFound)
  {
    // Item could not be found, so already deleted.
    return status;
  }
  else if (status != errSecSuccess)
  {
    return status;
  }

  status = SecKeychainItemDelete(item);
  CFRelease(item);
  if (status != errSecSuccess)
  {
    return status;
  }

  return errSecSuccess;
}

int AddOrUpdateGenericPassword(char *service, char *account, char *password)
{
  OSStatus result = AddGenericPassword(service, account, password);
  if (result == errSecDuplicateItem)
  {
    // This password already exists, delete it and try again.
    OSStatus delResult = DeleteGenericPassword(service, account);
    if (delResult != errSecSuccess)
      return delResult;
    else
      return AddGenericPassword(service, account, password);
  }
  else if (result != errSecSuccess)
  {
    return result;
  }

  return errSecSuccess;
}