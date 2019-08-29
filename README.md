# go-chains

[![Build Status](https://travis-ci.org/rdaniels6813/go-chains.svg?branch=master)](https://travis-ci.org/rdaniels6813/go-chains)
[![Appveyor](https://ci.appveyor.com/api/projects/status/github/rdaniels6813/go-chains?svg=true)](https://ci.appveyor.com/project/RyanDaniels/go-chains)
[![codecov](https://codecov.io/gh/rdaniels6813/go-chains/branch/master/graph/badge.svg)](https://codecov.io/gh/rdaniels6813/go-chains)

## Install

```sh
go get github.com/rdaniels6813/go-chains
```

## Usage

- Creating the keychain interface object

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

- Getting a password

  ```golang
  password, err := keychain.Get("account-name", "username")
  ```

- Setting a password

  ```golang
  err := keychain.Set("account-name", "username", "super-secret-password")
  ```

- Deleting a password
  ```golang
  err := keychain.Delete("account-name", "username")
  ```

## How does it work?

Using the cgo, and code written in c. Those parts are abstracted behind a golang API and separate OS-dependent implementations are used to make the calls to the keychain.

## OS Support

- Windows 10 (Credential Manager)
- macOS 10.14 Mojave (Keychain)
- Ubuntu 18 (libsecret)
  - `apt-get install libsecret-1-dev` to build an app using this library

NOTE: There may be further OS support for other versions of macOS, Windows, and Linux but I have verified the three above are working.

LINUX NOTE: Other variations of linux may work as well, but I have not tested them. Any linux OS with libsecret installed should work though.
