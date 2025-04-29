#include <stdio.h>
#include <string.h>
#include "structs.c"

/**
 * @brief Reads a line of text input from the user, removing trailing newline or carriage return,
 *        and stores it in the provided buffer.
 *
 * @param text Pointer to the character array where the user's input will be stored.
 *             It must have sufficient space to store the input text.
 */
void getTextInput(char *text) {
    char str[N];

    printf("");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\r\n")] = 0;
    strcpy(text, str);
}


/**
 * @brief Checks if a specified student is already a member of any group within a given class (turma).
 *
 * @param turma Pointer to the TURMA structure, representing the class being checked.
 * @param nomeAluno Pointer to a character array containing the name of the student to be checked.
 *
 * @return ERR_ALREADY_IN_GROUP if the student is found in any group,
 *         OK if the student is not in any group.
 *
 * @note This function assumes the structure of TURMA, GRUPOS, and ALUNOS
 *       is well-defined and that the pointers within the structures are properly initialized.
 */
int checkAlunoInGroup(TURMA *turma, char *nomeAluno) {
    GRUPOS *grupoCheck = turma->startGrupos;
    while (grupoCheck) {
        ALUNOS *grupoAluno = grupoCheck->start;
        while (grupoAluno) {
            if (strcmp(grupoAluno->nome, nomeAluno) == 0) {
                return ERR_ALREADY_IN_GROUP;
            }
            grupoAluno = grupoAluno->next;
        }
        grupoCheck = grupoCheck->next;
    }
    return OK;
}