# ======================
# Build directories
# ======================
BUILD_DIR := build

# ======================
# Default target
# ======================
all: wasm

# ======================
# Configure with CMake + Emscripten
# ======================
configure:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && emcmake cmake ..

# ======================
# Build WASM
# ======================
wasm: configure
	cd $(BUILD_DIR) && cmake --build . --config Release --verbose

# ======================
# Serve the output
# ======================
serve:
	python -m http.server

# ======================
# Clean build artifacts
# ======================
clean:
	rm -rf $(BUILD_DIR)/CMakeFiles \
	rm -f $(BUILD_DIR)/*.wasm \
	rm -f $(BUILD_DIR)/*.js \
	rm -f $(BUILD_DIR)/*.o


