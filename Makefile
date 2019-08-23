binary_name=cli-manager
golangci_lint_version=1.17.1

build:
	GOOS=darwin GOARCH=amd64 go build -o bin/$(binary_name)-darwin-amd64 chains/keychain.go
	GOOS=windows GOARCH=amd64 go build -o bin/$(binary_name)-windows-amd64.exe chains/keychain.go
	GOOS=linux GOARCH=amd64 go build -o bin/$(binary_name)-linux-amd64 chains/keychain.go

test:
	go test -coverprofile=coverage.txt -covermode=atomic ./...

lint:
	# Install golangci-lint if it's not already installed
	command -v golangci-lint || curl -sfL https://install.goreleaser.com/github.com/golangci/golangci-lint.sh | sh -s -- -b `go env GOPATH`/bin v$(golangci_lint_version)
	golangci-lint run

release:
	semantic-release