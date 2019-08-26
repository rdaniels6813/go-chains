// +build !windows !darwin linux

package chains

// #cgo CFLAGS: -g -Wall
// #cgo pkg-config: libsecret-1
// #include <stdlib.h>
// #include "keychain.h"
// #include <string.h>
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

	passwordPointer := C.malloc(C.sizeof_char * 5)
	defer C.free(passwordPointer)

	var sizePointer C.int

	passwordOrErrorMessage := C.GetGenericPassword(name, name2, &sizePointer)
	defer C.free(unsafe.Pointer(passwordOrErrorMessage))

	result := C.GoString(passwordOrErrorMessage)
	if sizePointer != 0 {
		return "", fmt.Errorf("Failed to get password: %s", result)
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

	var sizePointer C.int

	errorMessage := C.AddOrUpdateGenericPassword(cService, cAccount, cPassword, &sizePointer)
	defer C.free(unsafe.Pointer(errorMessage))
	if sizePointer != 0 {
		result := C.GoString(errorMessage)
		return fmt.Errorf("Failed to set password: %s", result)
	}
	return nil
}
func (k *OSKeychain) Delete(service string, account string) error {
	cService := C.CString(service)
	defer C.free(unsafe.Pointer(cService))

	cAccount := C.CString(account)
	defer C.free(unsafe.Pointer(cAccount))

	var sizePointer C.int

	errorMessage := C.DeleteGenericPassword(cService, cAccount, &sizePointer)
	defer C.free(unsafe.Pointer(errorMessage))
	if sizePointer != 0 {
		result := C.GoString(errorMessage)
		return fmt.Errorf("An error deleting the password occurred: %s", result)
	}
	return nil
}
