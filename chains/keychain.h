char *GetGenericPassword(char *service, char *account, int *resultCode);
char *AddGenericPassword(char *service, char *account, char *password, int *resultCode);
char *DeleteGenericPassword(char *service, char *account, int *resultCode);
char *AddOrUpdateGenericPassword(char *service, char *account, char *password, int *resultCode);