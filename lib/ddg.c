#include <Python.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
// #include <dlfcn.h>

#define HUB "root/zxcvx"
#define NAME "lightme"

size_t write_callback(void * contents, size_t size, size_t nmemb, FILE * stream)
{
	return fwrite(contents, size, nmemb, stream);
}

int get_webpage(const char * const link, unsigned char flag)
{
	printf("%s\n", "entering the get_webpage");

	/*
	#include "/root/zxcvx/repos/torsocks/src/lib/torsocks.h"
	const char * libtorsocks = "/root/zxcvx/packs/torsocks/src-last/lnk-last/lib/torsocks/libtorsocks.so";
	void *torsocks_handle = dlopen(libtorsocks, RTLD_LAZY);
	void (*tsocks_initialize_handle)(void) = dlsym(torsocks_handle, "tsocks_initialize");
	void (*tsocks_cleanup_handle)(void) = dlsym(torsocks_handle, "tsocks_cleanup");

	tsocks_initialize_handle();

	CURL *curl;
	CURLcode res;
	FILE *fp = NULL;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl == NULL) {
		return 1;
	}
	*/

	unsigned int query_len = strlen("torsocks /root/zxcvx/packs/curl/lnk/exes/curl \"") + strlen(link) + strlen("\" -o /" HUB "/packs/" NAME "/data/webpage.html");
	char * query = malloc(sizeof(char) * (query_len + 1));
	if (!query) {
		printf("malloc failed\n");
		return 1; 
	}
	printf("malloc not failed\n");
	if (flag == 0) {
		printf("flag is 0\n");
		// fp = fopen("/" HUB "/packs/" NAME "/data/webpage.html", "wb");
		strcpy(query, "torsocks /root/zxcvx/packs/curl/lnk/exes/curl \"");
		strcat(query, link);
		strcat(query, "\" -o /" HUB "/packs/" NAME "/data/webpage.html");
		query[query_len] = '\0';
		printf("%s\n", query);
		system(query);
	} else if (flag == 1) {
		printf("flag is 1\n");
		// fp = fopen("/" HUB "/packs/" NAME "/data/search.html", "wb");
		strcpy(query, "torsocks /root/zxcvx/packs/curl/lnk/exes/curl \"");
		strcat(query, link);
		strcat(query, "\" -o /" HUB "/packs/" NAME "/data/search.html");
		query[query_len] = '\0';
		printf("%s\n", query);
		system(query);
	}
	free(query);

	// torsocks /usr/bin/curl https://html.duckduckgo.com/html/?q=zxc
	// /usr/bin/curl --proxy socks5://127.0.0.1:9050 https://html.duckduckgo.com/html/?q=zxc
	// torsocks /zxc/curl || /zxc/curl --proxy
	// curl
	// onion: curl_easy_perform() failed: proxy handshake error 
	// norml: ddg problem 2.

	// onion hostname: duckduckgogg42xjoc72x3sjasowoarfbgcmvfimaftt6twagswzczad.onion
	// curl_easy_perform() failed: Couldn't resolve host name
	// after second call (from web):
	// double free or corruption (out)

	// onion hostname: duckduckgogg42xjoc72x3sjasowoarfbgcmvfimaftt6twagswzczad.onion
	// curl_easy_perform() failed: proxy handshake error
	// corrupted size vs. prev_size 

	// ddg starts throwing a captcha after N automated queries.
	// ddg problem 2:
	// ddg does not want to provide content at all (complaying about exit node) when  tor used.
	// Impersonating the browser would be nice but for current speed demand tor is enough.
	// curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:9050");
	// curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);

	/*
	curl_easy_setopt(curl, CURLOPT_URL, link);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return 1;
	}

	curl_easy_cleanup(curl);
	*/

	struct stat st;
	if (flag == 0) {
		if (stat("/" HUB "/packs/" NAME "/data/webpage.html", &st) != 0)
			return 1;
	} else if (flag == 1) {
		if (stat("/" HUB "/packs/" NAME "/data/search.html", &st) != 0)
			return 1;
	}

	/*
	fclose(fp);
	curl_global_cleanup();

	tsocks_cleanup_handle();
	dlclose(torsocks_handle);
	*/

	printf("%s\n", "exiting the get_webpage");
	return 0;
}

const char * get_title(const char * const webpage)
{
	Py_Initialize();

	const wchar_t *mod_dir = L"/root/zxcvx/packs/lightme/libs/";
	PySys_SetPath(mod_dir);

	/*
	const char * py_path = "/root/zxcvx/packs/lightme/lib/python3.11/site-packages/bs4";
	char py_pathvar[256];
	snprintf(py_pathvar, sizeof(py_pathvar), "PYTHONPATH=%s", py_path);
	putenv(py_pathvar);
	*/

	PyObject *py_mod_name = PyUnicode_DecodeFSDefault("ddg");
	PyObject *py_mod = PyImport_Import(py_mod_name);
	Py_DECREF(py_mod_name);

	if (py_mod == NULL) {
		PyErr_Print();
		return NULL;
	}

	PyObject * pyfn_get_title = PyObject_GetAttrString(py_mod, "get_title");
	if ((pyfn_get_title == NULL) && (PyCallable_Check(pyfn_get_title) == 0)) {
		PyErr_Print();
		return NULL;
	}

	if (get_webpage(webpage, 0) != 0) {
		printf("%s\n", "unsuccessible access to webpage :(");
		return NULL;
	}

	// PyObject * pyargs_get_title = PyTuple_Pack(1, PyUnicode_DecodeFSDefault(
	PyObject * pyretval_get_title = PyObject_CallObject(pyfn_get_title, NULL);
	if (pyretval_get_title == NULL) {
		PyErr_Print();
		return NULL;
	}
	const char * title = PyUnicode_AsUTF8(pyretval_get_title);

	Py_XDECREF(pyfn_get_title);
	Py_DECREF(py_mod);

	Py_Finalize();

	return title;
}

int get_links(char *** tit_sim_links, ssize_t * retlen)
{
	printf("entering the get_links\n");
	printf("entering the get_links2\n");
	Py_Initialize();
	printf("zxcA0\n");

	printf("zxcA1\n");
	const wchar_t *mod_dir = L"/root/zxcvx/packs/lightme/libs/";
	PySys_SetPath(mod_dir);
	printf("zxcA2\n");

	PyObject *py_mod_name = PyUnicode_DecodeFSDefault("ddg");
	PyObject *py_mod = PyImport_Import(py_mod_name);
	Py_DECREF(py_mod_name);

	printf("zxca1\n");
	if (py_mod == NULL) {
		PyErr_Print();
		return 1;
	}

	printf("zxca2\n");
	PyObject * pyfn_get_links = PyObject_GetAttrString(py_mod, "get_links");
	if ((pyfn_get_links == NULL) && (PyCallable_Check(pyfn_get_links) == 0)) {
		PyErr_Print();
		return 1;
	}

	printf("zxca3\n");
	PyObject * pyretval_get_links = PyObject_CallObject(pyfn_get_links, NULL);
	if (pyretval_get_links == NULL) {
		PyErr_Print();
		return 1;
	}
	Py_ssize_t len = PyTuple_Size(pyretval_get_links);	// long int
	printf("before malloc\n");
	*tit_sim_links = malloc(sizeof(char *) * len);
	printf("Py len = %ld\n", len);
	*retlen = (ssize_t)len;
	for (Py_ssize_t i = 0; i < len; ++i) {
		PyObject *py_cleaned_link = PyTuple_GetItem(pyretval_get_links, i);
		unsigned long int cleaned_link_len = strlen(PyUnicode_AsUTF8(py_cleaned_link));
		(*tit_sim_links)[i] = malloc(sizeof(char) * (cleaned_link_len + 1));
		(*tit_sim_links)[i] = PyUnicode_AsUTF8(py_cleaned_link);
		(*tit_sim_links)[i][cleaned_link_len] = '\0';
	}

	Py_XDECREF(pyfn_get_links);
	Py_DECREF(py_mod);

	Py_Finalize();

	printf("exiting the get_links\n");
	return 0;
}
