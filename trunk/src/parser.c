/**
 * \file parser.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#include "../inc/parser.h"

t_input * parseInput(int argc, char *argv[]) {
	t_input *input = malloc(sizeof(t_input));
	initInWrongValues(input);
	
	int i = 1;
	for(i = 1; i < argc; i++) {
		char *currentOpt = argv[i];

		if(strcmp(currentOpt, "-in") == 0) {
			if(i + 1 < argc) {
				if(input->input == NULL) {
					input->input = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-out") == 0) {
			if(i + 1 < argc) {
				if(input->output == NULL) {
					input->output = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-pass") == 0) {
			if(i + 1 < argc) {
				if(input->pass == NULL) {
					input->pass = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-K") == 0) {
			if(i + 1 < argc) {
				if(input->key == NULL) {
					input->key = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-iv") == 0) {
			if(i + 1 < argc) {
				if(input->iv == NULL) {
					input->iv = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-a") == 0) {
			if(i + 1 < argc) {
				if(input->algorithm == INVALID_OPT) {
					input->algorithm = parseAlgorithm(argv[i+1]);
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-m") == 0) {
			if(i + 1 < argc) {
				if(input->mode == INVALID_OPT) {
					input->mode = parseMode(argv[i+1]);
				} else {
					// TODO: Manage error, duplicated opcode.
				}		
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-d") == 0) {
			if(input->operation == INVALID_OPT) {
				input->operation = DEC;
			} else{
				// TODO: Manage error, duplicated opcode.
			}
		} else if(strcmp(currentOpt, "-e") == 0) {
			if(input->mode) {
				input->operation = ENC;
			} else {
				// TODO: Manage error, duplicated opcode.
			}
		} else {
			// TODO: Manage error, inexistant option.
		}
	}
	
	if(wrongInput(input)) {
		input = NULL;
		// TODO: Manage error, missing parameters.
	}
	
	return input;
}

t_alg parseAlgorithm(char *algorithm) {
	char *algorithms[] = {"aes128", "aes192", "aes256", "des"};
	int elems = sizeof(algorithms) / sizeof(char*);
	
	int i = 0;
	for(i = 0; i < elems; i++) {
	    sToLower(&algorithm);
		if(strcmp(algorithm, algorithms[i]) == 0) {
			return i;
		}
	}
	
	return 0;
}

t_mode parseMode(char *mode) {
	char *modes[] = {"ecb", "cfb", "ofb", "cbc"};
	int elems = sizeof(modes) / sizeof(char*);
	
	int i = 0;
	for(i = 0; i < elems; i++) {
        sToLower(&mode);
		if(strcmp(mode, modes[i]) == 0) {
			return i;
		}
	}
	
	return 0;
}

void initInWrongValues(t_input *input) {
	input->input = NULL;
	input->output = NULL;
	input->operation = INVALID_OPT;
	input->pass = NULL;
	input->key = NULL;
	input->iv = NULL;
	input->algorithm = INVALID_OPT;
	input->mode = INVALID_OPT;
}

boolean wrongInput(t_input *input) {
	if(input->input == NULL) {
		return TRUE;
	}
	if(input->output == NULL) {
		return TRUE;
	}
	if(input->operation == INVALID_OPT) {
		return TRUE;
	}
	if(input->pass == NULL) {
		if(input->key == NULL || input->iv == NULL) {
			return TRUE;
		}
	}
	if(input->pass != NULL) {
		if(input->key != NULL || input->iv != NULL) {
			return TRUE;
		}
	}
	if(input->algorithm == INVALID_OPT) {
		return TRUE;
	}
	if(input->algorithm > 3) {
		return TRUE;
	}
	if(input->mode == INVALID_OPT) {
		return TRUE;
	}
	if(input->mode > 3) {
		return TRUE;
	}
	return FALSE;
}
