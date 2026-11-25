CC = gcc

TARGET = build/CoNav_Client.exe
SRC_DIR = src
BUILD_DIR = build

CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lws2_32

# Recursively discover files.
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCES = $(call rwildcard,$(SRC_DIR)/,*.c)
HEADERS = $(call rwildcard,$(SRC_DIR)/,*.h)

# Include every directory containing headers.
INCLUDES = $(addprefix -I,$(sort $(dir $(HEADERS))))

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

client: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: client clean