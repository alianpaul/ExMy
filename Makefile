EXMY := include/exmy.hpp

CC      := g++
CCFLAGS := --std=c++11

INCLUDES := -I./include
LIBS     :=

TEST_SRCS := $(wildcard test/*.cpp)
TEST_OBJS := $(patsubst test/%.cpp, bin/%, $(TEST_SRCS))
TEST_DIR  := bin

tests: $(TEST_DIR) $(TEST_OBJS)
	@echo "Done"

bin/% : test/%.cpp $(EXMY)
	@echo $@ "..." $^
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(TEST_DIR):
	mkdir -p $(TEST_DIR)

clean:
	rm -rf $(TEST_DIR)
