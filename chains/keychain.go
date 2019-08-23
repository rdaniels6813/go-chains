package chains

import "github.com/rdaniels6813/go-chains/chains/os"

type Keychain interface {
	Set(key string, value string) error
	Get(key string) (string, error)
}

func NewKeychain() (Keychain, error) {
	return &os.OSKeychain{}, nil
}
