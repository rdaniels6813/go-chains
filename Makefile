golangci_lint_version=1.17.1

build:
	go build ./chains/

test:
	go test -coverprofile=coverage.txt -covermode=atomic ./...

lint:
	# Install golangci-lint if it's not already installed
	command -v golangci-lint || curl -sfL https://install.goreleaser.com/github.com/golangci/golangci-lint.sh | sh -s -- -b `go env GOPATH`/bin v$(golangci_lint_version)
	golangci-lint run

release:
	semantic-release