// +build windows !darwin !linux

package chains

// #cgo CFLAGS: -g -Wall
// #include <stdlib.h>
// #include "keychain.h"
import "C"
import "fmt"
import "unsafe"

type OSKeychain struct {
}

func (k *OSKeychain) Get(service string, account string) (string, error) {
	name := C.CString(service)
	defer C.free(unsafe.Pointer(name))

	name2 := C.CString(account)
	defer C.free(unsafe.Pointer(name2))

	var resultCode C.int

	passwordOrErrorMessage := C.GetGenericPassword(name, name2, &resultCode)
	defer C.free(unsafe.Pointer(passwordOrErrorMessage))
	result := C.GoString(passwordOrErrorMessage)
	if resultCode != 0 {
		return "", fmt.Errorf("Keychain Get error: %s", result)
	}
	return result, nil
}

func (k *OSKeychain) Set(service string, account string, password string) error {
	cService := C.CString(service)
	defer C.free(unsafe.Pointer(cService))

	cAccount := C.CString(account)
	defer C.free(unsafe.Pointer(cAccount))

	cPassword := C.CString(password)
	defer C.free(unsafe.Pointer(cPassword))

	var resultCode C.int

	resultMessage := C.AddOrUpdateGenericPassword(cService, cAccount, cPassword, &resultCode)
	defer C.free(unsafe.Pointer(resultMessage))
	if resultCode != 0 {
		return fmt.Errorf("Keychain Set error: %s", C.GoString(resultMessage))
	}
	return nil
}
func (k *OSKeychain) Delete(service string, account string) error {
	cService := C.CString(service)
	defer C.free(unsafe.Pointer(cService))

	cAccount := C.CString(account)
	defer C.free(unsafe.Pointer(cAccount))

	var resultCode C.int

	resultMessage := C.DeleteGenericPassword(cService, cAccount, &resultCode)
	defer C.free(unsafe.Pointer(resultMessage))
	if resultCode != 0 {
		return fmt.Errorf("Keychain Delete error: %s", C.GoString(resultMessage))
	}
	return nil
}
