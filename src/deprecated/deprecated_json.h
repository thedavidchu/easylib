/// Source: https://www.json.org/json-en.html
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

enum JSONType {
	JSON_ERROR = 0,	/* Return this for errors */
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_STRING,
	JSON_FLOAT,
	JSON_INT,
	JSON_TRUE,
	JSON_FALSE,
	JSON_NULL,
};

struct JSONString {
	char *data;
	uint64_t length;
	uint64_t capacity;
};

struct JSONFloat {
	double data;
};

struct JSONInt {
	int64_t data;
}

struct JSONObject {
	struct JSONString *keys;
	struct JSONElementType *values;
	uint64_t length;
	uint64_t capacity;
};

struct JSONArray {
	JSONElementType *data;
	uint64_t length;
	uint64_t capacity;
};


struct JSONElement {
	enum JSONType type;
	struct JSONObject;
	struct JSONArray;
	struct JSONString;
	struct JSONFloat;
	struct JSONInt;
};

enum JSONType
json__get_type(struct JSONElement *me)
{
	if (me == NULL) {
		return JSON_ERROR;
	}
	return me->type;
}



bool
json__init_from_file(JSONElementType *me, char const *const restrict file_name)
{
	if (me == NULL) {
		return false;
	}

	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		return false;
	}
	// TODO: parse object
}

