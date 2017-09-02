# run tests and diff vs. golden output
EXE=../bin/dxsyx
DIFF=diff
MKDIR_P = mkdir -p

# always cause the tests to run
.PHONY: always

all: current tests

always:

tests: t_help t_default
	@echo "======================================================================"
	@echo "tests PASSED"

current/t_help.log: $(EXE) always golden/t_help.log
	@echo "======================================================================"
	@echo "t_help: see if we print expected help message"
	$(EXE) -h > current/t_help.log
	$(DIFF) golden/t_help.log current/t_help.log

t_help: current/t_help.log

current/t_default.log: $(EXE) always golden/t_default.log
	@echo "======================================================================"
	@echo "t_default: see if we print expected default output"
	$(EXE) data/ROM1A.syx > current/t_default.log
	$(DIFF) golden/t_default.log current/t_default.log

t_default: current/t_default.log

current:
	$(MKDIR_P) current

clean:
	rm -rf current
