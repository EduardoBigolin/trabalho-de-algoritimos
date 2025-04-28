#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 80
#define CN 10

#define OK 0
#define ERROR -1

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

void getTextInput(char *text) {
    char str[N];

    getchar();
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\r\n")] = 0;
    strcpy(text, str);
}

void listGrupo(GRUPOS *grupo) {
    ALUNOS *aluno;

    printf("         - Grupo: %s (Alunos: %d)\n", grupo->nome, grupo->numeroDeAlunos);
    aluno = grupo->start;
    while (aluno != NULL) {
        printf("             - %s (idade: %d)\n", aluno->nome, aluno->idade);
        aluno = aluno->next;
    }
}

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

void addGrupoTurma(TURMA *turma, char *nome) {
    GRUPOS *newGrupo = (GRUPOS *) malloc(sizeof(GRUPOS));
    if (!newGrupo) return;

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
}

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

int menu_selectDisciplina(ESCOLA *escola, char *msg) {
    int idx;

    listDisciplinas(escola);

    printf("%s: \n", msg);
    scanf("%d", &idx);

    return idx;
}

int menu_selectTurma(ESCOLA *escola, char *msg, int *discIdx) {
    int idx;

    *discIdx = menu_selectDisciplina(escola, "Digite o numero da disciplina que a turma pertence");

    listTurmas(getDisciplinaByIndex(escola, *discIdx));

    printf("%s \n", msg);
    scanf("%d", &idx);

    return idx;
}

void menu_addDisciplina(ESCOLA *escola) {
    char nomeDisciplina[N];
    printf("Digite o nome da disciplina: \n");
    getchar();
    getTextInput(nomeDisciplina);
    addDisciplinaEscola(escola, nomeDisciplina);
}

int menu_addTurma(ESCOLA *escola) {
    int idx;
    char codTurma[CN];

    idx = menu_selectDisciplina(escola, "Digite o numero da disciplina que deseja adicionar a turma");

    printf("Digite o codigo da turma (Ex: 101, 204): \n");
    getTextInput(codTurma);

    return addTurmaDisciplina(getDisciplinaByIndex(escola, idx), codTurma);
}

int menu_addAluno(ESCOLA *escola) {
    char nome[N];
    int idade;

    printf("Digite o nome do aluno:\n");
    getTextInput(nome);

    printf("Digite a idade do aluno:\n");
    scanf("%d", &idade);

    return addAlunoEscola(escola, nome, idade);
}

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
        case 's':
            printf("Saindo do menu.\n");
            return OK;
        default:
            menu(escola, "Operacao invalida");
    }
}

int main() {
    ESCOLA escola = {.numeroDeDisciplina = 0, .startDisciplina = NULL, .endDisciplina = NULL};
    strcpy(escola.nome, "Universidade de Caxias do Sul");

    //Cria dados de teste
    addAlunoEscola(&escola, "Eduardo", 19);
    addAlunoEscola(&escola, "Ana", 20);
    addAlunoEscola(&escola, "Julio", 20);
    addAlunoEscola(&escola, "Marcos", 22);

    addDisciplinaEscola(&escola, "Matematica");
    addTurmaDisciplina(getDisciplinaByIndex(&escola, 1), "FM10001-A");
    addAlunoTurma(&escola, getTurmaByIndex(getDisciplinaByIndex(&escola, 1), 1), "Eduardo");
    addAlunoTurma(&escola, getTurmaByIndex(getDisciplinaByIndex(&escola, 1), 1), "Julio");
    addAlunoTurma(&escola, getTurmaByIndex(getDisciplinaByIndex(&escola, 1), 1), "Marcos");
    addGrupoTurma(getTurmaByIndex(getDisciplinaByIndex(&escola, 1), 1), "Grupo 01");
    addGrupoTurma(getTurmaByIndex(getDisciplinaByIndex(&escola, 1), 1), "Grupo 02");
    addAlunoToGrupo(getTurmaByIndex(getDisciplinaByIndex(&escola, 1), 1), "Eduardo", "Grupo 01");
    addAlunoToGrupo(getTurmaByIndex(getDisciplinaByIndex(&escola, 1), 1), "Julio", "Grupo 01");
    addTurmaDisciplina(getDisciplinaByIndex(&escola, 1), "FM10001-B");

    addDisciplinaEscola(&escola, "Portugues");
    addTurmaDisciplina(getDisciplinaByIndex(&escola, 2), "FP30002-A");
    addAlunoTurma(&escola, getTurmaByIndex(getDisciplinaByIndex(&escola, 2), 1), "Ana");
    addAlunoTurma(&escola, getTurmaByIndex(getDisciplinaByIndex(&escola, 2), 1), "Julio");
    addGrupoTurma(getTurmaByIndex(getDisciplinaByIndex(&escola, 2), 1), "Grupo 01");
    addTurmaDisciplina(getDisciplinaByIndex(&escola, 2), "FP30007-B");

    return menu(&escola, "");
}
