DOCKER_IMAGE ?= ghcr.io/skylord123/docker-coredevices-pebble-tool:latest
DOCKER_RUN = docker run --rm \
	-v "$(CURDIR)":/pebble \
	--user "$(shell id -u):$(shell id -g)" \
	-e HOME=/tmp \
	$(DOCKER_IMAGE)

PLATFORMS = aplite basalt chalk diorite flint emery gabbro
BUILT_PLATFORMS = aplite basalt chalk diorite flint emery

.PHONY: docker-build docker-clean docker-verify docker-sdk-list

docker-build:
	$(DOCKER_RUN) pebble build

docker-clean:
	$(DOCKER_RUN) pebble clean

docker-sdk-list:
	$(DOCKER_RUN) pebble sdk list

# Verify all platforms present in the current SDK build, and note gabbro if absent.
docker-verify: docker-build
	@test -f build/pebble.pbw
	@for p in $(BUILT_PLATFORMS); do \
		test -f build/$$p/pebble-app.elf || (echo "Missing build/$$p/pebble-app.elf" && exit 1); \
		unzip -l build/pebble.pbw | grep -q "$$p/pebble-app.bin" || (echo "Missing $$p in pbw" && exit 1); \
	done
	@if unzip -l build/pebble.pbw | grep -q gabbro/pebble-app.bin; then \
		echo "gabbro: included in pbw"; \
	else \
		echo "gabbro: not in pbw (SDK 4.9.127+ required; chalk build scales on Round 2 firmware)"; \
	fi
	@echo "Build verification passed for $(BUILT_PLATFORMS)"
