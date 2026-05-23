include ../../config.mk

# Config Arguments
TARGET_NAME := list
# Test Unit Source Directory
TU_SRC_DIR := src
# Source files which need to be compiled
SRCS += main.c ds.c
# Instantiator source files (include template headers)
INST_SRCS := int.c float.c double.c char.c unsigned_int.c long.c point.c
# Template files (auto-discovered)
TEMPLATES := $(wildcard $(TU_SRC_DIR)/template/*.temp.h)

# Auto Expansions (update not required)
TARGET    := $(BIN_DIR)/$(TARGET_NAME)
ALL_SRCS  := $(SRCS) $(INST_SRCS)
SRCS_PATH  = $(addprefix $(TU_SRC_DIR)/, $(ALL_SRCS))
OBJS      := $(SRCS_PATH:%.c=$(OBJ_DIR)/$(TARGET_NAME)/%.o) $(PREREQ_OBJS)

# Instantiator .o files depend on template files
INST_OBJS := $(addprefix $(OBJ_DIR)/$(TARGET_NAME)/$(TU_SRC_DIR)/, $(INST_SRCS:%.c=%.o))
$(INST_OBJS): $(TEMPLATES)
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OBJS) -o $@

$(OBJ_DIR)/$(TARGET_NAME)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/$(TARGET_NAME) $(TARGET)
