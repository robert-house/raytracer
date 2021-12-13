EXE       = runic
LIBS      = `sdl2-config --libs -L/opt/homebrew/lib`

CC       = c++
CXXFLAGS = -O3 -std=c++17 -Wall
LDFLAGS  = -pthread -lSDL2

MODULES   = ./ materials renderer scene shapes includes logging config profiling
SRC_DIR   = $(addprefix src/, $(MODULES))
BUILD_DIR = $(addprefix object/, $(MODULES))

SRC       = $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.cpp))
OBJ       = $(patsubst src/%.cpp, object/%.o, $(SRC))
INCLUDES  = $(addprefix -I,$(SRC_DIR)) -I/opt/homebrew/include

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs bin/$(EXE)

bin/$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir, $(BUILD_DIR), $(eval $(call make-goal, $(bdir))))
