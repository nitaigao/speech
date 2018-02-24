CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILD_DIR := build
TARGET_DIR := bin
TARGET := $(TARGET_DIR)/speech

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
STATIC_LIB := ./lib/snowboy/lib/libsnowboy-detect.a lib/portaudio/install/lib/libportaudio.a
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILD_DIR)/%,$(SOURCES:.$(SRCEXT)=.o)) $(STATIC_LIB)
CFLAGS := -g -Wall -std=c++11 -msse -msse2
LIB := -pthread -lportaudio -lprotobuf -lgrpc++ -lcblas -lasound -L./lib/libfvad/src/.libs -lfvad
INC := -I include -I ./lib/snowboy/include -I lib/portaudio/install/include -I lib/libfvad/include -Ilib/googleapis
COPY_FILES = common.res snowboy.umdl

all: $(TARGET) $(COPY_FILES)

common.res:
	cp resources/common.res $(TARGET_DIR)/common.res
snowboy.umdl:
	cp resources/snowboy.umdl $(TARGET_DIR)/snowboy.umdl

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILD_DIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(basename $@)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILD_DIR) $(TARGET)"; $(RM) -r $(BUILD_DIR) $(TARGET)

.PHONY: clean
