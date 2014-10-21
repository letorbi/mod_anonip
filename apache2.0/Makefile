# Makefile for mod_anonip.c (gmake)
# $Id:$

#APXS2=/path/to/your/apxs
APXS2=`which apxs2`

default:
	@echo mod_anonip:
	@echo author: andrew@scoop.co.nz
	@echo
	@echo following options available:
	@echo \"make anonip\" to compile the 2.0 version
	@echo \"make install\" to install the 2.0 version
	@echo
	@echo change path to apxs if this is not it: \"$(APXS2)\"


anonip: mod_anonip.o
	@echo make done, type \"make install\" to install mod_anonip-2.0

mod_anonip.o: mod_anonip.c
	$(APXS2) -c -n $@ mod_anonip.c

mod_anonip.c:

install: mod_anonip.o
	$(APXS2) -i -n mod_anonip.so mod_anonip.la

clean:
	rm -rf *~ *.o *.so *.lo *.la *.slo .libs/
