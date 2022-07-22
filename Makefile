PROGS := ls mkdir mkfile append cat useradd userinfo usermod su mem

.PHONY: all $(PROGS) clean

all: $(PROGS)
$(PROGS): %:
	@$(MAKE) -C _$@
	@cp _$@/$@ $@

clean:
	@rm -f $(PROGS)
	@$(foreach x, $(PROGS), $(MAKE) -C _$(x) clean;)
