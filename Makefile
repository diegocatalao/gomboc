# Makefile for multiplatform Golang build

# Binary name
BINARY_NAME=gomboc

# Build directory
BUILD_DIR=build
BIN_DIR=$(BUILD_DIR)/bin
RELEASE_DIR=$(BUILD_DIR)/release

# Version
VERSION=1.0.0

# Target platforms
PLATFORMS=darwin linux windows

WINDOWS_ARCHS=386 amd64
DARWIN_ARCHS=amd64 arm64
LINUX_ARCHS=386 amd64 arm arm64 ppc64 ppc64le mips mipsle mips64 mips64le

WINDOWS_EXT=.exe
DARWIN_EXT=.app
LINUX_EXT=

# Setup linker flags option for build that interoperate with variable names in your Go code
LDFLAGS=-ldflags "-X main.Version=$(VERSION)"

# Default target
all: clean build

# Builds the binary for all target platforms
build: $(PLATFORMS)

# Targets for each platform
$(PLATFORMS):
	@$(eval EXT=$($(shell echo $@ | tr '[:lower:]' '[:upper:]')_EXT))
	@$(eval ARCHS=$($(shell echo $@ | tr '[:lower:]' '[:upper:]')_ARCHS))
	@$(MAKE) --no-print-directory EXT=$(EXT) GOOS=$@ ARCHITECTURES="$(ARCHS)" build-target

build-target:
	@$(foreach arch,$(ARCHITECTURES), \
		echo "Building $(BINARY_NAME)-$(GOOS)-$(VERSION)-$(arch)"; \
		GOOS=$(GOOS) GOARCH=$(arch) go build $(LDFLAGS) -o \
		'$(BIN_DIR)/$(BINARY_NAME)-$(GOOS)-$(VERSION)-$(arch)$(EXT)'; \
	)

compress:
	$(shell mkdir -p $(RELEASE_DIR))
	@$(foreach platform,$(PLATFORMS), \
		zip -r $(RELEASE_DIR)/$(BINARY_NAME)-$(VERSION)-$(platform).zip $(BIN_DIR)/$(BINARY_NAME)-$(platform)-$(VERSION)*; \
	)

# Cleans up the build directory
clean:
	@rm -rf $(BUILD_DIR)/

# Phony targets for make
.PHONY: all build clean $(PLATFORMS) build-target
