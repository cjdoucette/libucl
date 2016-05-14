#include <stdio.h>
#include <ucl.h>

static inline void
print_usage(const char *msg)
{
	printf("%s\nusage: ./ucl_c <filename>\n", msg);
}

void
iterate(const ucl_object_t *obj)
{
	ucl_object_iter_t it = NULL;
	const ucl_object_t *cur = ucl_iterate_object(obj, &it, true);
	while (cur) {
		const char *key = ucl_object_key(cur);
		ucl_type_t type = ucl_object_type(cur);
		const char *type_str = ucl_object_type_to_string(type);

		switch (type) {
		case UCL_OBJECT:
			iterate(cur);
			break;
		case UCL_ARRAY: {
			/* To print nested arrays, the code gets ugly because
			 * nested arrays do not have keys themselves. To
			 * process nested arrays you can similarly iterate by
			 * recursively calling iterate(), but be aware that
			 * ucl_object_key() will be null.
			 */
			ucl_object_iter_t ait = NULL;
			const ucl_object_t *acur;
			printf("%s : [ ", key);
			while ((acur = ucl_iterate_object(cur, &ait, true))) {
				printf("%s ", ucl_object_tostring_forced(acur));
			}
			printf("]\n");
			break;
		}
		case UCL_INT: {
			int val = ucl_object_toint(cur);
			printf("%s : %d (%s)\n", key, val, type_str);
			break;
		}
		case UCL_FLOAT: {
			double val = ucl_object_todouble(cur);
			printf("%s : %f (%s)\n", key, val, type_str);
			break;
		}
		case UCL_STRING: {
			const char *val = ucl_object_tostring(cur);
			printf("%s : %s (%s)\n", key, val, type_str);
			break;
		}
		case UCL_BOOLEAN: {
			bool val = ucl_object_toboolean(cur);
			printf("%s : %s (%s)\n", key, val ? "true" : "false",
				type_str);
			break;
		}
		default:
			printf("none");
			break;
		}

		cur = ucl_iterate_object(obj, &it, true);
	}
}

int
main(int argc, char **argv)
{
	struct ucl_parser *parser;
	ucl_object_t *top;
	int ret = 0, r = 0;

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
	iterate(top);
	ucl_object_unref(top);

	ucl_parser_free(parser);

	return ret;
}
