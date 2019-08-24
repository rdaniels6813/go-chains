// +build !windows darwin !linux

package chains

// #cgo LDFLAGS: -framework Security -framework CoreFoundation
// #cgo CFLAGS: -g -Wall
// #include <stdlib.h>
// #include "keychain_darwin.h"
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

	ptr := C.malloc(C.sizeof_char * 1)
	defer C.free(ptr)

	size := C.GetGenericPassword(name, name2, (*C.char)(ptr))

	b := C.GoBytes(ptr, size)
	result := string(b)
	return result, nil
}

func (k *OSKeychain) Set(service string, account string, password string) error {
	cService := C.CString(service)
	defer C.free(unsafe.Pointer(cService))

	cAccount := C.CString(account)
	defer C.free(unsafe.Pointer(cAccount))

	cPassword := C.CString(password)
	defer C.free(unsafe.Pointer(cPassword))

	result := C.AddOrUpdateGenericPassword(cService, cAccount, cPassword)
	if result != 0 {
		return fmt.Errorf("An error setting the password occurred: %d", result)
	}
	return nil
}
func (k *OSKeychain) Delete(service string, account string) error {
	cService := C.CString(service)
	defer C.free(unsafe.Pointer(cService))

	cAccount := C.CString(account)
	defer C.free(unsafe.Pointer(cAccount))

	result := C.DeleteGenericPassword(cService, cAccount)
	if result != 0 {
		return fmt.Errorf("An error deleting the password occurred: %d", result)
	}
	return nil
}
