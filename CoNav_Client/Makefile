CC = gcc
WINDRES = windres

TARGET = build/CoNav_Client.exe
SRC_DIR = src
BUILD_DIR = build
RES_DIR = res

CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lws2_32


rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCES = $(call rwildcard,$(SRC_DIR)/,*.c)
HEADERS = $(call rwildcard,$(SRC_DIR)/,*.h)

INCLUDES = $(addprefix -I,$(sort $(dir $(HEADERS))))

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
RESOURCE = $(BUILD_DIR)/CoNav_logo.res.o

client: $(TARGET)

$(TARGET): $(OBJECTS) $(RESOURCE)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(RESOURCE): $(RES_DIR)/CoNav_logo.rc $(RES_DIR)/CoNav_logo.ico
	@mkdir -p $(dir $@)
	$(WINDRES) -I $(RES_DIR) -i $< -O coff -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: client clean