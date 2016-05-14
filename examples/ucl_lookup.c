#include <assert.h>
#include <stdio.h>
#include <ucl.h>

static inline void
print_usage(const char *msg)
{
	printf("%s\nusage: ./ucl_c2 <filename>\n", msg);
}

int
main(int argc, char **argv)
{
	ucl_object_iter_t it;
	struct ucl_parser *parser;
	ucl_object_t *top;
	const ucl_object_t *obj;
	const ucl_object_t *cur;

	/* Sample configuration data. */
	const char *name;
	int day, month, year;
	int arr[3];
	int i = 0;

	if (argc != 2) {
		print_usage("wrong number of arguments");
		return -1;
	}

	parser = ucl_parser_new(0);
	if (!parser) {
		printf("not enough memory for a parser\n");
		return -1;
	}

	if (!ucl_parser_add_file(parser, argv[1])) {
		print_usage(ucl_parser_get_error(parser));
		return -1;
	}

	top = ucl_parser_get_object(parser);

	/* Sample of fetching primitive type. */
	obj = ucl_object_lookup(top, "name");
	assert(ucl_object_type(obj) == UCL_STRING);
	name = ucl_object_tostring(obj);

	/* Sample of fetching object type. */
	obj = ucl_object_lookup(top, "date");
	assert(ucl_object_type(obj) == UCL_OBJECT);
	it = ucl_object_iterate_new(obj);
	while ((cur = ucl_object_iterate_safe(it, true))) {
		const char *key = ucl_object_key(cur);
		assert(ucl_object_type(cur) == UCL_INT);
		if (strcmp(key, "year") == 0)
			year = ucl_object_toint(cur);
		else if (strcmp(key, "month") == 0)
			month = ucl_object_toint(cur);
		else if (strcmp(key, "day") == 0)
			day = ucl_object_toint(cur);
		else
			assert(false);
	}

	/* Sample of fetching array type. */
	obj = ucl_object_lookup(top, "arr");
	assert(ucl_object_type(obj) == UCL_ARRAY);
	/* Note that we're resetting the iterator here. */
	it = ucl_object_iterate_reset(it, obj);
	while ((cur = ucl_object_iterate_safe(it, true))) {
		assert(ucl_object_type(cur) == UCL_INT);
		arr[i++] = ucl_object_toint(cur);
	}

	printf("Name: %s\n", name);
	printf("Array: [%d, %d, %d]\n", arr[0], arr[1], arr[2]);
	printf("Date: %d-%d-%d\n", year, month, day);

	ucl_object_iterate_free(it);
	ucl_object_unref(top);
	ucl_parser_free(parser);
	return 0;
}
