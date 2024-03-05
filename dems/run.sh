#!/usr/bin/bash

run()
{
	export LD_LIBRARY_PATH=/root/zxcvx/packs/ulfius/src-latest/lnk-latest/lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=/root/zxcvx/packs/libmicrohttpd/src-latest/lnk-latest/libs:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=/root/zxcvx/packs/orcania/src-latest/lnk-latest/lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=/root/zxcvx/packs/yder/src-latest/lnk-latest/lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=/root/zxcvx/packs/jansson/src-latest/lnk-latest/lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=/root/zxcvx/packs/lightme/libs:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=/root/zxcvx/packs/curl/lnk/libs:$LD_LIBRARY_PATH
	# export LD_PRELOAD=/root/zxcvx/packs/curl/src-latest/lnk-latest/libs/libcurl.so
	# export LD_PRELOAD=/root/zxcvx/packs/curl/lnk/libs/libcurl.so
	# export LD_PRELOAD=/root/zxcvx/packs/torsocks/src-last/lnk-last/lib/torsocks/libtorsocks.so
	cd /root/zxcvx/packs/lightme/dems
	# strace -o ../logs/api.e.trace ./api.e
	./api.e
}

run
