#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 80
#define CN 10

#define OK 0
#define ERROR (-1)
#define ERR_ALREADY_IN_GROUP 4
#define ERR_NOT_IN_GROUP 4

typedef struct Alunos {
    char nome[N];
    int idade;
    int numeroTurmas;

    struct Alunos *next;
    struct Alunos *prev;
} ALUNOS;

typedef struct Grupos {
    char nome[N];
    int numeroAlunos;

    struct Grupos *next;
    struct Grupos *prev;

    ALUNOS *start;
    ALUNOS *end;
} GRUPOS;

typedef struct Turma {
    char codigo[CN];
    int numeroGrupo;
    int numeroAlunosSemGrupo;
    int numeroAlunosTotal;

    ALUNOS *startAlunos;
    ALUNOS *endAlunos;

    struct Turma *next;
    struct Turma *prev;

    GRUPOS *startGrupos;
    GRUPOS *endGrupos;
} TURMA;

typedef struct Disciplina {
    struct Disciplina *next;
    struct Disciplina *prev;

    char nome[N];
    int numeroTurmas;

    TURMA *startTurma;
    TURMA *endTurma;
} DISCIPLINAS;

typedef struct Escola {
    char nome[N];
    int numeroDisciplinas;
    int numeroAlunosTotal;

    ALUNOS *startAlunos;
    ALUNOS *endAlunos;

    DISCIPLINAS *startDisciplina;
    DISCIPLINAS *endDisciplina;
} ESCOLA;

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

/**
 * @brief Displays the details of a group, including its name, the total number of students,
 *        and a list of all students in the group with their respective names and ages.
 *
 * @param grupo Pointer to a GRUPOS structure containing the group information to be displayed.
 *              It must be non-NULL.
 */
void listGrupo(GRUPOS *grupo) {
    ALUNOS *aluno;

    printf("         - Grupo: %s (Alunos: %d)\n", grupo->nome, grupo->numeroAlunos);
    aluno = grupo->start;
    while (aluno != NULL) {
        printf("             - %s (idade: %d)\n", aluno->nome, aluno->idade);
        aluno = aluno->next;
    }
}

void listGrupoSimple(GRUPOS *grupo) {
    ALUNOS *aluno;

    printf("    - %s (Alunos: %d)\n", grupo->nome, grupo->numeroAlunos);
    aluno = grupo->start;
    while (aluno != NULL) {
        printf("        - %s\n", aluno->nome);
        aluno = aluno->next;
    }
}

/**
 * @brief Lists all groups within a class (turma), displaying their details including the total
 *        number of groups. If there are no groups, an appropriate message is displayed.
 *
 * @param turma Pointer to a TURMA structure representing the class whose groups are to be listed.
 *              It must be non-NULL*/
void listGrupos(TURMA *turma) {
    GRUPOS *currGrupo = turma->startGrupos;
    printf("      Grupos (total: %d):\n", turma->numeroGrupo);
    if (currGrupo == NULL) {
        printf("         Nenhum grupo cadastrado.\n");
    } else {
        while (currGrupo != NULL) {
            listGrupo(currGrupo);
            currGrupo = currGrupo->next;
        }
    }
    printf("\n");
}

void listGruposSimple(TURMA *turma) {
    GRUPOS *currGrupo = turma->startGrupos;
    if (currGrupo == NULL) {
        printf("         Nenhum grupo cadastrado.\n");
    } else {
        while (currGrupo != NULL) {
            listGrupoSimple(currGrupo);
            currGrupo = currGrupo->next;
        }
    }
    printf("\n");
}

/**
 * @brief Lists details of a specific class (turma), including the total number of students,
 *        students without a group, and groups within the class.
 *
 * @param turma Pointer to a TURMA structure representing the class whose details are to be listed.
 *              It must be non-NULL.
 */
void listTurma(TURMA *turma) {
    printf("   -> Turma Codigo: %s\n      Numero total de alunos: %d\n", turma->codigo,
           turma->numeroAlunosTotal);

    printf("      Alunos sem grupo (total: %d):\n", turma->numeroAlunosSemGrupo);

    ALUNOS *currAluno = turma->startAlunos;
    if (currAluno == NULL) {
        printf("         Nenhum aluno sem grupo.\n");
    } else {
        while (currAluno != NULL) {
            if (checkAlunoInGroup(turma, currAluno->nome) == OK) {
                printf("         - %s (idade: %d)\n", currAluno->nome, currAluno->idade);
            }
            currAluno = currAluno->next;
        }
    }

    listGrupos(turma);
}

/**
 * @brief Lists all disciplines and their associated classes (turmas) in the given school (escola).
 *
 * @param escola Pointer to an ESCOLA structure containing the list of disciplines. It must be non-NULL.
 *               If no disciplines are registered, a message will be displayed indicating that no disciplines are available.
 */
void listAll(ESCOLA *escola) {
    if (escola->startDisciplina == NULL) {
        printf("Nenhuma disciplina cadastrada.\n");
        return;
    }

    printf("Escola: %s\n", escola->nome);
    printf("Total de disciplinas: %d\n", escola->numeroDisciplinas);
    printf("========================================\n");

    DISCIPLINAS *currDisc = escola->startDisciplina;
    int countDisc = 1;

    while (currDisc != NULL) {
        printf("%d. Disciplina: %s\n", countDisc++, currDisc->nome);
        printf("   Total de turmas: %d\n", currDisc->numeroTurmas);

        TURMA *currTurma = currDisc->startTurma;
        if (currTurma == NULL) {
            printf("   Nenhuma turma cadastrada.\n");
        } else {
            while (currTurma != NULL) {
                listTurma(currTurma);
                currTurma = currTurma->next;
            }
        }

        printf("\n========================================\n");
        currDisc = currDisc->next;
    }
}

/**
 * @brief Lists all disciplines (DISCIPLINAS) registered in the ESCOLA.
 *
 * @param escola Pointer to the ESCOLA structure that contains the list of disciplines.
 *               Must be non-NULL. If no disciplines are registered, a message will
 *               be displayed indicating that no disciplines are available.
 */
void listDisciplinas(ESCOLA *escola) {
    if (escola->startDisciplina == NULL) {
        printf("Nenhuma disciplina cadastrada.\n");
        return;
    }

    DISCIPLINAS *currDisc = escola->startDisciplina;
    int countDisc = 1;

    printf("\n=========================================\n");
    while (currDisc != NULL) {
        printf("%d. %s\n", countDisc++, currDisc->nome);
        currDisc = currDisc->next;
    }
    printf("\n=========================================\n");
}

/**
 * @brief Lists all students (ALUNOS) registered in the ESCOLA (school).
 *
 * @param escola Pointer to the ESCOLA structure that contains the list of students.
 *               Must be non-NULL. If no students are registered, a message will
 *               be displayed indicating that no students are available.
 */
void listAlunos(ESCOLA *escola) {
    if (escola->startAlunos == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    ALUNOS *currAluno = escola->startAlunos;
    int countDisc = 1;

    printf("\n=========================================\n");
    while (currAluno != NULL) {
        printf("%d. %s\n", countDisc++, currAluno->nome);
        currAluno = currAluno->next;
    }
    printf("\n=========================================\n");
}

/**
 * @brief Lists students from the school, categorized by the number of classes they are enrolled in.
 *        Students are grouped into three categories: without any classes, in one class, and in multiple classes.
 *
 * @param escola Pointer to the ESCOLA structure containing the list of students
 *               and their respective enrollment details.
 */
void listAlunosExtended(ESCOLA *escola) {
    if (escola->startAlunos == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("Alunos sem turma:\n");
    ALUNOS *currAluno = escola->startAlunos;
    int countDisc = 1;
    while (currAluno != NULL) {
        if (currAluno->numeroTurmas == 0) {
            printf("% 5d. %s\n", countDisc++, currAluno->nome);
        }
        currAluno = currAluno->next;
    }
    if (countDisc == 1) {
        printf("     Nenhum aluno sem turma.\n");
    }

    printf("Alunos em apenas uma turma:\n");
    currAluno = escola->startAlunos;
    countDisc = 1;
    while (currAluno != NULL) {
        if (currAluno->numeroTurmas == 1) {
            printf("% 5d. %s\n", countDisc++, currAluno->nome);
        }
        currAluno = currAluno->next;
    }
    if (countDisc == 1) {
        printf("     Nenhum aluno em apenas uma turma.\n");
    }

    printf("Alunos em mais de uma turma:\n");
    currAluno = escola->startAlunos;
    countDisc = 1;
    while (currAluno != NULL) {
        if (currAluno->numeroTurmas > 1) {
            printf("% 5d. %s (%d turmas)\n", countDisc++, currAluno->nome, currAluno->numeroTurmas);
        }
        currAluno = currAluno->next;
    }
    if (countDisc == 1) {
        printf("     Nenhum aluno em mais de uma turma.\n");
    }
}

/**
 * @brief Lists all students (ALUNOS) in a given TURMA (class).
 *
 * @param turma Pointer to the TURMA structure that contains the list of students.
 *              Must be non-NULL. If no students are registered in the turma,
 *              a message will be printed indicating that no students exist.
 */
void listAlunosTurma(TURMA *turma) {
    if (turma->startAlunos == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    ALUNOS *currAluno = turma->startAlunos;
    int countDisc = 1;

    printf("\n=========================================\n");
    while (currAluno != NULL) {
        printf("%d. %s\n", countDisc++, currAluno->nome);
        currAluno = currAluno->next;
    }
    printf("\n=========================================\n");
}

/**
 * @brief Retrieves a DISCIPLINA (discipline) from the specified ESCOLA (school) by its index.
 *
 * @param escola Pointer to the ESCOLA structure that contains the list of disciplinas.
 *               Must be non-NULL.
 * @param index The index of the desired disciplina in the list. Indexing starts at 1.
 * @return Pointer to the DISCIPLINAS structure corresponding to the given index,
 *         or NULL if the index is out of bounds or if no disciplinas exist in the escola.
 */
DISCIPLINAS *getDisciplinaByIndex(ESCOLA *escola, int index) {
    if (escola->startDisciplina == NULL) {
        printf("Disciplina não encontrada.\n");
        return NULL;
    }
    DISCIPLINAS *currDisc = escola->startDisciplina;
    int i = 1;
    while (i != index && currDisc != NULL) {
        currDisc = currDisc->next;
        i++;
    }
    return currDisc;
}

/**
 * @brief Adds a new DISCIPLINA (discipline) to a specified ESCOLA (school).
 *
 * @param escola Pointer to the ESCOLA structure where the new disciplina
 *               will be added. Must be non-NULL.
 * @param nome Name of the new disciplina to be added. Must be a valid string
 *             that fits within the DISCIPLINAS::nome field.
 */
void addDisciplinaEscola(ESCOLA *escola, char *nome) {
    DISCIPLINAS *newDisciplina = (DISCIPLINAS *) malloc(sizeof(DISCIPLINAS));
    if (!newDisciplina) return;

    strcpy(newDisciplina->nome, nome);
    newDisciplina->numeroTurmas = 0;
    newDisciplina->startTurma = NULL;
    newDisciplina->endTurma = NULL;
    newDisciplina->next = NULL;
    newDisciplina->prev = NULL;

    if (escola->endDisciplina == NULL) {
        escola->startDisciplina = newDisciplina;
        escola->endDisciplina = newDisciplina;
    } else {
        DISCIPLINAS *aux = escola->endDisciplina;
        aux->next = newDisciplina;
        newDisciplina->prev = aux;
        escola->endDisciplina = newDisciplina;
    }

    escola->numeroDisciplinas++;
}

/**
 * @brief Adds a new TURMA (class) to a specified DISCIPLINAS (discipline).
 *
 * @param disciplina Pointer to the DISCIPLINAS structure where the turma will be added.
 *                   Must be non-NULL.
 * @param codigo Code of the new turma to be added. Must be a valid string that fits
 *               within the TURMA::codigo field.
 *
 * @return OK on success, or ERROR if memory allocation for the new turma fails.
 */
int addTurmaDisciplina(DISCIPLINAS *disciplina, char *codigo) {
    TURMA *newTurma = (TURMA *) malloc(sizeof(TURMA));
    if (!newTurma) return ERROR;

    strcpy(newTurma->codigo, codigo);
    newTurma->numeroAlunosTotal = 0;
    newTurma->numeroAlunosSemGrupo = 0;
    newTurma->numeroGrupo = 0;

    newTurma->startAlunos = NULL;
    newTurma->endAlunos = NULL;
    newTurma->startGrupos = NULL;
    newTurma->endGrupos = NULL;

    newTurma->next = NULL;
    newTurma->prev = NULL;

    if (disciplina->endTurma == NULL) {
        disciplina->startTurma = newTurma;
        disciplina->endTurma = newTurma;
    } else {
        TURMA *aux = disciplina->endTurma;
        aux->next = newTurma;
        newTurma->prev = aux;
        disciplina->endTurma = newTurma;
    }

    disciplina->numeroTurmas++;
    return OK;
}

/**
 * @brief Lists all available TURMA structures within a given DISCIPLINAS structure.
 *
 * @param disc Pointer to the DISCIPLINAS structure containing the list of turmas to be listed.
 *             If NULL or if the startTurma property is NULL, no turmas are listed.
 */
void listTurmas(DISCIPLINAS *disc) {
    if (disc == NULL || disc->startTurma == NULL) {
        printf("Nenhuma turma cadastrada.\n");
        return;
    }

    TURMA *currTurma = disc->startTurma;
    int countDisc = 1;

    while (currTurma != NULL) {
        printf("%d. %s\n", countDisc++, currTurma->codigo);
        currTurma = currTurma->next;
    }
    printf("\n=========================================\n");
}

/**
 * @brief Retrieves a TURMA structure by its index from a given DISCIPLINAS structure.
 *
 * @param disc Pointer to the DISCIPLINAS structure representing the discipline from which the turma is to be retrieved.
 * @param index An integer representing the position of the turma in the list, starting from 1.
 * @return A pointer to the TURMA structure if found, or NULL if the turma does not exist.
 */
TURMA *getTurmaByIndex(DISCIPLINAS *disc, int index) {
    if (disc->startTurma == NULL) {
        printf("Turma não encontrada.\n");
        return NULL;
    }
    TURMA *currTurma = disc->startTurma;
    int i = 1;
    while (i != index) {
        currTurma = currTurma->next;
        i++;
    }
    return currTurma;
}

/**
 * @brief Adds a new student to the school's list of students.
 *
 * @param escola Pointer to the ESCOLA structure representing the school where the student will be added.
 * @param nome A string containing the name of the student to be added.
 * @param idade An integer representing the age of the student to be added.
 * @return An integer value indicating success (OK) or failure (ERROR) of the operation.
 */
int addAlunoEscola(ESCOLA *escola, char *nome, int idade) {
    ALUNOS *newAluno = (ALUNOS *) malloc(sizeof(ALUNOS));
    if (!newAluno) return ERROR;

    strcpy(newAluno->nome, nome);
    newAluno->idade = idade;
    newAluno->numeroTurmas = 0;
    newAluno->next = NULL;
    newAluno->prev = NULL;

    if (escola->endAlunos == NULL) {
        escola->startAlunos = newAluno;
        escola->endAlunos = newAluno;
    } else {
        ALUNOS *aux = escola->endAlunos;
        aux->next = newAluno;
        newAluno->prev = aux;
        escola->endAlunos = newAluno;
    }

    escola->numeroAlunosTotal++;
    return OK;
}

/**
 * @brief Searches for a student in the school by their name.
 *
 * @param escola Pointer to the ESCOLA structure representing the school where the student is being searched.
 * @param nomeAluno A string containing the name of the student to search for.
 * @return A pointer to the ALUNOS structure representing the student if found, or NULL if the student does not exist in the school.
 */
ALUNOS *getAlunoEscola(ESCOLA *escola, char *nomeAluno) {
    ALUNOS *aluno = escola->startAlunos;
    while (aluno != NULL && strcmp(aluno->nome, nomeAluno) != 0) {
        aluno = aluno->next;
    }

    if (!aluno) {
        printf("Aluno %s nao encontrado na escola.\n", nomeAluno);
        return NULL;
    }
    return aluno;
}

/**
 * @brief Adds a student to a specific class.
 *
 * @param escola Pointer to the ESCOLA structure representing the school where the student and class are registered.
 * @param turma Pointer to the TURMA structure representing the class to which the student will be added.
 * @param nome A string containing the name of the student to be added to the class.
 * @return An integer indicating the success or failure of the operation. Returns OK on success or ERROR if the student does not exist or memory allocation fails.
 */
int addAlunoTurma(ESCOLA *escola, TURMA *turma, char *nome) {
    ALUNOS *alunoCopy = (ALUNOS *) malloc(sizeof(ALUNOS)), *aluno = getAlunoEscola(escola, nome);
    if (!aluno) return ERROR;

    strcpy(alunoCopy->nome, aluno->nome);
    alunoCopy->idade = aluno->idade;
    alunoCopy->next = NULL;
    alunoCopy->prev = NULL;

    if (turma->endAlunos == NULL) {
        turma->startAlunos = alunoCopy;
        turma->endAlunos = alunoCopy;
    } else {
        ALUNOS *aux = turma->endAlunos;
        aux->next = alunoCopy;
        alunoCopy->prev = aux;
        turma->endAlunos = alunoCopy;
    }
    aluno->numeroTurmas++;

    turma->numeroAlunosTotal++;
    turma->numeroAlunosSemGrupo++;
    return OK;
}

/**
 * @brief Adds a new group to a specific class.
 *
 * @param turma Pointer to the TURMA structure representing the class to which the group will be added.
 * @param nome A string containing the name of the group to be added.
 * @return An integer indicating the success or failure of the operation. Returns OK on success or ERROR on failure.
 */
int addGrupoTurma(TURMA *turma, char *nome) {
    GRUPOS *newGrupo = (GRUPOS *) malloc(sizeof(GRUPOS));
    if (!newGrupo) return ERROR;

    strcpy(newGrupo->nome, nome);
    newGrupo->numeroAlunos = 0;
    newGrupo->start = NULL;
    newGrupo->end = NULL;
    newGrupo->next = NULL;
    newGrupo->prev = NULL;

    if (turma->endGrupos == NULL) {
        turma->startGrupos = newGrupo;
        turma->endGrupos = newGrupo;
    } else {
        GRUPOS *aux = turma->endGrupos;
        aux->next = newGrupo;
        newGrupo->prev = aux;
        turma->endGrupos = newGrupo;
    }

    turma->numeroGrupo++;
    return OK;
}

/**
 * @brief Adds a student to a specific group within a class.
 *
 * @param turma Pointer to the TURMA structure representing the class where the operation will occur.
 * @param nomeAluno A string containing the name of the student to be added to the group.
 * @param nomeGrupo A string containing the name of the group to which the student will be added.
 */
void addAlunoToGrupo(TURMA *turma, char *nomeAluno, char *nomeGrupo) {
    ALUNOS *aluno = turma->startAlunos;
    while (aluno && strcmp(aluno->nome, nomeAluno) != 0)
        aluno = aluno->next;

    if (!aluno) {
        printf("Aluno '%s' não encontrado na turma.\n", nomeAluno);
        return;
    }

    GRUPOS *grupo = turma->startGrupos;
    while (grupo && strcmp(grupo->nome, nomeGrupo) != 0)
        grupo = grupo->next;

    if (!grupo) {
        printf("Grupo '%s' não encontrado na turma.\n", nomeGrupo);
        return;
    }

    ALUNOS *alunoCopy = (ALUNOS *) malloc(sizeof(ALUNOS));
    strcpy(alunoCopy->nome, aluno->nome);
    alunoCopy->idade = aluno->idade;
    alunoCopy->next = NULL;
    alunoCopy->prev = NULL;

    turma->numeroAlunosSemGrupo--;

    // Adiciona no grupo
    if (!grupo->end) {
        grupo->start = alunoCopy;
        grupo->end = alunoCopy;
    } else {
        grupo->end->next = alunoCopy;
        alunoCopy->prev = grupo->end;
        grupo->end = alunoCopy;
    }

    grupo->numeroAlunos++;
}

/**
 * @brief Removes a student from the group they belong to within the specified class, if the student is found.
 *
 * @param turma Pointer to the `TURMA` structure representing the class containing the groups to be searched.
 * @param nomeAluno Pointer to a null-terminated string containing the name of the student to be removed.
 *
 * @return Returns `OK` (0) if the student is successfully removed, or `ERROR` (-1) if the student
 *         is not found within any group in the class.
 */
int removeAlunoFromGrupo(TURMA *turma, char *nomeAluno) {
    if (checkAlunoInGroup(turma, nomeAluno) == ERR_ALREADY_IN_GROUP) {
        GRUPOS *grupoCheck = turma->startGrupos;
        while (grupoCheck != NULL) {
            ALUNOS *currAluno = grupoCheck->start;

            while (currAluno) {
                if (strcmp(currAluno->nome, nomeAluno) == 0) {
                    if (currAluno->prev) currAluno->prev->next = currAluno->next;
                    else grupoCheck->start = currAluno->next;

                    if (currAluno->next) currAluno->next->prev = currAluno->prev;
                    else grupoCheck->end = currAluno->prev;

                    grupoCheck->numeroAlunos--;
                    turma->numeroAlunosSemGrupo++;
                    free(currAluno);
                    break;
                }
                currAluno = currAluno->next;
            }
            grupoCheck = grupoCheck->next;
        }
        return OK;
    } else {
        return ERR_NOT_IN_GROUP;
    }
}

/**
 * @brief Removes a student from a given class (turma), including their associated group if necessary.
 *
 * @param turma Pointer to the TURMA structure representing the class from which the student
 *              should be removed. The structure contains information about students, groups,
 *              and class details.
 * @param nomeAluno Pointer to a character array containing the name of the student to be removed.
 *                  The name must match an entry in the class list or group list for removal.
 *
 * @return Returns OK if the student was successfully removed;
 *         ERROR if the student was not found in the class or any groups.
 */
int removeAlunoFromTurma(ESCOLA *escola, TURMA *turma, char *nomeAluno) {
    removeAlunoFromGrupo(turma, nomeAluno);
    turma->numeroAlunosSemGrupo--;

    TURMA *turmaCheck = turma;
    ALUNOS *currAluno = turmaCheck->startAlunos;

    while (currAluno) {
        if (strcmp(currAluno->nome, nomeAluno) == 0) {
            if (currAluno->prev) currAluno->prev->next = currAluno->next;
            else turma->startAlunos = currAluno->next;

            if (currAluno->next) currAluno->next->prev = currAluno->prev;
            else turma->endAlunos = currAluno->prev;

            free(currAluno);
            turmaCheck->numeroAlunosTotal--;

            ALUNOS *aluno = getAlunoEscola(escola, nomeAluno);
            aluno->numeroTurmas--;
            return OK;
        }
        currAluno = currAluno->next;
    }

    return ERROR;
}

/**
 * @brief Removes a group from a class (turma) identified by its name.
 *
 * @param turma Pointer to the class (TURMA) from which the group will be removed.
 *              The class contains a doubly linked list of groups.
 * @param nomeGrupo Pointer to the name of the group to be removed.
 *                  This is matched against the group names in the class.
 *
 * @return OK (0) if the group is successfully removed, or ERROR (-1) if the group
 *         is not found or the list of groups is empty.
 */
int removeGrupoFromTurma(TURMA *turma, char *nomeGrupo) {
    if (turma->startGrupos == NULL) {
        return ERROR;
    }

    GRUPOS *currGrupo = turma->startGrupos;

    while (currGrupo != NULL) {
        if (strcmp(currGrupo->nome, nomeGrupo) == 0) {
            if (currGrupo->prev) currGrupo->prev->next = currGrupo->next;
            else turma->startGrupos = currGrupo->next;

            if (currGrupo->next) currGrupo->next->prev = currGrupo->prev;
            else turma->endGrupos = currGrupo->prev;

            free(currGrupo);
            turma->numeroGrupo--;
            return OK;
        }
        currGrupo = currGrupo->next;
    }

    return ERROR;
}

/**
 * @brief Recursively removes a class (turma) identified by its code from the given discipline.
 *        Frees all associated resources, including students and groups within the class.
 *
 * @param disc Pointer to the structure representing the discipline containing the class to be removed.
 *             It must not be NULL and should be properly initialized.
 * @param cod Pointer to a string containing the code of the class to be removed.
 *            It must not be NULL and should match the code of an existing class within the discipline.
 *
 * @return Returns OK (0) if the class was successfully removed, or ERROR (-1) if the class with the specified code
 *         was not found or the discipline is invalid.
 */
int removeTurmaRecursive(ESCOLA *escola, DISCIPLINAS *disc, char *cod) {
    if (disc == NULL || disc->startTurma == NULL) {
        return ERROR;
    }

    TURMA *currTurma = disc->startTurma;

    while (currTurma != NULL) {
        if (strcmp(currTurma->codigo, cod) == 0) {
            if (currTurma->startAlunos != NULL) {
                ALUNOS *nextAluno, *currAluno = currTurma->startAlunos;

                while (currAluno != NULL) {
                    printf("Removendo %s\n", currAluno->nome);
                    nextAluno = currAluno->next;
                    removeAlunoFromTurma(escola, currTurma, (currAluno->nome));
                    currAluno = nextAluno;
                }
            }
            if (currTurma->startGrupos != NULL) {
                GRUPOS *nextGrupo, *currGrupo = currTurma->startGrupos;

                while (currGrupo != NULL) {
                    nextGrupo = currGrupo->next;
                    removeGrupoFromTurma(currTurma, currGrupo->nome);
                    currGrupo = nextGrupo;
                }
            }
            if (currTurma->prev) currTurma->prev->next = currTurma->next;
            else disc->startTurma = currTurma->next;

            if (currTurma->next) currTurma->next->prev = currTurma->prev;
            else disc->endTurma = currTurma->prev;
            free(currTurma);
            disc->numeroTurmas--;
            return OK;
        }
        currTurma = currTurma->next;
    }

    return ERROR;
}

/**
 * @brief Displays a list of all disciplines in the school and allows the user to select one.
 *
 * @param escola Pointer to the ESCOLA structure representing the school,
 *               which contains disciplines to be listed.
 * @param msg A message to display when prompting the user to select a discipline.
 *
 * @return The index of the selected discipline.
 */
int menu_selectDisciplina(ESCOLA *escola, char *msg) {
    int idx;

    listDisciplinas(escola);

    printf("%s: \n", msg);
    scanf("%d", &idx);

    return idx;
}

/**
 * @brief Selects a specific "turma" (class) from the school.
 *
 * @param escola Pointer to the ESCOLA structure representing the school,
 *               which contains disciplines and "turmas".
 * @param msg A message to display when prompting the user to select a "turma".
 * @param discIdx Pointer to an integer where the selected discipline index
 *                will be stored.
 *
 * @return The index of the selected "turma".
 */
int menu_selectTurma(ESCOLA *escola, char *msg, int *discIdx) {
    int idx;

    *discIdx = menu_selectDisciplina(escola, "Digite o numero da disciplina que a turma pertence");

    listTurmas(getDisciplinaByIndex(escola, *discIdx));

    printf("%s \n", msg);
    scanf("%d", &idx);

    return idx;
}

/**
 * @brief Adds a new discipline to the school.
 *
 * @param escola Pointer to the ESCOLA structure representing the school,
 *               which contains the current disciplines and related data.
 */
void menu_addDisciplina(ESCOLA *escola) {
    char nomeDisciplina[N];
    printf("Digite o nome da disciplina: \n");
    getTextInput(nomeDisciplina);
    addDisciplinaEscola(escola, nomeDisciplina);
}

/**
 * @brief Adds a new class (turma) to a specific discipline in the school.
 *
 * @param escola Pointer to the ESCOLA structure representing the school,
 *               which contains the disciplines, classes, and students.
 *
 * @return OK if the class is successfully added,
 *         ERROR if an error occurs during the addition process.
 */
int menu_addTurma(ESCOLA *escola) {
    int idx;
    char codTurma[CN];

    idx = menu_selectDisciplina(escola, "Digite o numero da disciplina que deseja adicionar a turma");

    printf("Digite o codigo da turma (Ex: 101, 204): \n");
    getchar();
    getTextInput(codTurma);

    return addTurmaDisciplina(getDisciplinaByIndex(escola, idx), codTurma);
}

/**
 * @brief Adds a new student to the school.
 *
 * @param escola Pointer to the ESCOLA structure representing the school,
 *               which contains disciplines, classes, and students.
 *
 * @return OK if the student is successfully added,
 *         ERROR if an error occurs during the addition process.
 */
int menu_addAluno(ESCOLA *escola) {
    char nome[N];
    int idade;

    printf("Digite o nome do aluno:\n");
    getTextInput(nome);

    printf("Digite a idade do aluno:\n");
    scanf("%d", &idade);

    return addAlunoEscola(escola, nome, idade);
}

/**
 * @brief Adds a student to an existing class (turma) within a discipline.
 *
 * @param escola Pointer to the ESCOLA structure representing the school, which contains disciplines, classes, and students.
 *
 * @return OK if the student is successfully added to the class,
 *         ERROR if an error occurs during the addition process (e.g., student not found).
 */
int menu_addAlunoToTurma(ESCOLA *escola) {
    int idx, discIdx;
    char nome[N];

    idx = menu_selectTurma(escola, "Digite o numero da turma que deseja adicionar o aluno", &discIdx);

    TURMA *turma = getTurmaByIndex(getDisciplinaByIndex(escola, discIdx), idx);

    listAlunos(escola);
    printf("Digite o nome do aluno:\n");
    getchar();
    getTextInput(nome);

    if (addAlunoTurma(escola, turma, nome) == ERROR) {
        return ERROR;
    }

    return OK;
}

/**
 * @brief Adds a new group to a specified class (turma) within a discipline in the school.
 *
 * @param escola Pointer to the ESCOLA structure, representing the school containing the disciplines and classes.
 *
 * @return OK if the group is successfully added,
 *         ERROR if an error occurs during the addition process (e.g., duplicate group name).
 */
int menu_addGrupo(ESCOLA *escola) {
    int idx, discIdx;
    char nome[N];

    idx = menu_selectTurma(escola, "Digite o numero da turma que deseja adicionar o grupo", &discIdx);

    TURMA *turma = getTurmaByIndex(getDisciplinaByIndex(escola, discIdx), idx);

    printf("Digite o nome do grupo:\n");
    getchar();
    getTextInput(nome);

    if (addGrupoTurma(turma, nome) == ERROR) {
        return ERROR;
    }

    return OK;
}

/**
 * @brief Adds a student to a group within a selected class (turma).
 *
 * @param escola Pointer to the ESCOLA structure representing the school, which holds disciplines, classes, and students.
 *
 * @return OK if the student is successfully added to the group,
 *         ERR_ALREADY_IN_GROUP if the student is already part of a group,
 *         ERROR if an error occurs during the process (e.g., student or class not found).
 */
int menu_addAlunoToGrupo(ESCOLA *escola) {
    int turmaIdx, discIdx;
    char nomeAluno[N], nomeGrupo[N];

    turmaIdx = menu_selectTurma(escola, "Digite o numero da turma onde o aluno e o grupo estão", &discIdx);

    DISCIPLINAS *disc = getDisciplinaByIndex(escola, discIdx);
    if (!disc) return ERROR;

    TURMA *turma = getTurmaByIndex(disc, turmaIdx);
    if (!turma) return ERROR;

    listAlunosTurma(turma);
    printf("Digite o nome do aluno:\n");
    getchar();
    getTextInput(nomeAluno);
    printf("");

    if (checkAlunoInGroup(turma, nomeAluno) == ERR_ALREADY_IN_GROUP) {
        return ERR_ALREADY_IN_GROUP;
    }

    listGruposSimple(turma);
    printf("Digite o nome do grupo:\n");
    getTextInput(nomeGrupo);

    addAlunoToGrupo(turma, nomeAluno, nomeGrupo);

    return OK;
}

/**
 * @brief Displays a menu to list students either from a specific class or from the entire school.
 *        Allows the user to choose between simple or extended listing for the school overview.
 *
 * @param escola Pointer to the school structure containing information about disciplines
 *               and students.
 * @return Returns an integer status code (e.g., OK if the operation completes successfully).
 */
int menu_listAlunoFromTurma(ESCOLA *escola) {
    int idx, discIdx;
    char choice[N];
    printf("Voce deseja listar alunos de uma turma ou da escola? (E/t):\n");
    scanf("%s", choice);

    if (choice[0] == 't' || choice[0] == 'T') {
        idx = menu_selectTurma(escola, "Digite o numero da turma que deseja Listar os alunos", &discIdx);
        TURMA *turma = getTurmaByIndex(getDisciplinaByIndex(escola, discIdx), idx);

        listAlunosTurma(turma);
    } else {
        printf("Voce deseja ver a listagem simples ou extendida? (S/e)\n");
        scanf("%s", choice);
        if (choice[0] == 'e' || choice[0] == 'E') {
            listAlunosExtended(escola);
        } else {
            listAlunos(escola);
        }
    }

    printf("Pressione Enter para voltar ao menu: \n");
    getchar();
    getchar();
    return OK;
}


/**
 * @brief Removes a student from a specific class in the school system.
 *
 * @param escola Pointer to the ESCOLA structure containing all registered disciplines,
 *               classes, and students in the school system.
 *
 * @return OK on successful removal of the student from the class.
 *         ERROR in case of an error during the removal process.
 */
int menu_removeAlunoFromTurma(ESCOLA *escola) {
    int idx, discIdx;
    char nome[N];

    idx = menu_selectTurma(escola, "Digite o numero da turma que deseja remover o aluno", &discIdx);

    TURMA *turma = getTurmaByIndex(getDisciplinaByIndex(escola, discIdx), idx);

    listAlunosTurma(turma);
    printf("Digite o nome do aluno:\n");
    getchar();
    getTextInput(nome);
    printf("nome: %s", nome);

    return removeAlunoFromTurma(escola, turma, nome);
}

/**
 * @brief Removes a student from a group in the specified school.
 *
 * @param escola A pointer to the ESCOLA structure that contains information
 *               about the school, including its classes, disciplines, and students.
 *
 * @return An integer indicating the result of the operation. Returns OK (0)
 *         if the student was successfully removed, or ERROR (-1) if the operation failed.
 */
int menu_removeAlunoFromGrupo(ESCOLA *escola) {
    int idx, discIdx;
    char nome[N];

    idx = menu_selectTurma(escola, "Digite o numero da turma do grupo que deseja remover o aluno", &discIdx);

    TURMA *turma = getTurmaByIndex(getDisciplinaByIndex(escola, discIdx), idx);

    listAlunosTurma(turma);
    printf("Digite o nome do aluno:\n");
    getchar();
    getTextInput(nome);

    return removeAlunoFromGrupo(turma, nome);
}

/**
 * @brief Removes a specific class (turma) from the school (escola) based on user selection.
 *
 * @param escola Pointer to the ESCOLA structure representing the school, which contains
 *               all relevant data including disciplines and classes.
 * @return Returns an integer indicating success or failure of the operation. Typically, a defined
 *         constant like ERROR is used to indicate failure.
 */
int menu_removeTurma(ESCOLA *escola) {
    int idx, discIdx;
    idx = menu_selectTurma(escola, "Digite o numero da turma que deseja remover", &discIdx);

    DISCIPLINAS *disc = getDisciplinaByIndex(escola, discIdx);
    TURMA *turma = getTurmaByIndex(disc, idx);

    return removeTurmaRecursive(escola, disc, turma->codigo);
}

/**
 * @brief Displays the main menu for managing a school, allowing the user to perform various operations such as creating disciplines, groups, and adding students.
 *
 * @param escola Pointer to the ESCOLA structure representing the school, which contains information about disciplines, classes, students, and groups.
 * @param errorMsg Pointer to a string containing an error message to be displayed if an error occurred in a previous operation.
 *
 * @return OK if the menu executes successfully and the user exits,
 *         ERROR if any unhandled error occurs during the menu execution.
 */
int menu(ESCOLA *escola, char *errorMsg) {
    char msg[N] = "";

    listAll(escola);

    if (strcmp(errorMsg, "") != 0) {
        printf("Erro ao executar a operacao: %s\n", errorMsg);
    }

    printf("Menu:\n");
    printf("1. Criar Disciplina\n");
    printf("2. Criar Turma\n");
    printf("3. Criar Aluno\n");
    printf("4. Adicionar Aluno em Turma\n");
    printf("5. Criar Grupo em Turma\n");
    printf("6. Adicionar Aluno em Grupo\n");
    printf("7. Remover Aluno de Turma\n");
    printf("8. Remover Aluno de Grupo\n");
    printf("9. Listar Alunos (Turma e Escola)\n");
    printf("0. Remover Turma\n");

    printf("Digite o numero correspontente com a operacao desejada (1, 2, 3, 4...) ou \"s\" para sair: \n");
    int choice = getchar();
    fflush(stdin);
    printf("\n");

    switch (choice) {
        case '1':
            menu_addDisciplina(escola);
            menu(escola, "");
            break;
        case '2':
            if (menu_addTurma(escola) == ERROR) {
                strcpy(msg, "Turma ja existe");
            }
            menu(escola, msg);
            break;
        case '3':
            if (menu_addAluno(escola) == ERROR) {
                strcpy(msg, "Aluno ja existe");
            }
            menu(escola, msg);
            break;
        case '4':
            if (menu_addAlunoToTurma(escola) == ERROR) {
                strcpy(msg, "Aluno nao encontrado");
            }
            menu(escola, msg);
            break;
        case '5':
            if (menu_addGrupo(escola) == ERROR) {
                strcpy(msg, "Grupo ja existe");
            }
            menu(escola, msg);
            break;
        case '6':
            switch (menu_addAlunoToGrupo(escola)) {
                case ERR_ALREADY_IN_GROUP:
                    strcpy(msg, "Aluno ja pertence a um grupo");
                    break;
                case ERROR:
                    strcpy(msg, "Aluno nao encontrado");
                default:
                    strcpy(msg, "");
            }
            menu(escola, msg);
        case '7':
            if (menu_removeAlunoFromTurma(escola) == ERROR) {
                strcpy(msg, "Aluno nao encontrado");
            }
            menu(escola, msg);
            break;
        case '8':
            int resp = menu_removeAlunoFromGrupo(escola);
            if (resp == ERROR) {
                strcpy(msg, "Aluno nao encontrado");
            } else if (resp == ERR_NOT_IN_GROUP) {
                strcpy(msg, "Aluno nao pertence a nenhum grupo");
            }
            menu(escola, msg);
            break;
        case '9':
            menu_listAlunoFromTurma(escola);
            menu(escola, msg);
            break;
        case '0':
            if (menu_removeTurma(escola) == ERROR) {
                strcpy(msg, "Turma nao encontrada");
            }
            menu(escola, msg);
            break;
        case 's':
            printf("Saindo do menu.\n");
            return OK;
        default:
            menu(escola, "Operacao invalida");
    }
}

/**
 * @brief Creates test data for the school (escola), including students, disciplines, classes, and groups.
 *
 * @param escola Pointer to the ESCOLA structure representing the school where test data will be created.
 */
void createTestData(ESCOLA *escola) {
    addAlunoEscola(escola, "Eduardo", 19);
    addAlunoEscola(escola, "Ana", 20);
    addAlunoEscola(escola, "Julio", 20);
    addAlunoEscola(escola, "Marcos", 22);

    addDisciplinaEscola(escola, "Matematica");
    addTurmaDisciplina(getDisciplinaByIndex(escola, 1), "FM10001-A");
    addAlunoTurma(escola, getTurmaByIndex(getDisciplinaByIndex(escola, 1), 1), "Eduardo");
    addAlunoTurma(escola, getTurmaByIndex(getDisciplinaByIndex(escola, 1), 1), "Julio");
    addAlunoTurma(escola, getTurmaByIndex(getDisciplinaByIndex(escola, 1), 1), "Marcos");
    addGrupoTurma(getTurmaByIndex(getDisciplinaByIndex(escola, 1), 1), "Grupo 01");
    addGrupoTurma(getTurmaByIndex(getDisciplinaByIndex(escola, 1), 1), "Grupo 02");
    addAlunoToGrupo(getTurmaByIndex(getDisciplinaByIndex(escola, 1), 1), "Eduardo", "Grupo 01");
    addAlunoToGrupo(getTurmaByIndex(getDisciplinaByIndex(escola, 1), 1), "Julio", "Grupo 01");
    addTurmaDisciplina(getDisciplinaByIndex(escola, 1), "FM10001-B");

    addDisciplinaEscola(escola, "Portugues");
    addTurmaDisciplina(getDisciplinaByIndex(escola, 2), "FP30002-A");
    addAlunoTurma(escola, getTurmaByIndex(getDisciplinaByIndex(escola, 2), 1), "Ana");
    addAlunoTurma(escola, getTurmaByIndex(getDisciplinaByIndex(escola, 2), 1), "Julio");
    addGrupoTurma(getTurmaByIndex(getDisciplinaByIndex(escola, 2), 1), "Grupo 01");
    addTurmaDisciplina(getDisciplinaByIndex(escola, 2), "FP30007-B");
}

int main() {
    ESCOLA escola = {.numeroDisciplinas = 0, .startDisciplina = NULL, .endDisciplina = NULL};
    strcpy(escola.nome, "Universidade de Caxias do Sul");

    createTestData(&escola);

    return menu(&escola, "");
}
