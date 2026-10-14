BUILD := build
CXXFLAGS := -std=c++20 -Wall -Wextra -g -pthread -I common/cpp

.PHONY: run test clean

run:
ifndef FILE
	$(error Usage: make run FILE=path/to/File.cpp or path/to/File.java)
endif
ifeq ($(suffix $(FILE)),.cpp)
	@mkdir -p $(BUILD)
	g++ $(CXXFLAGS) -o $(BUILD)/$(basename $(notdir $(FILE))) $(FILE)
	@./$(BUILD)/$(basename $(notdir $(FILE)))
else ifeq ($(suffix $(FILE)),.java)
	@mkdir -p $(BUILD)
	javac -d $(BUILD) $(FILE)
	@java -ea -cp $(BUILD) $(basename $(notdir $(FILE)))
else
	$(error Unsupported file type: $(FILE))
endif

# Compile and run every .cpp under DIR; report PASS/FAIL per file and a summary.
# A compile error or non-zero exit counts as FAIL (stubs fail until you solve them).
test:
ifndef DIR
	$(error Usage: make test DIR=curriculum/101_basics)
endif
	@mkdir -p $(BUILD)
	@pass=0; fail=0; \
	for f in $$(find $(DIR) -name '*.cpp' | sort); do \
		b=$(BUILD)/$$(basename $$f .cpp); \
		if g++ $(CXXFLAGS) -o $$b $$f 2>/dev/null && perl -e 'alarm 20; exec @ARGV' $$b >/dev/null 2>&1; then \
			echo "PASS  $$f"; pass=$$((pass+1)); \
		else \
			echo "FAIL  $$f"; fail=$$((fail+1)); \
		fi; \
	done; \
	echo "$$pass passed, $$fail failed"

clean:
	rm -rf $(BUILD)
