include .env

.DEFAULT_GOAL := help
.PHONY: help
help:
	@grep -E '^[a-zA-Z-]+:.*?## .*$$' Makefile | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "[32m%-25s[0m %s\n", $$1, $$2}'

compile: ## Only build sketch
	@arduino-cli compile --fqbn esp8266:esp8266:generic --build-path $$PWD/build --output $$PWD/build/build.bin .

upload: ## Only upload sketch
	@arduino-cli upload --fqbn esp8266:esp8266:generic -p ${UPLOAD_PORT} -i $$PWD/build/

compile-and-upload: compile upload ## Compile and upload sketch
