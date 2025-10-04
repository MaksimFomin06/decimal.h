CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LIBS = -lcheck -lm -lsubunit -lpthread

BUILD_DIR = build
GCOV_DIR = gcov_report

LIB_NAME = s21_decimal.a
TEST_EXEC = test

SRCS = s21_arithmetic_operators.c s21_comparsion_operators.c s21_convertors_and_parsers.c s21_help_functions.c s21_other_functions.c s21_support.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
TEST_SRCS = test_*.c

.PHONY: all clean test s21_decimal.a gcov_report rebuild valgrind

all: s21_decimal.a

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

s21_decimal.a: $(OBJS)
	ar rcs $(LIB_NAME) $(OBJS)
	ranlib $(LIB_NAME)
	@echo "Library $(LIB_NAME) built successfully"

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB_NAME)
	$(CC) $(CFLAGS) $(TEST_SRCS) -L. -l:$(LIB_NAME) -o $(TEST_EXEC) $(LIBS)
	@echo "Running tests..."
	@./$(TEST_EXEC) || (echo "Tests failed"; exit 1)

valgrind: $(LIB_NAME)
	$(CC) $(CFLAGS) $(TEST_SRCS) -L. -l:$(LIB_NAME) -o $(TEST_EXEC) $(LIBS)
	@echo "Running valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(TEST_EXEC)

gcov_report: clean
	@echo "Generating coverage report..."
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRCS) $(TEST_SRCS) -o $(TEST_EXEC) $(LIBS)
	@./$(TEST_EXEC) > /dev/null 2>&1
	@mkdir -p $(GCOV_DIR)
	@lcov --capture --directory . --output-file $(GCOV_DIR)/coverage.info
	@lcov --remove $(GCOV_DIR)/coverage.info "test_*.c" -o $(GCOV_DIR)/filtered.info --ignore-errors unused
	@genhtml $(GCOV_DIR)/filtered.info --output-directory $(GCOV_DIR)
	@echo "Coverage report generated: $(GCOV_DIR)/index.html"
	@echo "Coverage summary:"
	@lcov --summary $(GCOV_DIR)/filtered.info 2>/dev/null | grep -E "lines|functions" || true
	@xdg-open $(GCOV_DIR)/index.html 2>/dev/null || open $(GCOV_DIR)/index.html 2>/dev/null || echo "Open $(GCOV_DIR)/index.html in your browser"

clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR) $(GCOV_DIR)
	rm -f $(LIB_NAME) $(TEST_EXEC) *.gcno *.gcda *.info valgrind-out.txt
	@echo "Clean completed"

rebuild: clean all

help:
	@echo "Available targets:"
	@echo "  all        - Build library $(LIB_NAME) (default)"
	@echo "  clean      - Remove all build artifacts"
	@echo "  test       - Build and run tests"
	@echo "  valgrind   - Run tests with valgrind memory checker"
	@echo "  gcov_report- Generate code coverage report"
	@echo "  rebuild    - Clean and rebuild everything"
	@echo "  help       - Show this help message"