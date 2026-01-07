#include "json.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	#define STACK_SIZE 127
	size_t stack_length = 0;
	enum json_state stack[STACK_SIZE] = {};	

	char ascii;
	while((ascii = fgetc(stdin)) != EOF)
	{
		enum json_state state = json_parse(stack[stack_length], ascii);	

		switch(state)
		{
			case WHITESPACE:
				break;

			case OBJECT_START:
			case ARRAY_START:
				if(stack_length >= STACK_SIZE)
				{
					printf("STACK OVERFLOW %d > %d\n", (int)stack_length + 1, STACK_SIZE);
					exit(1);
				}				

				if(OBJECT_START <= stack[stack_length] && stack[stack_length] <= OBJECT_END)
				{
					stack[stack_length] = OBJECT_STRING_END;
				}
				else if(ARRAY_START <= stack[stack_length] && stack[stack_length] <= ARRAY_END)
				{
					stack[stack_length] = ARRAY_STRING_END;
				}

				stack[++stack_length] = state;
				break;

			case OBJECT_END:
			case ARRAY_END:
				if(stack_length == 0)
				{
					printf("STACK UNDERFLOW: %d < 0\n", (int)stack_length - 1);
					exit(2);
				}

				stack_length--;
				break;

			case INVALID:
				printf("INVALID JSON %d\n", stack[stack_length]);
				exit(3);

			case ERROR:
				printf("ERROR\n");
				exit(4);

			default:
				stack[stack_length] = state;
				break;
		}
	}

	printf("VALID JSON\n");
	return 0;
}
