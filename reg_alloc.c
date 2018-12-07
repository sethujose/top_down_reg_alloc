#include <stdio.h>
#include <stdlib.h>
#include "reg_alloc.h"
#include "list.h"
#include "string.h"
#include "merge_sort.h"

int reg_f[REG_SPILL_CNT];

void RegAlloc_GenerateVarList(FILE *fp_in)
{
    char line[50];
    int line_count = 0;

    while(NULL != fgets(line, 50, fp_in))
    {
        line_count++;
        
        char *op_code = strtok(line, " ");

        if (strcmp(op_code, OP_CODE) == 0)
        {
            char *var;

            while ((var = strtok(NULL, ", \n")) != NULL)
            {
                SymTable_addVariable(var);
            }
        }
        else
        {
            printf("Invalid op code %s at line %d!", op_code, line_count);
            exit(EXIT_FAILURE);
        }
    }

    if ( fseek(fp_in, 0L, SEEK_SET) != 0 )
    {
        printf("Error setting file pointer to start\n");
        exit(EXIT_FAILURE);
    }
}

void RegAlloc_AllocReg(varList_t *pHead, int numReg)
{
    varList_t *cursor = pHead;

    int varReg = 1;
    
    while ((cursor != NULL) && (varReg <= numReg))
    {
        cursor->regName = varReg;

        varReg++;
        cursor = cursor->next;
    }
}

void RegAlloc_generateCode(varList_t *pHead, FILE *fp_in, FILE *fp_out)
{
    char line[50];
    int line_count = 0;

    while(NULL != fgets(line, 50, fp_in))
    {
        line_count++;
       
        char *op_code = strtok(line, " ");

        if (strcmp(op_code, OP_CODE) == 0)
        {
            char *var;
            char out_line[50];
            int var_pos = 0;

            strcpy(out_line, op_code);

            while ((var = strtok(NULL, ", \n")) != NULL)
            {
                varList_t *cursor = SymTable_getNode(var);
                
                if (cursor != NULL)
                {
                    var_pos++;

                    if (cursor->regName != 0)
                    {
                        char temp[20];

                        if (var_pos == 3)
                        {
                            sprintf(temp, " r%d\n", cursor->regName);
                            strcat(out_line, temp);
                            fprintf(fp_out, out_line, "\n");
                        }   
                        else
                        {
                            sprintf(temp, " r%d,", cursor->regName);
                            strcat(out_line, temp);
                        }    
                    }
                    else
                    {
                        char extraCode[100];

                        if (var_pos == 3)
                        {
                            //this is destination register
                            char temp[20];
                            sprintf(temp, " f%d\n", reg_f[var_pos-1]);
                            sprintf(extraCode, "store f%d, @%s\n", reg_f[var_pos-1], var);
                            strcat(out_line, temp);
                            strcat(out_line, extraCode);
                            fprintf(fp_out, out_line, "");                            
                        }
                        else
                        {
                            char temp[20];
                            sprintf(temp, " f%d,", reg_f[var_pos-1]);
                            strcat(out_line, temp);
                            //this is source register
                            sprintf(extraCode, "loadi @%s, f%d\n", var, reg_f[var_pos-1] );
                            fprintf(fp_out, extraCode, "\n");
                        }
                    }
                }
                else
                {
                    printf("Unexpected error while generating reg map for %s at line %d!", op_code, line_count);
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            printf("Invalid op code %s at line %d!", op_code, line_count);
            exit(EXIT_FAILURE);
        }
    }
}

int main (int argc, char *argv[])
{
	if (argc != CMD_ARG_LEN)
	{
		printf("Invalid input parameters. Exiting!\n");
		exit(EXIT_FAILURE);
	}

	FILE *fp_in = fopen(argv[1], "r");
    FILE *fp_out = fopen("output.txt", "w");
    FILE *fp_report = fopen("top_dwn_reg_alloc_report.txt", "w");

    int numReg = strtol(argv[2], NULL, 10);
    int listLen;

	if ((fp_in == NULL) || (fp_out == NULL) || (fp_report == NULL))
	{
		printf("Error: couldn't open file\n");
		exit(EXIT_FAILURE);
	}

    RegAlloc_GenerateVarList(fp_in);

    MergeSort(&pHead);

    listLen = SymTable_getLen();

    if (listLen > numReg)
    {
        reg_f[0] = numReg - 2;
        reg_f[1] = numReg - 1;
        reg_f[2] = numReg;
        numReg -= 3;

        fprintf(fp_report, "Registers assigned for spilling = r%d, r%d and r%d\n", reg_f[0], reg_f[1], reg_f[2]);
    } 

    fprintf(fp_report, "Registers available for general purpose use: ");
    for (int i = 1; i <= numReg; i++)
    {
        fprintf(fp_report, "r%d, ", i);
    }
    fprintf(fp_report, "\n\n");

    RegAlloc_AllocReg(pHead, numReg);

    SymTable_traverseList(fp_report);

    RegAlloc_generateCode(pHead, fp_in, fp_out);

    SymTable_freeList();

	fclose(fp_in);
    fclose(fp_out);
    fclose(fp_report);

    exit(EXIT_SUCCESS);
}
