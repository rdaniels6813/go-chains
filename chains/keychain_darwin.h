int GetGenericPassword(char *service, char *account, char *out);
int AddGenericPassword(char *service, char *account, char *password);
int DeleteGenericPassword(char *service, char *account);
int AddOrUpdateGenericPassword(char *service, char *account, char *password);