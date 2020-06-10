BUILDDIR=$(PWD)
SYS=`fs sysname | cut -d "'" -f 2`
BINDIR=$(DESTDIR)/usr/bin
SBINDIR=$(DESTDIR)/usr/sbin
MANDIR=$(DESTDIR)/usr/share/man

all:  setuid-akcron akcron-helper akcron

clean:
	rm -f *~ *.o setuid-akcron

test: setuid-akcron.c
	@echo "###########################################################"
	@echo "WARNING: building a test binary which will exec the program"
	@echo "$(BUILDDIR)/akcron"
	@echo "###########################################################"
	@echo
	gcc -o setuid-akcron -DAKCRON_SCRIPT=\"$(BUILDDIR)/akcron\" setuid-akcron.c
#chown root setuid-akcron
#chmod 4755 setuid-akcron

setuid-akcron: setuid-akcron.c
	gcc -o setuid-akcron setuid-akcron.c
#chown root setuid-akcron
#chmod 4755 setuid-akcron

akcron:
	chmod 755 akcron

akcron-helper:
	chmod 500 akcron-helper

man:	akcron.1

akcron.1: akcron
	pod2man akcron akcron.1

tar:
	tar czvf  ~/src/rpms/SOURCES/akcron.tar.gz ../akcron-1.2

install: setuid-akcron akcron akcron-helper akcron.1
	echo $(SYS)
	mkdir -p $(BINDIR)
	mkdir -p $(SBINDIR)
	mkdir -p $(MANDIR)/man1
	install -m 755 setuid-akcron $(SBINDIR)/setuid-akcron
	install -m 755 akcron-helper $(SBINDIR)/akcron-helper
	install -m 755 akcron $(BINDIR)/akcron
	install -m 644 akcron.1 $(MANDIR)/man1/akcron.1
	fsr sa $(DESTDIR)/$(SYS) system:anyuser read
	fsr sa $(DESTDIR)/common system:anyuser read
