CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := .
BUILD_DIR := build
TARGET_DIR := bin
TARGET := libgoogleapis.a

CXXFLAGS=-g -I .
LIB := -pthread -lprotobuf -lgrpc++
INC :=
SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS=$(patsubst %.cc, %.o, $(SOURCES))

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " ar rcs $(TARGET) $(OBJECTS)"; ar rcs $(TARGET) $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Building..."
	@mkdir -p $(BUILD_DIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILD_DIR) $(TARGET) $(OBJECTS)"; $(RM) -r $(BUILD_DIR) $(TARGET) $(OBJECTS)

.PHONY: clean
