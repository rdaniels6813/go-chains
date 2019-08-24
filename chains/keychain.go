package chains

// Keychain keychain interface which uses OS native keychain
type Keychain interface {
	Set(service string, account string, password string) error
	Get(service string, account string) (string, error)
	Delete(service string, account string) error
}

// NewKeychain creates a new API for interacting with the OS's keychain
func NewKeychain() (Keychain, error) {
	return &OSKeychain{}, nil
}
