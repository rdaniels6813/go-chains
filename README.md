# go-chains

## Install

```sh
go get github.com/rdaniels6813/go-chains
```

## Usage

* Creating the keychain interface object

    ```golang
    import (
        "fmt"

        "github.com/rdaniels6813/go-chains"
    )

    func main() {
        keychain, err := chains.NewKeyChain()
        if err != nil {
            fmt.Printf("An error occurred creating the keychain wrapper: %s", err)
        }
    }
    ```

* Getting a password
    ```golang
    password, err := keychain.Get("account-name", "username")
    ```

* Setting a password    
    ```golang
    err := keychain.Set("account-name", "username", "super-secret-password")
    ```

* Deleting a password
    ```golang
    err := keychain.Delete("account-name", "username")
    ```


## How does it work?

Using the cgo, and code written in c.  Those parts are abstracted behind a golang API and separate OS-dependent implementations are used to make the calls to the keychain.

## OS Support
* Windows 10 (Credential Manager)
* macOS 10.14 Mojave (Keychain)

NOTE: There may be further OS support for other versions of macOS & Windows, but I have verified the two above are working.