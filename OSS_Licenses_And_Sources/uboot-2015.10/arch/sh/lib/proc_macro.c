/*
 * Copyright (C) 2013 emtrion GmbH.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <image.h>

/* copy from file common/main.c because call not possible due to static attribute there */
static void process_macros (const char *input, char *output)
{
	char c, prev;
	const char *varname_start = NULL;
	int inputcnt = strlen (input);
	int outputcnt = CONFIG_SYS_CBSIZE;
	int state = 0;		/* 0 = waiting for '$'  */

	/* 1 = waiting for '(' or '{' */
	/* 2 = waiting for ')' or '}' */
	/* 3 = waiting for '''  */
#ifdef DEBUG
	char *output_start = output;

	debug("[PROCESS_MACROS] INPUT len %d: \"%s\"\n", strlen (input), input);
#endif

	prev = '\0';		/* previous character   */

	while (inputcnt && outputcnt) {
		c = *input++;
		inputcnt--;

		if (state != 3) {
			/* remove one level of escape characters */
			if ((c == '\\') && (prev != '\\')) {
				if (inputcnt-- == 0)
					break;
				prev = c;
				c = *input++;
			}
		}

		switch (state) {
		case 0:	/* Waiting for (unescaped) $    */
			if ((c == '\'') && (prev != '\\')) {
				state = 3;
				break;
			}
			if ((c == '$') && (prev != '\\')) {
				state++;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		case 1:	/* Waiting for (        */
			if (c == '(' || c == '{') {
				state++;
				varname_start = input;
			} else {
				state = 0;
				*(output++) = '$';
				outputcnt--;

				if (outputcnt) {
					*(output++) = c;
					outputcnt--;
				}
			}
			break;
		case 2:	/* Waiting for )        */
			if (c == ')' || c == '}') {
				int i;
				char envname[CONFIG_SYS_CBSIZE], *envval;
				int envcnt = input - varname_start - 1;	/* Varname # of chars */

				/* Get the varname */
				for (i = 0; i < envcnt; i++) {
					envname[i] = varname_start[i];
				}
				envname[i] = 0;

				/* Get its value */
				envval = getenv (envname);

				/* Copy into the line if it exists */
				if (envval != NULL)
					while ((*envval) && outputcnt) {
						*(output++) = *(envval++);
						outputcnt--;
					}
				/* Look for another '$' */
				state = 0;
			}
			break;
		case 3:	/* Waiting for '        */
			if ((c == '\'') && (prev != '\\')) {
				state = 0;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		}
		prev = c;
	}

	if (outputcnt)
		*output = 0;
	else
		*(output - 1) = 0;

#ifdef DEBUG
	debug("[PROCESS_MACROS] OUTPUT len %d: \"%s\"\n", strlen (output_start), output_start);
#endif

}

/*!
 **********************************************************************
 * returns a string where all ${var} macros have been expanded. If ${var} has
 * itself macros, they are also expanded
 * \param  macro string to be expanded. may be NULL
 * \retval static buffer to expanded macro
 *********************************************************************/
char *process_recursive_macros(char *macro)
{
        static char processed[CONFIG_SYS_CBSIZE];
        char unprocessed[CONFIG_SYS_CBSIZE];
        size_t length;

        if (macro == NULL)
                return NULL;

        processed[0]=0;
        strcpy(unprocessed, macro);

        do {
                length = strlen(unprocessed);

                process_macros(/*IN*/ unprocessed,  /*OUT*/ processed);
                strcpy(unprocessed, processed);
        } while (strlen(processed) != length);

        return processed;
}

