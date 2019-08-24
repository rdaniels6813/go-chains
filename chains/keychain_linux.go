// +build !windows !darwin linux

package chains

import "fmt"

type OSKeychain struct {
}

func (k *OSKeychain) Get(key string,account string) (string, error) {
	return "", fmt.Errorf("Linux keychain not implemented")
}
func (k *OSKeychain) Set(key string, value string) error {
	return fmt.Errorf("Linux keychain not implemented")
}
