# Makefile for mod_removeip.c (gmake)
# $Id:$

#APXS=/path/to/your/apxs
APXS=`which apxs`

default:
	@echo mod_removeip:
	@echo author: andrew@scoop.co.nz
	@echo
	@echo following options available:
	@echo \"make removeip\" to compile the 1.3 version
	@echo \"make install\" to install the 1.3 version
	@echo
	@echo change path to apxs if this is not it: \"$(APXS)\"


removeip: mod_removeip.so
	@echo make done, type \"make install\" to install mod_removeip

mod_removeip.so: mod_removeip.c
	$(APXS) -c -o $@ mod_removeip.c

mod_removeip.c:

install: mod_removeip.so
	$(APXS) -i -n mod_removeip mod_removeip.so

clean:
	rm -rf *~ *.o *.so *.lo *.la *.slo .libs/
