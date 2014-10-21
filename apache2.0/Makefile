# Makefile for mod_removeip.c (gmake)
# $Id:$

#APXS2=/path/to/your/apxs
APXS2=`which apxs2`

default:
	@echo mod_removeip:
	@echo author: andrew@scoop.co.nz
	@echo
	@echo following options available:
	@echo \"make removeip\" to compile the 2.0 version
	@echo \"make install\" to install the 2.0 version
	@echo
	@echo change path to apxs if this is not it: \"$(APXS2)\"


removeip: mod_removeip.o
	@echo make done, type \"make install\" to install mod_removeip-2.0

mod_removeip.o: mod_removeip.c
	$(APXS2) -c -n $@ mod_removeip.c

mod_removeip.c:

install: mod_removeip.o
	$(APXS2) -i -n mod_removeip.so mod_removeip.la

clean:
	rm -rf *~ *.o *.so *.lo *.la *.slo .libs/
