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

#include "json.h"

enum json_bool json_character_is_whitespace(char ascii)
{
	switch (ascii)
	{
		case ' ':
		case '\n':
		case '\r':
		case '\t':
			return TRUE;
		default:
			return FALSE;
	}
}

enum json_bool json_character_is_digit(char ascii)
{
	return '0' <= ascii && ascii <= '9' ? TRUE : FALSE;
}

enum json_bool json_character_is_hex(char ascii)
{
	return '0' <= ascii && ascii <= '9' || 'A' <= ascii && ascii <= 'F' || 'a' <= ascii && ascii <= 'f' ? TRUE : FALSE;
}

enum json_bool json_character_is_control(char ascii)
{
	return 0 <= ascii && ascii <= 31 || ascii == 127 ? TRUE : FALSE;
}

enum json_bool json_character_is_decimal(char ascii)
{
	return ascii == '.';
}

enum json_bool json_character_is_exponent(char ascii)
{
	return ascii == 'E' || ascii == 'e' ? TRUE : FALSE;
}

enum json_bool json_character_is_sign(char ascii)
{
	return ascii == '+' || ascii == '-';
}

enum json_state_string
{
	JSON_STATE_STRING(BASE)
};

enum json_state_number
{
	JSON_STATE_NUMBER(BASE)
};

enum json_state_true
{
	JSON_STATE_TRUE(BASE)
};

enum json_state_false
{
	JSON_STATE_FALSE(BASE)
};

enum json_state_null
{
	JSON_STATE_NULL(BASE)
};

enum json_state json_string_base(enum json_state state)
{
	if (OBJECT_KEY_STRING_START <= state && state <= OBJECT_KEY_STRING_END)
	{
		return OBJECT_KEY_STRING_START;
	}

	if (OBJECT_STRING_START <= state && state <= OBJECT_STRING_END)
	{
		return OBJECT_STRING_START;
	}

	if (ARRAY_STRING_START <= state && state <= ARRAY_STRING_END)
	{
		return ARRAY_STRING_START;
	}

	return ERROR;
}

enum json_state json_number_base(enum json_state state)
{
	if (OBJECT_NUMBER_START <= state && state <= OBJECT_NUMBER_END)
	{
		return OBJECT_NUMBER_START;
	}

	if (ARRAY_NUMBER_START <= state && state <= ARRAY_NUMBER_END)
	{
		return ARRAY_NUMBER_START;
	}

	return ERROR;
}

enum json_state json_true_base(enum json_state state)
{
	if (OBJECT_TRUE_START <= state && state <= OBJECT_TRUE_END)
	{
		return OBJECT_TRUE_START;
	}

	if (ARRAY_TRUE_START <= state && state <= ARRAY_TRUE_END)
	{
		return ARRAY_TRUE_START;
	}

	return ERROR;
}

enum json_state json_false_base(enum json_state state)
{
	if (OBJECT_FALSE_START <= state && state <= OBJECT_FALSE_END)
	{
		return OBJECT_FALSE_START;
	}

	if (ARRAY_FALSE_START <= state && state <= ARRAY_FALSE_END)
	{
		return ARRAY_FALSE_START;
	}

	return ERROR;
}

enum json_state json_null_base(enum json_state state)
{
	if (OBJECT_NULL_START <= state && state <= OBJECT_NULL_END)
	{
		return OBJECT_NULL_START;
	}

	if (ARRAY_NULL_START <= state && state <= ARRAY_NULL_END)
	{
		return ARRAY_NULL_START;
	}

	return ERROR;
}

enum json_state json_parse(enum json_state state, char ascii)
{
	switch (state)
	{
		case OBJECT_NUMBER_START:
		case ARRAY_NUMBER_START:
		
		case OBJECT_NUMBER_END:
		case ARRAY_NUMBER_END:
			return ERROR;

		case OBJECT_NUMBER_MINUS:
		case ARRAY_NUMBER_MINUS:
			if (ascii == '0')
			{
				return json_number_base(state) + BASE_NUMBER_ZERO;
			}
			
			if (json_character_is_digit(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER;	
			}

			return INVALID;
	
		case OBJECT_NUMBER_ZERO:
		case ARRAY_NUMBER_ZERO:
			if (json_character_is_decimal(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_FRACTION_START;
			}
			
			if (json_character_is_exponent(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_EXPONENT_START;
			}
			
			state = json_number_base(state) + BASE_NUMBER_END;
			break;

		case OBJECT_NUMBER:
		case ARRAY_NUMBER:
			if (json_character_is_digit(ascii) == TRUE)
			{
				return state;
			}
			
			if (json_character_is_decimal(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_FRACTION_START; 
			}
			
			if (json_character_is_exponent(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_EXPONENT_START;
			}

			state = json_number_base(state) + BASE_NUMBER_END;
			break;

		case OBJECT_NUMBER_FRACTION_START:
		case ARRAY_NUMBER_FRACTION_START:
			if (json_character_is_digit(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_FRACTION;
			}

			return INVALID;
		
		case OBJECT_NUMBER_FRACTION:
		case ARRAY_NUMBER_FRACTION:
			if (json_character_is_digit(ascii) == TRUE)
			{
				return state;
			}
			
			if (json_character_is_exponent(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_EXPONENT_START;
			}

			state = json_number_base(state) + BASE_NUMBER_END;
			break;
	
		case OBJECT_NUMBER_EXPONENT_START:
		case ARRAY_NUMBER_EXPONENT_START:
			if (json_character_is_sign(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_EXPONENT_SIGNED;
			}	
			
			if (json_character_is_digit(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_EXPONENT;
			}

			return INVALID;

		case OBJECT_NUMBER_EXPONENT_SIGNED:
		case ARRAY_NUMBER_EXPONENT_SIGNED:
			if (json_character_is_digit(ascii) == TRUE)
			{
				return json_number_base(state) + BASE_NUMBER_EXPONENT;
			}

			return INVALID;

		case OBJECT_NUMBER_EXPONENT:
		case ARRAY_NUMBER_EXPONENT:
			if (json_character_is_digit(ascii) == TRUE)
			{
				return state;
			}

			state = json_number_base(state) + BASE_NUMBER_END;
			break;

		default:
			break;
	}

	switch(state)
	{
		case OBJECT_END:
		case ARRAY_END:
			if (json_character_is_whitespace(ascii) == TRUE)
			{
				return WHITESPACE;
			}
			
		case WHITESPACE:
		case ERROR:
			return ERROR;
			
		case EMPTY:
			if (ascii == '{')
			{
				return OBJECT_START;
			}
			
			if (ascii == '[')
			{
				return ARRAY_START;
			}
			
			return json_character_is_whitespace(ascii) == TRUE ? WHITESPACE : INVALID;

		case OBJECT_START:
			if (ascii == '}')
			{
				return OBJECT_END;
			}
			
		case OBJECT_KEY:
			if (ascii == '"')
			{
				return OBJECT_KEY_STRING_START;
			}

			return json_character_is_whitespace(ascii) == TRUE ? WHITESPACE : INVALID;

		case OBJECT_KEY_STRING_END:
			if (ascii == ':')
			{
				return OBJECT_COLON;
			}

			return json_character_is_whitespace(ascii) == TRUE ? WHITESPACE : INVALID;

		case OBJECT_COLON:
			switch(ascii)
			{
				case '"':
					return OBJECT_STRING_START;
				case '-':
					return OBJECT_NUMBER_MINUS;
				case '0':
					return OBJECT_NUMBER_ZERO;
				case '{':
					return OBJECT_START;
				case '[':
					return ARRAY_START;
				case 't':
					return OBJECT_TRUE_START;
				case 'f':
					return OBJECT_FALSE_START;
				case 'n':
					return OBJECT_NULL_START;
				default:
					if (json_character_is_digit(ascii) == TRUE)
					{
						return OBJECT_NUMBER;
					}

					return json_character_is_whitespace(ascii) == TRUE ? WHITESPACE : INVALID;
			}	

		case OBJECT_STRING_END:
		case OBJECT_NUMBER_END:
		case OBJECT_TRUE_END:
		case OBJECT_FALSE_END:
		case OBJECT_NULL_END:
			if (ascii == ',')
			{
				return OBJECT_KEY;
			}
			
			if (ascii == '}')
			{
				return OBJECT_END;
			}

			return json_character_is_whitespace(ascii) == TRUE ? WHITESPACE : INVALID;
		
		case ARRAY_START:
			if (ascii == ']')
			{
				return ARRAY_END;
			}
			
		case ARRAY_VALUE:
			switch(ascii)
			{
				case '"':
					return ARRAY_STRING_START;
				case '-':
					return ARRAY_NUMBER_MINUS;
				case '0':
					return ARRAY_NUMBER_ZERO;
				case '{':
					return OBJECT_START;
				case '[':
					return ARRAY_START;
				case 't':
					return ARRAY_TRUE_START;
				case 'f':
					return ARRAY_FALSE_START;
				case 'n':
					return ARRAY_NULL_START;
				default:
					if (json_character_is_digit(ascii) == TRUE)
					{
						return ARRAY_NUMBER;
					}

					return json_character_is_whitespace(ascii) == TRUE ? WHITESPACE : INVALID;
			}		

		case ARRAY_STRING_END:
		case ARRAY_NUMBER_END:
		case ARRAY_TRUE_END:
		case ARRAY_FALSE_END:
		case ARRAY_NULL_END:
			if (ascii == ',')
			{
				return ARRAY_VALUE;
			}
			
			if (ascii == ']')
			{
				return ARRAY_END;
			}

			return json_character_is_whitespace(ascii) == TRUE ? WHITESPACE : INVALID;
			
		case OBJECT_KEY_STRING_START:
		case OBJECT_KEY_STRING:
		case OBJECT_KEY_STRING_ESCAPE_END:
		case OBJECT_KEY_STRING_HEX_END:

		case OBJECT_STRING_START:
		case OBJECT_STRING:
		case OBJECT_STRING_ESCAPE_END:
		case OBJECT_STRING_HEX_END:

		case ARRAY_STRING_START:
		case ARRAY_STRING:
		case ARRAY_STRING_ESCAPE_END:
		case ARRAY_STRING_HEX_END:
			if (ascii == '\\')
			{
				return json_string_base(state) + BASE_STRING_ESCAPE_START;
			}
			
			if (ascii == '"')
			{
				return json_string_base(state) + BASE_STRING_END;
			}
			
			if (json_character_is_control(ascii) == FALSE)
			{
				return json_string_base(state) + BASE_STRING;
			}

			return json_character_is_whitespace(ascii) == TRUE ? WHITESPACE : INVALID;

		case OBJECT_KEY_STRING_ESCAPE_START:
		case OBJECT_STRING_ESCAPE_START:
		case ARRAY_STRING_ESCAPE_START:
			switch(ascii)
			{
				case '"':
				case '\\':
				case '/':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					return json_string_base(state) + BASE_STRING_ESCAPE_END;
				case 'u':
					return json_string_base(state) + BASE_STRING_HEX_START;
				default:
					return INVALID;
			}

		case OBJECT_KEY_STRING_HEX_START:
		case OBJECT_STRING_HEX_START:
		case ARRAY_STRING_HEX_START:
			if (json_character_is_hex(ascii) == TRUE)
			{
				return json_string_base(state) + BASE_STRING_HEX0;
			}
		
			return INVALID;

		case OBJECT_KEY_STRING_HEX0:
		case OBJECT_STRING_HEX0:
		case ARRAY_STRING_HEX0:
			if (json_character_is_hex(ascii) == TRUE)
			{
				return json_string_base(state) + BASE_STRING_HEX1;
			}

			return INVALID;

		case OBJECT_KEY_STRING_HEX1:
		case OBJECT_STRING_HEX1:
		case ARRAY_STRING_HEX1:
			if (json_character_is_hex(ascii) == TRUE)
			{
				return json_string_base(state) + BASE_STRING_HEX2;
			}

			return INVALID;

		case OBJECT_KEY_STRING_HEX2:
		case OBJECT_STRING_HEX2:
		case ARRAY_STRING_HEX2:
			if (json_character_is_hex(ascii) == TRUE)
			{
				return json_string_base(state) + BASE_STRING_HEX_END;
			}

			return INVALID;

		case OBJECT_TRUE_START:
		case ARRAY_TRUE_START:
			if (ascii == 'r')
			{
				return json_true_base(state) + BASE_TRUE1;
			}

			return INVALID;

		case OBJECT_TRUE1:
		case ARRAY_TRUE1:
			if (ascii == 'u')
			{
				return json_true_base(state) + BASE_TRUE2;
			}

			return INVALID;

		case OBJECT_TRUE2:
		case ARRAY_TRUE2:
			if (ascii == 'e')
			{
				return json_true_base(state) + BASE_TRUE_END;
			}

			return INVALID;

		case OBJECT_FALSE_START:
		case ARRAY_FALSE_START:
			if (ascii == 'a')
			{
				return json_false_base(state) + BASE_FALSE1;
			}

			return INVALID;

		case OBJECT_FALSE1:
		case ARRAY_FALSE1:
			if (ascii == 'l')
			{
				return json_false_base(state) + BASE_FALSE2;
			}

			return INVALID;

		case OBJECT_FALSE2:
		case ARRAY_FALSE2:
			if (ascii == 's')
			{
				return json_false_base(state) + BASE_FALSE3;
			}

			return INVALID;

		case OBJECT_FALSE3:
		case ARRAY_FALSE3:
			if (ascii == 'e')
			{
				return json_false_base(state) + BASE_FALSE_END;
			}
	
			return INVALID;

		case OBJECT_NULL_START:
		case ARRAY_NULL_START:
			if (ascii == 'u')
			{
				return json_null_base(state) + BASE_NULL1;
			}

			return INVALID;

		case OBJECT_NULL1:
		case ARRAY_NULL1:
			if (ascii == 'l')
			{
				return json_null_base(state) + BASE_NULL2;
			}
				
			return INVALID;

		case OBJECT_NULL2:
		case ARRAY_NULL2:
			if (ascii == 'l')
			{
				return json_null_base(state) + BASE_NULL_END;
			}

			return INVALID;

		default:
			return INVALID;
	}
}
