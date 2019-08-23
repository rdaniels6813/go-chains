// +build !windows !darwin linux

package os

type OSKeychain struct {
}

func (k *OSKeychain) Get(key string) (string, error) {
	return "", nil
}
func (k *OSKeychain) Set(key string, value string) error {
	return nil
}
