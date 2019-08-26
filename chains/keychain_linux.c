#include "keychain.h"
#define SECRET_API_SUBJECT_TO_CHANGE
#include <libsecret/secret.h>
#include <stdio.h>
#include <string.h>

static const SecretSchema schema = {
    "org.freedesktop.Secret.Generic", SECRET_SCHEMA_NONE, {{"service", SECRET_SCHEMA_ATTRIBUTE_STRING}, {"account", SECRET_SCHEMA_ATTRIBUTE_STRING}}};

char *mallocString(const char *message)
{
  char *errorPointer = (char *)malloc(sizeof(char) * (strlen(message) + 1));
  sprintf(errorPointer, "%s", message);
  return errorPointer;
}

char *GetGenericPassword(char *service, char *account, int *resultCode)
{
  GError *error = NULL;

  gchar *raw_password = secret_password_lookup_sync(
      &schema,
      NULL,
      &error,
      "service",
      service,
      "account",
      account,
      NULL);

  if (error != NULL)
  {
    char *errorMessage = mallocString(error->message);
    g_error_free(error);
    *resultCode = -1;
    return errorMessage;
  }

  if (raw_password == NULL)
  {
    *resultCode = -1;
    return mallocString("Null credential returned");
  }

  char *password = mallocString(raw_password);
  secret_password_free(raw_password);
  *resultCode = 0;
  return password;
}

char *DeleteGenericPassword(char *service, char *account, int *resultCode)
{
  GError *error = NULL;

  gboolean result = secret_password_clear_sync(
      &schema,
      NULL,
      &error,
      "service",
      service,
      "account",
      account,
      NULL);
  if (error != NULL)
  {
    char *errorMessage = mallocString(error->message);
    g_error_free(error);
    *resultCode = -1;
    return errorMessage;
  }

  if (!result)
  {
    *resultCode = -1;
    return mallocString("Failed to delete credential with no error returned");
  }

  *resultCode = 0;
  return NULL;
}

char *AddOrUpdateGenericPassword(char *service, char *account, char *password, int *resultCode)
{
  GError *error = NULL;

  char *label = malloc(strlen(service) + strlen(account) + 2);

  sprintf(label, "%s/%s", service, account);

  secret_password_store_sync(
      &schema,
      SECRET_COLLECTION_DEFAULT,
      label,
      password,
      NULL,
      &error,
      "service", service,
      "account", account,
      NULL);
  free(label);
  if (error != NULL)
  {
    char *errorMessage = mallocString(error->message);
    g_error_free(error);
    *resultCode = -1;
    return errorMessage;
  }

  *resultCode = 0;
  return NULL;
}

char *AddGenericPassword(char *service, char *account, char *password, int *resultCode)
{
  return AddOrUpdateGenericPassword(service, account, password, resultCode);
}