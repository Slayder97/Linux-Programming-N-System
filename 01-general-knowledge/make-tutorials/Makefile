.PHONY: rule1 rule2 rule3
 
var := "var"
var3 := "var3"


var1 = $(var)
var2 := $(var)
var3 ?= $(var)

var := "changed"


rule3:
	@echo "$(var1)"
	@echo "$(var2)"
	@echo "$(var3)"

rule1:
	echo "Hi im rule 1"

rule2:
	@echo "Hi im rule 2"

include abcd.mk
