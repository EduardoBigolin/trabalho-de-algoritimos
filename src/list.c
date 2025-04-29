#include "utils.c"


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