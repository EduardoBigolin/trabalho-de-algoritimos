#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 80
#define CN 10

#define OK 0
#define ERROR (-1)
#define ERR_ALREADY_IN_GROUP 4

typedef struct Alunos {
    char nome[N];
    int idade;

    struct Alunos *next;
    struct Alunos *prev;
} ALUNOS;

typedef struct Grupos {
    char nome[N];
    int numeroDeAlunos;

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
    int numeroDeTurma;

    TURMA *startTurma;
    TURMA *endTurma;
} DISCIPLINAS;

typedef struct Escola {
    char nome[N];
    int numeroDeDisciplina;
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

    getchar();
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\r\n")] = 0;
    strcpy(text, str);
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

    printf("         - Grupo: %s (Alunos: %d)\n", grupo->nome, grupo->numeroDeAlunos);
    aluno = grupo->start;
    while (aluno != NULL) {
        printf("             - %s (idade: %d)\n", aluno->nome, aluno->idade);
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

    printf("      Alunos sem grupos (total: %d):\n", turma->numeroAlunosSemGrupo);

    ALUNOS *currAluno = turma->startAlunos;
    if (currAluno == NULL) {
        printf("         Nenhum aluno sem grupo.\n");
    } else {
        while (currAluno != NULL) {
            printf("         - %s (idade: %d)\n", currAluno->nome, currAluno->idade);
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
    printf("Total de disciplinas: %d\n", escola->numeroDeDisciplina);
    printf("========================================\n");

    DISCIPLINAS *currDisc = escola->startDisciplina;
    int countDisc = 1;

    while (currDisc != NULL) {
        printf("%d. Disciplina: %s\n", countDisc++, currDisc->nome);
        printf("   Total de turmas: %d\n", currDisc->numeroDeTurma);

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
    newDisciplina->numeroDeTurma = 0;
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

    escola->numeroDeDisciplina++;
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

    disciplina->numeroDeTurma++;
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
    newGrupo->numeroDeAlunos = 0;
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

    // Remove aluno da lista geral
    if (aluno->prev) aluno->prev->next = aluno->next;
    else turma->startAlunos = aluno->next;

    if (aluno->next) aluno->next->prev = aluno->prev;
    else turma->endAlunos = aluno->prev;

    aluno->next = NULL;
    aluno->prev = NULL;

    turma->numeroAlunosSemGrupo--;

    // Adiciona no grupo
    if (!grupo->end) {
        grupo->start = aluno;
        grupo->end = aluno;
    } else {
        grupo->end->next = aluno;
        aluno->prev = grupo->end;
        grupo->end = aluno;
    }

    grupo->numeroDeAlunos++;
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
    getchar();
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
    getTextInput(nome);

    printf("%s\n", nome);

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
    getTextInput(nome);

    if (addGrupoTurma(turma, nome) == ERROR) {
        return ERROR;
    }

    return OK;
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
int menu_checkAlunoInGroup(TURMA *turma, char *nomeAluno) {
    GRUPOS *grupoCheck = turma->startGrupos;
    while (grupoCheck) {
        ALUNOS *grupoAluno = grupoCheck->start;
        while (grupoAluno) {
            if (strcmp(grupoAluno->nome, nomeAluno) == 0) {
                printf("Aluno '%s' já está em um grupo.\n", nomeAluno);
                return ERR_ALREADY_IN_GROUP;
            }
            grupoAluno = grupoAluno->next;
        }
        grupoCheck = grupoCheck->next;
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
    getTextInput(nomeAluno);

    // Check if the student is already in any group
    if (menu_checkAlunoInGroup(turma, nomeAluno) == ERR_ALREADY_IN_GROUP) {
        return ERR_ALREADY_IN_GROUP;
    }

    listGrupos(turma);
    printf("Digite o nome do grupo:\n");
    getTextInput(nomeGrupo);

    addAlunoToGrupo(turma, nomeAluno, nomeGrupo);

    return OK;
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
            switch(menu_addAlunoToGrupo(escola)) {
                case ERR_ALREADY_IN_GROUP:
                    strcpy(msg, "Aluno ja pertence a um grupo");
                    break;
                case ERROR:
                    strcpy(msg, "Aluno nao encontrado");
                default:
            }
        menu(escola, msg);
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
    ESCOLA escola = {.numeroDeDisciplina = 0, .startDisciplina = NULL, .endDisciplina = NULL};
    strcpy(escola.nome, "Universidade de Caxias do Sul");

    createTestData(&escola);

    return menu(&escola, "");
}
