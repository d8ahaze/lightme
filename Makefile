.PHONY: all layout layout_dirs libs libs_files webs web_files dems dems_files docs clean

CPP_OPTS = -Wall -Wextra -Werror=incompatible-pointer-types

hub ?= root/zxcvx
name = lightme

all: layout docs libs_x webs dems

layout: layout_dirs

layout_dirs: /$(hub)/packs/$(name)/data /$(hub)/packs/$(name)/infs /$(hub)/packs/$(name)/exes /$(hub)/packs/$(name)/logs /$(hub)/packs/$(name)/docs /$(hub)/packs/$(name)/libs /$(hub)/packs/$(name)/webs /$(hub)/packs/$(name)/dems

/$(hub)/packs/$(name)/exes:
	mkdir /$(hub)/packs/$(name)/exes

/$(hub)/packs/$(name)/logs:
	mkdir /$(hub)/packs/$(name)/logs

/$(hub)/packs/$(name)/docs:
	mkdir /$(hub)/packs/$(name)/docs

/$(hub)/packs/$(name)/libs:
	mkdir /$(hub)/packs/$(name)/libs

/$(hub)/packs/$(name)/infs:
	mkdir /$(hub)/packs/$(name)/infs

/$(hub)/packs/$(name)/webs:
	mkdir /$(hub)/packs/$(name)/webs

/$(hub)/packs/$(name)/dems:
	mkdir /$(hub)/packs/$(name)/dems

/$(hub)/packs/$(name)/data:
	mkdir /$(hub)/packs/$(name)/data

infs = $(hub)/repos/$(name)/include

ulf_pfx=root/zxcvx/packs/ulfius/src-latest/lnk-latest
mhd_pfx=root/zxcvx/packs/libmicrohttpd/src-latest/lnk-latest
orc_pfx=root/zxcvx/packs/orcania/src-latest/lnk-latest
ydr_pfx=root/zxcvx/packs/yder/src-latest/lnk-latest
jsn_pfx=root/zxcvx/packs/jansson/src-latest/lnk-latest
ltm_pfx=root/zxcvx/packs/lightme/

conv = -Wall -Wextra -g
infs = -I/$(ltm_pfx)/infs/ -I/$(ulf_pfx)/include/ -I/$(mhd_pfx)/infs/ -I/$(orc_pfx)/include/ -I/$(ydr_pfx)/include/ -I$(jsn_pfx)/infs/
libs = -L/usr/libs/ -L/$(ltm_pfx)/libs/ -L/$(ulf_pfx)/lib/ -L/$(mhd_pfx)/libs/ -L/$(orc_pfx)/lib/ -L/$(ydr_pfx)/lib/ -L/$(jsn_pfx)/libs/
objs = -ldominf -lpython3 -lddg -lulfius -lmicrohttpd -lorcania -lyder -ljansson

dems: dems_files

dems_files: /$(hub)/packs/$(name)/dems/api.e /$(hub)/packs/$(name)/dems/run.sh	\
	/$(hub)/packs/$(name)/dems/start.sh

/$(hub)/packs/$(name)/dems/start.sh: /$(hub)/repos/$(name)/dems/start.sh
	cp /$(hub)/repos/$(name)/dems/start.sh /$(hub)/packs/$(name)/dems/start.sh
	chmod 700 /$(hub)/packs/$(name)/dems/start.sh

/$(hub)/packs/$(name)/dems/run.sh: /$(hub)/repos/$(name)/dems/run.sh
	cp /$(hub)/repos/$(name)/dems/run.sh /$(hub)/packs/$(name)/dems/run.sh
	chmod 700 /$(hub)/packs/$(name)/dems/run.sh

/$(hub)/packs/$(name)/dems/api.o: /$(hub)/repos/$(name)/api/main.c
	gcc $(conv) -Werror=incompatible-pointer-types -c -O3 /$(hub)/repos/$(name)/api/main.c $(infs) -o /$(hub)/packs/$(name)/dems/api.o

/$(hub)/packs/$(name)/dems/api.e: /$(hub)/packs/$(name)/dems/api.o /$(hub)/packs/$(name)/libs/libddg.so
	gcc /$(hub)/packs/$(name)/dems/api.o -L/usr/lib -lpython3.11 -ldl -lm $(libs) $(objs) -o /$(hub)/packs/$(name)/dems/api.e

webs: web_files

web_files: /$(hub)/packs/$(name)/webs/dashboard.html

/$(hub)/packs/$(name)/webs/dashboard.html: /$(hub)/repos/$(name)/web/dashboard.html
	cp /$(hub)/repos/$(name)/web/dashboard.html /$(hub)/packs/$(name)/webs/dashboard.html
	chown root:zxc-nginx /$(hub)/packs/$(name)/webs/dashboard.html
	chmod 640 /$(hub)/packs/$(name)/webs/dashboard.html

libs_x: libs_files

libs_files: /$(hub)/packs/$(name)/infs/dominf.h /$(hub)/packs/$(name)/libs/libdominf.so \
	/$(hub)/packs/$(name)/infs/ddg.h /$(hub)/packs/$(name)/libs/libddg.so /$(hub)/packs/$(name)/libs/ddg.py \
	/$(hub)/packs/$(name)/libs/script.js /$(hub)/packs/$(name)/libs/style.css

# ======
/$(hub)/packs/$(name)/infs/dominf.h: /$(hub)/repos/$(name)/include/dominf.h
	cp /$(hub)/repos/$(name)/include/dominf.h /$(hub)/packs/$(name)/infs/dominf.h

/$(hub)/packs/$(name)/libs/dominf.o: /$(hub)/repos/$(name)/lib/dominf.c
	gcc $(CPP_OPTS) -O3 -fPIC					\
		-I/root/zxcvx/packs/curl/lnk/infs/			\
		-I/root/zxcvx/packs/jansson/src-latest/lnk-latest/infs/	\
		-c /$(hub)/repos/$(name)/lib/dominf.c			\
		-o /$(hub)/packs/$(name)/libs/dominf.o

/$(hub)/packs/$(name)/libs/libdominf.so: /$(hub)/packs/$(name)/libs/dominf.o
	gcc /$(hub)/packs/$(name)/libs/dominf.o						\
		-L/root/zxcvx/packs/curl/lnk/libs/ -lcurl				\
		-L/root/zxcvx/packs/jansson/src-latest/lnk-latest/libs/ -ljansson	\
		-shared -o /$(hub)/packs/$(name)/libs/libdominf.so

# ======
/$(hub)/packs/$(name)/infs/ddg.h: /$(hub)/repos/$(name)/include/ddg.h
	cp /$(hub)/repos/$(name)/include/ddg.h /$(hub)/packs/$(name)/infs/ddg.h

/$(hub)/packs/$(name)/libs/ddg.o: /$(hub)/repos/$(name)/lib/ddg.c
	gcc $(conv) -Werror=incompatible-pointer-types -O3 -c -fPIC -I/root/zxcvx/packs/curl/lnk/infs/ -I/usr/include -I/usr/include/python3.11/ /$(hub)/repos/$(name)/lib/ddg.c -o /$(hub)/packs/$(name)/libs/ddg.o

/$(hub)/packs/$(name)/libs/libddg.so: /$(hub)/packs/$(name)/libs/ddg.o
	gcc /$(hub)/packs/$(name)/libs/ddg.o -L/root/zxcvx/packs/curl/lnk/libs/ -L/usr/lib/ -ldl -lm -lcurl -lpython3 -shared -o /$(hub)/packs/$(name)/libs/libddg.so

/$(hub)/packs/$(name)/libs/ddg.py: /$(hub)/repos/$(name)/lib/ddg.py
	cp /$(hub)/repos/$(name)/lib/ddg.py /$(hub)/packs/$(name)/libs/ddg.py
# =======

/$(hub)/packs/$(name)/libs/script.js: /$(hub)/repos/$(name)/web/script.js
	cp /$(hub)/repos/$(name)/web/script.js /$(hub)/packs/$(name)/libs/script.js
	chown root:zxc-nginx /$(hub)/packs/$(name)/libs/script.js
	chmod 640 /$(hub)/packs/$(name)/libs/script.js

/$(hub)/packs/$(name)/libs/style.css: /$(hub)/repos/$(name)/web/style.css
	cp /$(hub)/repos/$(name)/web/style.css /$(hub)/packs/$(name)/libs/style.css
	chown root:zxc-nginx /$(hub)/packs/$(name)/libs/style.css
	chmod 640 /$(hub)/packs/$(name)/libs/style.css

docs: /$(hub)/packs/$(name)/docs/process-ru.html

/$(hub)/packs/$(name)/docs/process-ru.html: /$(hub)/repos/$(name)/docs/process-ru.html
	cp /$(hub)/repos/$(name)/docs/process-ru.html /$(hub)/packs/$(name)/docs/process-ru.html
	chown root:zxc-nginx /$(hub)/packs/$(name)/docs/process-ru.html
	chmod 640 /$(hub)/packs/$(name)/docs/process-ru.html

clean:
	rm -fr /$(hub)/packs/$(name)/*/*.{o,e,so}
