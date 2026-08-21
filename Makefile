BUILD := build

.PHONY: run clean

run:
ifndef FILE
	$(error Usage: make run FILE=path/to/File.cpp or path/to/File.java)
endif
ifeq ($(suffix $(FILE)),.cpp)
	@mkdir -p $(BUILD)
	g++ -std=c++20 -Wall -Wextra -g -o $(BUILD)/$(basename $(notdir $(FILE))) $(FILE)
	@./$(BUILD)/$(basename $(notdir $(FILE)))
else ifeq ($(suffix $(FILE)),.java)
	@mkdir -p $(BUILD)
	javac -d $(BUILD) $(FILE)
	@java -cp $(BUILD) $(basename $(notdir $(FILE)))
else
	$(error Unsupported file type: $(FILE))
endif

clean:
	rm -rf $(BUILD)
