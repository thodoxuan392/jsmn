/*
 * jsmng_test.c
 *
 *  Created on: Jun 25, 2023
 *      Author: xuanthodo
 */

#include "string.h"
#include "Lib/jsmn/jsmn_test.h"
#include "Lib/jsmn/jsmn.h"
#include "Lib/utils/utils_logger.h"

static const char *JSON_STRING =
    "{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\r\n  "
    "\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

void JSMNG_test(){
	jsmn_parser p;
	 jsmntok_t t[128]; /* We expect no more than 128 tokens */

	  jsmn_init(&p);
	  uint32_t r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
	                 sizeof(t) / sizeof(t[0]));
	  if (r < 0) {
	    utils_log_debug("Failed to parse JSON: %d\r\n", r);
	    return 1;
	  }

	  /* Assume the top-level element is an object */
	  if (r < 1 || t[0].type != JSMN_OBJECT) {
	    utils_log_debug("Object expected\r\n");
	    return 1;
	  }

	  /* Loop over all keys of the root object */
	  for (uint32_t i = 1; i < r; i++) {
	    if (jsmn_streq(JSON_STRING, &t[i], "user") == 0) {
	      /* We may use strndup() to fetch string value */
	      utils_log_debug("- User: %.*s\r\n", t[i + 1].end - t[i + 1].start,
	             JSON_STRING + t[i + 1].start);
	      i++;
	    } else if (jsmn_streq(JSON_STRING, &t[i], "admin") == 0) {
	      /* We may additionally check if the value is either "true" or "false" */
	      utils_log_debug("- Admin: %.*s\r\n", t[i + 1].end - t[i + 1].start,
	             JSON_STRING + t[i + 1].start);
	      i++;
	    } else if (jsmn_streq(JSON_STRING, &t[i], "uid") == 0) {
	      /* We may want to do strtol() here to get numeric value */
	      utils_log_debug("- UID: %.*s\r\n", t[i + 1].end - t[i + 1].start,
	             JSON_STRING + t[i + 1].start);
	      i++;
	    } else if (jsmn_streq(JSON_STRING, &t[i], "groups") == 0) {
	      int j;
	      utils_log_debug("- Groups:\r\n");
	      if (t[i + 1].type != JSMN_ARRAY) {
	        continue; /* We expect groups to be an array of strings */
	      }
	      for (j = 0; j < t[i + 1].size; j++) {
	        jsmntok_t *g = &t[i + j + 2];
	        utils_log_debug("  * %.*s\r\n", g->end - g->start, JSON_STRING + g->start);
	      }
	      i += t[i + 1].size + 1;
	    } else {
	      utils_log_debug("Unexpected key: %.*s\r\n", t[i].end - t[i].start,
	             JSON_STRING + t[i].start);
	    }
	  }
}
