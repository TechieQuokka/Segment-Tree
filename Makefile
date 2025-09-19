CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
DEBUG_FLAGS = -g -DDEBUG -O0
SRCDIR = src
INCDIR = include
TESTDIR = tests
EXAMPLEDIR = examples
OBJDIR = obj
LIBDIR = lib
BINDIR = bin

TARGET_LIB = $(LIBDIR)/libsegtree.a
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
HEADERS = $(wildcard $(INCDIR)/*.h)

TEST_SOURCES = $(wildcard $(TESTDIR)/*.c)
TEST_TARGETS = $(TEST_SOURCES:$(TESTDIR)/%.c=$(BINDIR)/%)

EXAMPLE_SOURCES = $(wildcard $(EXAMPLEDIR)/*.c)
EXAMPLE_TARGETS = $(EXAMPLE_SOURCES:$(EXAMPLEDIR)/%.c=$(BINDIR)/%)

.PHONY: all clean lib tests examples install uninstall debug release help

all: lib tests examples

lib: $(TARGET_LIB)

tests: lib $(TEST_TARGETS)

examples: lib $(EXAMPLE_TARGETS)

$(TARGET_LIB): $(OBJECTS) | $(LIBDIR)
	ar rcs $@ $^
	ranlib $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(BINDIR)/test_%: $(TESTDIR)/test_%.c $(TARGET_LIB) | $(BINDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) $< -L$(LIBDIR) -lsegtree -o $@

$(BINDIR)/example_%: $(EXAMPLEDIR)/example_%.c $(TARGET_LIB) | $(BINDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) $< -L$(LIBDIR) -lsegtree -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIBDIR):
	mkdir -p $(LIBDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

release: CFLAGS += -DNDEBUG
release: clean all

run-tests: tests
	@echo "Running all tests..."
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test..."; \
		$$test; \
		if [ $$? -ne 0 ]; then \
			echo "Test $$test failed!"; \
			exit 1; \
		fi; \
	done
	@echo "All tests passed!"

run-examples: examples
	@echo "Running all examples..."
	@for example in $(EXAMPLE_TARGETS); do \
		echo "Running $$example..."; \
		$$example; \
		echo ""; \
	done

valgrind-tests: tests
	@echo "Running tests with valgrind..."
	@for test in $(TEST_TARGETS); do \
		echo "Valgrind check for $$test..."; \
		valgrind --leak-check=full --error-exitcode=1 $$test; \
		if [ $$? -ne 0 ]; then \
			echo "Valgrind check failed for $$test!"; \
			exit 1; \
		fi; \
	done
	@echo "All valgrind checks passed!"

install: lib
	@echo "Installing segment tree library..."
	sudo cp $(TARGET_LIB) /usr/local/lib/
	sudo cp $(INCDIR)/*.h /usr/local/include/
	sudo ldconfig
	@echo "Installation completed!"

uninstall:
	@echo "Uninstalling segment tree library..."
	sudo rm -f /usr/local/lib/libsegtree.a
	sudo rm -f /usr/local/include/segment_tree.h
	sudo ldconfig
	@echo "Uninstallation completed!"

benchmark: examples
	@echo "Running performance benchmarks..."
	$(BINDIR)/test_performance

clean:
	rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR)
	@echo "Clean completed!"

help:
	@echo "Available targets:"
	@echo "  all          - Build library, tests, and examples"
	@echo "  lib          - Build only the library"
	@echo "  tests        - Build tests"
	@echo "  examples     - Build examples"
	@echo "  debug        - Build with debug flags"
	@echo "  release      - Build with release optimization"
	@echo "  run-tests    - Build and run all tests"
	@echo "  run-examples - Build and run all examples"
	@echo "  valgrind-tests - Run tests with valgrind memory check"
	@echo "  benchmark    - Run performance benchmarks"
	@echo "  install      - Install library system-wide"
	@echo "  uninstall    - Remove installed library"
	@echo "  clean        - Remove all build artifacts"
	@echo "  help         - Show this help message"

info:
	@echo "Build Configuration:"
	@echo "  Compiler: $(CC)"
	@echo "  Flags: $(CFLAGS)"
	@echo "  Source files: $(SOURCES)"
	@echo "  Object files: $(OBJECTS)"
	@echo "  Test files: $(TEST_SOURCES)"
	@echo "  Example files: $(EXAMPLE_SOURCES)"
	@echo "  Target library: $(TARGET_LIB)"