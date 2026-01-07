/*
 * Copyright 2025 Ryan S. Marcil
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JSON_H
#define JSON_H

enum json_bool
{
	FALSE,
	TRUE,
};

enum json_bool json_character_is_whitespace(char ascii);
enum json_bool json_character_is_digit(char ascii);
enum json_bool json_character_is_hex(char ascii);
enum json_bool json_character_is_control(char ascii);

#define JSON_STATE_STRING(PREFIX) \
	PREFIX##_STRING_START, \
	PREFIX##_STRING, \
	PREFIX##_STRING_ESCAPE_START, \
	PREFIX##_STRING_ESCAPE_END, \
	PREFIX##_STRING_HEX_START, \
	PREFIX##_STRING_HEX0, \
	PREFIX##_STRING_HEX1, \
	PREFIX##_STRING_HEX2, \
	PREFIX##_STRING_HEX_END, \
	PREFIX##_STRING_END

#define JSON_STATE_NUMBER(PREFIX) \
	PREFIX##_NUMBER_START, \
	PREFIX##_NUMBER_MINUS, \
	PREFIX##_NUMBER_ZERO, \
	PREFIX##_NUMBER, \
	PREFIX##_NUMBER_FRACTION_START, \
	PREFIX##_NUMBER_FRACTION, \
	PREFIX##_NUMBER_EXPONENT_START, \
	PREFIX##_NUMBER_EXPONENT_SIGNED, \
	PREFIX##_NUMBER_EXPONENT, \
	PREFIX##_NUMBER_END

#define JSON_STATE_TRUE(PREFIX) \
	PREFIX##_TRUE_START, \
	PREFIX##_TRUE1, \
	PREFIX##_TRUE2, \
	PREFIX##_TRUE_END

#define JSON_STATE_FALSE(PREFIX) \
	PREFIX##_FALSE_START, \
	PREFIX##_FALSE1, \
	PREFIX##_FALSE2, \
	PREFIX##_FALSE3, \
	PREFIX##_FALSE_END

#define JSON_STATE_NULL(PREFIX) \
	PREFIX##_NULL_START, \
	PREFIX##_NULL1, \
	PREFIX##_NULL2, \
	PREFIX##_NULL_END

enum json_state
{
	EMPTY,
	WHITESPACE,

	OBJECT_START,
	OBJECT_KEY,
	JSON_STATE_STRING(OBJECT_KEY),
	OBJECT_COLON,
	JSON_STATE_STRING(OBJECT),
	JSON_STATE_NUMBER(OBJECT),
	JSON_STATE_TRUE(OBJECT),
	JSON_STATE_FALSE(OBJECT),
	JSON_STATE_NULL(OBJECT),
	OBJECT_END,
	
	ARRAY_START,
	ARRAY_VALUE,
	JSON_STATE_STRING(ARRAY),
	JSON_STATE_NUMBER(ARRAY),
	JSON_STATE_TRUE(ARRAY),
	JSON_STATE_FALSE(ARRAY),
	JSON_STATE_NULL(ARRAY),
	ARRAY_END,

	INVALID,
	ERROR,
};

enum json_state json_string_base(enum json_state state);
enum json_state json_number_base(enum json_state state);
enum json_state json_true_base(enum json_state state);
enum json_state json_false_base(enum json_state state);
enum json_state json_null_base(enum json_state state);
enum json_state json_parse(enum json_state state, char ascii);

#endif
