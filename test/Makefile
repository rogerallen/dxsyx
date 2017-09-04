# run tests and diff vs. golden output
# allow override of EXE location for testing
ifeq ($(EXE),)
EXE := ../bin/dxsyx
endif
DIFF := diff
MKDIR := mkdir
WC := wc

# always cause the tests to run
.PHONY: always

all: current tests

always:

tests: t_help t_default t_stdin t_crc t_yml t_breed t_select t_select_mk2
	@echo "======================================================================"
	@echo "tests PASSED"

# ----------------------------------------------------------------------
current/t_help.log: $(EXE) always golden/t_help.log
	@echo "======================================================================"
	@echo "t_help: see if we print expected help message"
	$(EXE) -h > current/t_help.log
	$(DIFF) golden/t_help.log current/t_help.log

t_help: current/t_help.log

# ----------------------------------------------------------------------
current/t_default.log: $(EXE) always golden/t_default.log
	@echo "======================================================================"
	@echo "t_default: see if we print expected default output"
	$(EXE) data/ROM1A.syx > current/t_default.log
	$(DIFF) golden/t_default.log current/t_default.log

t_default: current/t_default.log

# ----------------------------------------------------------------------
current/t_stdin.log: $(EXE) always golden/t_default.log
	@echo "======================================================================"
	@echo "t_stdin: see if we print expected default output for stdin filename"
	echo "data/ROM1A.syx" | $(EXE) -- > current/t_stdin.log
	$(DIFF) golden/t_default.log current/t_stdin.log

t_stdin: current/t_stdin.log

# ----------------------------------------------------------------------
current/t_crc.log: $(EXE) always golden/t_crc.log
	@echo "======================================================================"
	@echo "t_crc: see if we print expected crc output"
	$(EXE) -c data/ROM1A.syx > current/t_crc.log
	$(DIFF) golden/t_crc.log current/t_crc.log

t_crc: current/t_crc.log

# ----------------------------------------------------------------------
current/t_yml.log: $(EXE) always golden/t_yml.log
	@echo "======================================================================"
	@echo "t_yml: see if we print expected yml output"
	$(EXE) -y data/ROM1A.syx > current/t_yml.log
	$(DIFF) golden/t_yml.log current/t_yml.log

t_yml: current/t_yml.log

# ----------------------------------------------------------------------
current/t_breed.syx: $(EXE) always data/t_config.txt
	@echo "======================================================================"
	@echo "t_breed: see if we can breed new voices (just not crashing)"
	$(EXE) -b data/t_config.txt current/t_breed.syx
	$(EXE) current/t_breed.syx | $(WC) -l

t_breed: current/t_breed.syx

# ----------------------------------------------------------------------
current/t_select.log: $(EXE) always data/t_config.txt golden/t_select.log
	@echo "======================================================================"
	@echo "t_select: see if we can select voices for new syx file."
	$(EXE) -s data/t_config.txt current/t_select.syx
	$(EXE) current/t_select.syx > current/t_select.log
	$(DIFF) golden/t_select.log current/t_select.log

t_select: current/t_select.log

# ----------------------------------------------------------------------
current/t_select_mk2.log: $(EXE) always data/t_config.txt golden/t_select_mk2.log
	@echo "======================================================================"
	@echo "t_select_mk2: see if we can upgrade voices for new syx file."
	$(EXE) -U -u ../doc/mk2cfg.test.txt -s data/t_config.txt current/t_select_mk2.syx
	$(EXE) current/t_select_mk2.syx > current/t_select_mk2.log
	$(DIFF) golden/t_select_mk2.log current/t_select_mk2.log

t_select_mk2: current/t_select_mk2.log

# ----------------------------------------------------------------------
current:
	$(MKDIR) -p current

clean:
	rm -rf current
