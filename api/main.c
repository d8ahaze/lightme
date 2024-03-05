#include <stdio.h>
#include <ulfius.h>
#include <string.h>
#include <ddg.h>
#include <string.h>
#include <dominf.h>
#include <errno.h>

#define PORT 8080

int check_link(const char * const link)
{
	if ((link[0] != 'h') && (link[1] != 't') && (link[2] != 't') && (link[3] != 'p')) {
		printf("proto should be specified\n");
		return 1;
	}

	unsigned long int i = 4;
	unsigned long int len = strlen(link);
	while (i < len) {
		if (((link[i] >= 97) && (link[i] <= 122)) || ((link[i] >= 65) && (link[i] <= 90)) || (link[i] == 47) || (link[i] == 46) || (link[i] == 58)) {
			++i;
		} else {
			printf("link[%ld]=%c=%d\n", i, link[i], link[i]);
			return 1;
		}
	}
	return 0;
}

int shit2plus(char * const str)
{
	unsigned long int len = strlen(str);
	unsigned long int i = 0;
	printf("%d=%c\n", str[len - 1], str[len - 1]);
	printf("%d=%c\n", str[len - 2], str[len - 2]);
	printf("%d=%c\n", str[len - 3], str[len - 3]);
	while (i < len) {
		if (str[i] == ' ')
			str[i] = '+';
		if (str[i] == '\n')
			str[i] = '+';
		++i;
	}
	printf("zxczxczxc\n");
	printf("%d=%c\n", str[i], str[i]);
	// str[i] = '\0';
	// printf("%d=%c\n", str[i], str[i]);
	printf("len=%ld, i=%ld, sizeof=%ld\n", len, i, sizeof(*str));
	printf("%d=%c\n", str[len - 1], str[len - 1]);
	printf("%d=%c\n", str[len - 2], str[len - 2]);
	printf("%d=%c\n", str[len - 3], str[len - 3]);
	return 0;
}

// extract top domain from link to webpage
char * xtdom(const char * const wplink)
{
	char * domain = NULL;
	unsigned char i = 0;
	unsigned long int wplink_len = strlen(wplink);
	while (wplink[i] != '/') {
		++i;
		if ((unsigned long int)i == wplink_len)
			break;
	}
	signed char dot_cut_place = i;
	unsigned char dot_num = 0;
	while (dot_cut_place >= 0) {	
		if (wplink[dot_cut_place] == '.')
			++dot_num;
		if (dot_num > 1)
			break;
		--dot_cut_place;
	}
	if (dot_num >= 2)
		domain = malloc(sizeof(char) * (i - dot_cut_place - 1));
	else
		domain = malloc(sizeof(char) * i);
	unsigned char j = 0;
	while (1) {
		if (dot_num >= 2) {
			if (j == (i - dot_cut_place - 1)) {
				domain[j] = '\0';
				return domain;
			}
		} else {
			if (j == i) {
				domain[j] = '\0';
				return domain;
			}
		}
		if (dot_num >= 2)
			domain[j] = wplink[j + dot_cut_place + 1];
		else
			domain[j] = wplink[j];
		++j;
	}
}

int callback_diff(
	const struct _u_request * request,
	struct _u_response * response,
	void * user_data)
{
	if (user_data == NULL)
		printf("user's data is null\n");

	const char * link = u_map_get(request->map_post_body, "link");
	if (link == NULL) {
		return U_CALLBACK_IGNORE;
	} else if (check_link(link)) {
		printf("link is invalid\n");
		return U_CALLBACK_IGNORE;
	}
	json_t * json_obj;
	json_t * json_arr;
	json_arr = json_array();
	const char * title = get_title(link);
	unsigned long int len_title_test = strlen(title);
	char * title_alloc = malloc(sizeof(char) * (strlen(title) + 1));
	strcpy(title_alloc, title);
	title_alloc[len_title_test] = '\0';
	printf("BEFORE get_title\n");
	printf("%c=%d\n", title[len_title_test - 1], title[len_title_test - 1]);
	printf("%c=%d\n", title[len_title_test - 2], title[len_title_test - 2]);
	printf("AFTER get_title\n");
	printf("BEFORE (alloc) get_title\n");
	printf("%c=%d\n", title_alloc[len_title_test - 1], title_alloc[len_title_test - 1]);
	printf("%c=%d\n", title_alloc[len_title_test - 2], title_alloc[len_title_test - 2]);
	printf("AFTER (alloc) get_title\n");

	if (title == NULL)
		return U_CALLBACK_IGNORE;

	json_obj = json_object();
	json_object_set(json_obj, "link", json_string(link));
	json_object_set(json_obj, "title", json_string(title_alloc));

	#define SEAENG "https://html.duckduckgo.com/html/?q="
	#define SEAENG_TOR "https://duckduckgogg42xjoc72x3sjasowoarfbgcmvfimaftt6twagswzczad.onion/html/?q="
	char * seastr = malloc(sizeof(char) * (strlen(SEAENG_TOR) + strlen(title_alloc) + 1));
	strcpy(seastr, SEAENG_TOR);
	strcat(seastr, title_alloc);
	unsigned long int len_zxc = strlen(SEAENG_TOR) + strlen(title_alloc);
	free(title_alloc);
	seastr[len_zxc] = '\0';
	printf("len_zxc=%ld\n", len_zxc);
	shit2plus(seastr);
	printf("before get_webpage\n");
	if (get_webpage(seastr, 1) == 1)
		return 1;
	printf("after get_webpage\n");
	free(seastr);
	printf("after free(seastr)\n");

	json_object_set(json_obj, "ent1", json_string("zxc"));
	printf("after json obj append key1 : val\n");
	json_object_set(json_obj, "ent2", json_string("zxc"));
	printf("after json obj append key2 : val\n");
	json_array_append(json_arr, json_obj);
	printf("after json arr append obj\n");

	char ** tit_sim_links = NULL;
	ssize_t len;
	if (get_links(&tit_sim_links, &len) != 0)
		return 1;
	json_obj = json_object();
	ssize_t i = 0;
	printf("len=%ld\n", len);
	while (i < len) {
		printf("i=%ld\n", i);
		char key[32];
		sprintf(key, "%ld", i);
		json_object_set(json_obj, key, json_string(tit_sim_links[i]));
		printf("%s\n", tit_sim_links[i]);
		++i;
	}
	json_array_append(json_arr, json_obj);

	json_obj = json_object();
	unsigned int * keys_arr = (unsigned int *)malloc(sizeof(unsigned int) * (unsigned int)len);
	i = 0;
	while (i <= len) {
		keys_arr[i] = i;
		++i;
	}
	i = 0;
	printf("LEN=%ld\n", len);
	while (i < len) {
		char key[16];
		sprintf(key, "%d", keys_arr[i]);
		printf("key:%s:key\n", key);
		printf("tit_sim_links[i] is: %s\n", tit_sim_links[i]);
		char * dom = xtdom(tit_sim_links[i]);
		printf("dom is: %s\n", dom);
		char * regar = dominf(dom);
		printf("regar is: %s\n", regar);
		printf("dom freeing\n");
		free(dom);
		printf("Error: %s\n", strerror(errno));
		json_object_set(json_obj, key, json_string(regar));
		// printf("regar freeing\n");
		// free(regar);
		// printf("Error: %s\n", strerror(errno));
		++i;
	}
	printf("keys_arr freeing\n");
	free(keys_arr);
	printf("Error: %s\n", strerror(errno));
	json_array_append(json_arr, json_obj);

	free(tit_sim_links);
	printf("tit_sim_links freeing\n");
	json_decref(json_obj);
	ulfius_set_json_body_response(response, 200, json_arr);
	json_decref(json_arr);

	return U_CALLBACK_CONTINUE;
}

static char * read_file(const char * filename)
{
	char * buffer = NULL;
	long length;
	FILE * f;
	if (filename != NULL) {
		f = fopen(filename, "rb");
		if (f) {
			fseek(f, 0, SEEK_END);
			length = ftell(f);
			fseek(f, 0, SEEK_SET);
			buffer = malloc((size_t)(length + 1));
			if (buffer != NULL) {
				fread(buffer, 1, (size_t)length, f);
				buffer[length] = '\0';
			}
			fclose(f);
		}
		return buffer;
	} else {
		return NULL;
	}
}

int main(void)
{
	struct _u_instance instance;

	if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
		fprintf(stderr, "Error ulfius_init_instance, abort\n");
		return 1;
	}

	u_map_put(instance.default_headers, "Access-Control-Allow-Origin", "*");

	ulfius_add_endpoint_by_val(&instance, "POST", "/api/getdiff", NULL, 0,
		&callback_diff, NULL);

	char *priv_key = read_file("/etc/letsencrypt/live/zxcvx.cc/privkey.pem");
	char *pub_key = read_file("/etc/letsencrypt/live/zxcvx.cc/fullchain.pem");
	if (ulfius_start_secure_framework(&instance, priv_key, pub_key) == U_OK) {
		free(priv_key);
		free(pub_key);
		printf("Start framework on port %d\n", instance.port);
		getchar();
	} else {
		fprintf(stderr, "Error start framework");
	}

	printf("End framework\n");
	ulfius_stop_framework(&instance);
	ulfius_clean_instance(&instance);

	return 0;
}
