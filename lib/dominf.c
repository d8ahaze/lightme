#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <jansson.h>

// #include <Python.h>

#define URL "http://192.168.1.111:43/whois"

typedef unsigned long int u64;

struct membuf {
	char *mm;
	u64 sz;
};

static u64 membuf_cb(void *contents, u64 sz, u64 nmemb, void *ptr)
{
	u64 realsz = sz * nmemb;
	struct membuf *mem = (struct membuf *)ptr;

	mem->mm = realloc(mem->mm, mem->sz + realsz + 1);
	if (mem->mm == NULL)
		return 0;

	memcpy(&(mem->mm[mem->sz]), contents, realsz);
	mem->sz += realsz;
	mem->mm[mem->sz] = 0;

	return realsz;
}

char *get_regar_name(const char *json_resp)
{
	json_t *root;
	json_error_t error;

	root = json_loads(json_resp, 0, &error);
	if (!root) {
		fprintf(stderr, "Error parsing JSON: %s\n", error.text);
		return NULL;
	}

	json_t *whois = json_object_get(root, "whois");
	if (!json_is_object(whois)) {
		fprintf(stderr, "Error: 'whois' key is not found or not obj\n");
		json_decref(root);
		return NULL;
	}

	json_t *parsed = json_object_get(whois, "parsed");
	if (!json_is_object(parsed)) {
		fprintf(stderr, "Error: 'parsed' key is not found or not obj\n");
		json_decref(whois);
		json_decref(root);
		return NULL;
	}

	json_t *regar = json_object_get(parsed, "registrar");
	if (!json_is_object(regar)) {
		fprintf(stderr, "Error: 'registrar' key is not found or not obj\n");
		json_decref(parsed);
		json_decref(whois);
		json_decref(root);
		return NULL;
	}


	json_t *name = json_object_get(regar, "name");
	if (!json_is_string(name)) {
		fprintf(stderr, "Error: 'name' key is not found or not string\n");
		json_decref(regar);
		json_decref(parsed);
		json_decref(whois);
		json_decref(root);
		return NULL;
	}

	const char *regar_name = json_dumps(name, JSON_ENCODE_ANY);
	json_decref(root);
	return strdup(regar_name);
}

char *dominf(const char *const dom)
{
	char *regar = NULL;
	struct curl_slist *headers = NULL;
	char *url;
	struct membuf chunk;
	char *postdata;

	unsigned long int dom_len = strlen(dom);
	if (dom_len > 256 - 27)
		return NULL;
	
	if ((dom[dom_len - 4] == '.') && (dom[dom_len - 3] == 'e') && (dom[dom_len - 2] == 'd') && (dom[dom_len - 1] == 'u')) {
		regar = malloc(sizeof(char) * (strlen("EDUCAUSE") + 1));
		strcpy(regar, "EDUCAUSE");
		regar[strlen("EDUCAUSE")] = '\0';
		printf("Registrar name: %s\n", regar);
		return regar;
	}

	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl == NULL) {
		return NULL;
	}

	headers = curl_slist_append(headers, "Accepts: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	url = malloc(sizeof(char) * (strlen(URL) + 1));
	strcpy(url, URL);
	url[strlen(URL)] = '\0';
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POST, 1);

	postdata = malloc(sizeof(char) * (12 + dom_len + 1));
	snprintf(postdata, 12 + dom_len + 1, "{\"query\":\"%s\"}", dom);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, membuf_cb);

	chunk.mm = malloc(1);
	chunk.sz = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return NULL;
	} else {
		// printf("Response: %s\n", chunk.mm);
		regar = get_regar_name(chunk.mm);
		if (regar == NULL)
			return NULL;
		printf("Registrar name: %s\n", regar);
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	free(chunk.mm);
	curl_slist_free_all(headers);

	return regar;
}

/*
char * parse_result(const char * const dominf)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	char * ret = NULL;
	if (dominf[i] == ' ') {
		while (dominf[i] != '\n') {
			++i;
			if (dominf[i] == ':')
				j = i + 2;
		}
		printf("i=%d,j=%d\n", i, j);
		ret = malloc(sizeof(char) * j);
		j = 0;
		printf("%c\n", dominf[i + 1]);
		++i;
		while (1) {
			if ((dominf[i] == '\0') || (dominf[i] == '\n'))
				break;
			if (dominf[i - 2] == ':')
				j = 1;
			if (j == 1) {
				ret[k] = dominf[i];
				++k;
			}
			++i;
		}
		ret[k] = '\0';
	} else {
		unsigned long int len = strlen(dominf);
		printf("%d,%ld\n", i, len);
		while (i != len) {
			if (i > 2) {
				if (dominf[i - 2] == ':') {
					j = 1;
					printf("len=%ld,i=%d\n", len, i);
					ret = malloc(sizeof(char) * (len - i));
				}
			}
			if (j == 1) {
				ret[k] = dominf[i];
				++k;
			}
			++i;
		}
		ret[k - 1] = '\0';
	}

	return ret;
}
*/

/*
char * dominf(const char * const dom)
{
	Py_Initialize();

	const wchar_t *mod_dir = L"/root/zxcvx/packs/lightme/libs/";
	PySys_SetPath(mod_dir);

	PyObject *py_mod_name = PyUnicode_DecodeFSDefault("dominf");
	PyObject *py_mod = PyImport_Import(py_mod_name);
	Py_DECREF(py_mod_name);

	PyObject *pyfn_get_whois = PyObject_GetAttrString(py_mod, "get_whois");
	if ((pyfn_get_whois == NULL) && (PyCallable_Check(pyfn_get_whois) == 0)) {
		PyErr_Print();
		return NULL;
	}

	PyObject *pyargval_get_whois = PyTuple_Pack(1, PyUnicode_DecodeFSDefault(dom));
	PyObject *pyretval_get_whois = PyObject_CallObject(pyfn_get_whois, pyargval_get_whois);
	if (pyretval_get_whois == NULL) {
		PyErr_Print();
		return NULL;
	}

	char * regar = PyUnicode_AsUTF8(pyretval_get_whois);

	Py_XDECREF(pyfn_get_whois);
	Py_DECREF(py_mod);

	Py_Finalize();

	return regar;
}
*/

/*
char * dominf(const char * const dom)
{
	char * cleaned_result = NULL;
	char whois[256];
	unsigned long int dom_len = strlen(dom);
	if (dom_len > 256 - 27)
		return NULL;
	
	if ((dom[dom_len - 4] == '.') && (dom[dom_len - 3] == 'e') && (dom[dom_len - 2] == 'd') && (dom[dom_len - 1] == 'u')) {
		cleaned_result = malloc(sizeof(char) * (strlen("EDUCAUSE") + 1));
		strcpy(cleaned_result, "EDUCAUSE");
		cleaned_result[strlen("EDUCAUSE")] = '\0';
		return cleaned_result;
	}

	strcpy(whois, "whois ");
	strcat(whois, dom);
	strcat(whois, " | grep Registrar:");

	FILE * fp = popen(whois, "r");
	if (fp == NULL)
		return NULL;

	char result[512];
	size_t bytes_read;
	if ((bytes_read = fread(result, 1, sizeof(result) - 1, fp)) > 0) {
		result[bytes_read] = '\0';
		cleaned_result = parse_result(result);
	}
	if (cleaned_result != NULL) {
		pclose(fp);
		printf("%s\n", cleaned_result);
		return cleaned_result;
	}
	pclose(fp);

	strcpy(whois, "");
	strcpy(whois, "whois ");
	strcat(whois, dom);
	strcat(whois, " | grep registrar:");
	fp = popen(whois, "r");
	if (fp == NULL)
		return NULL;

	if ((bytes_read = fread(result, 1, sizeof(result) - 1, fp)) > 0) {
		result[bytes_read] = '\0';
		cleaned_result = parse_result(result);
	}
	if (cleaned_result != NULL) {
		pclose(fp);
		printf("%s\n", cleaned_result);
		return cleaned_result;
	}

	pclose(fp);
	return NULL;
}
*/

/*
int main(int x1, char * x2[])
{
	if (x1 == 1)
		return 1;
	dominf(x2[1]);
	return 0;
}
*/

/*
int main(int x1, char *x2[])
{
	if (x1 != 2)
		return 1;

	dominf(x2[1]);

	return 0;
}
*/
