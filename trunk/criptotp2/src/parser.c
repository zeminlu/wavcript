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
		} else if(strcmp(currentOpt, "-embed") == 0) {
			if(input->stegMode == INVALID_OPT) {
				input->stegMode = EMB;
				input->operation = ENC;
			} else {
				// TODO: Manage error, duplicated opcode.
			}
		} else if(strcmp(currentOpt, "-extract") == 0) {
			if(input->stegMode == INVALID_OPT) {
				input->stegMode = EXT;
				input->operation = DEC;
			} else {
				// TODO: Manage error, duplicated opcode.
			} 
		} else if(strcmp(currentOpt, "-steg") == 0) {
			if(i + 1 < argc) {
				if(input->stegAlg == INVALID_OPT) {
					input->stegAlg = parseStegAlg(argv[i+1]);
				} else {
					// TODO: Manage error, duplicated opcode.
				}		
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-p") == 0) {
			if(i + 1 < argc) {
				if(input->carrier == NULL) {
					input->carrier = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
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
	return INVALID_OPT;
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
	return INVALID_OPT;
}

t_steg_alg parseStegAlg(char *mode) {
	char *modes[] = {"lsb1", "lsb4", "lsbe"};
	int elems = sizeof(modes) / sizeof(char*);
	int i = 0;
	for(i = 0; i < elems; i++) {
        sToLower(&mode);
		if(strcmp(mode, modes[i]) == 0) {
			return i;
		}
	}
	return INVALID_OPT;
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
	input->carrier = NULL;
	input->stegMode = INVALID_OPT;
	input->stegAlg = INVALID_OPT;
}

boolean wrongInput(t_input *input) {
	if(input->stegMode == INVALID_OPT) {
		return TRUE;
	}
	if(input->input == NULL) {
		return TRUE;
	}
	if(input->carrier == NULL) {
		return TRUE;
	}
	if(input->output == NULL) {
		return TRUE;
	}
	if(input->stegAlg == INVALID_OPT) {
		return TRUE;
	}
	if(input->pass != NULL && input->algorithm != INVALID_OPT && input->mode == INVALID_OPT) {
		input->mode = CBC;
	}
	if(input->pass != NULL && input->algorithm == INVALID_OPT && input->mode != INVALID_OPT) {
		input->mode = AES128;
	}
	if(input->pass != NULL && input->algorithm == INVALID_OPT && input->mode == INVALID_OPT) {
		input->mode = AES128;
		input->mode = CBC;
	}
	return FALSE;
}
