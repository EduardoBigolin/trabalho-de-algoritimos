#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 80
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
    int codigo;
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

void listAll(ESCOLA *escola) {
    if (escola->startDisciplina == NULL) {
        printf("Nenhuma disciplina cadastrada.\n");
        return;
    }

    printf("Escola: %s\n", escola->nome);
    printf("Total de disciplinas: %d\n", escola->numeroDeDisciplina);
    printf("========================================\n");

    DISCIPLINAS *atualDisc = escola->startDisciplina;
    int countDisc = 1;

    while (atualDisc != NULL) {
        printf("%d. Disciplina: %s\n", countDisc++, atualDisc->nome);
        printf("   Total de turmas: %d\n", atualDisc->numeroDeTurma);

        TURMA *atualTurma = atualDisc->startTurma;
        if (atualTurma == NULL) {
            printf("   Nenhuma turma cadastrada.\n");
        } else {
            while (atualTurma != NULL) {
                printf("   -> Turma Codigo: %d\n      Numero total de alunos: %d\n", atualTurma->codigo,
                       atualTurma->numeroAlunosTotal);
                printf("      Alunos sem grupos(total: %d):\n", atualTurma->numeroAlunosSemGrupo);

                ALUNOS *atualAluno = atualTurma->startAlunos;
                if (atualAluno == NULL) {
                    printf("         Nenhum aluno cadastrado.\n");
                } else {
                    while (atualAluno != NULL) {
                        printf("         - %s (idade: %d)\n", atualAluno->nome, atualAluno->idade);
                        atualAluno = atualAluno->next;
                    }
                }

                GRUPOS *atualGrupo = atualTurma->startGrupos;
                printf("      Grupos (total: %d):\n", atualTurma->numeroGrupo);
                if (atualGrupo == NULL) {
                    printf("         Nenhum grupo cadastrado.\n");
                } else {
                    while (atualGrupo != NULL) {
                        printf("         - Grupo: %s (Alunos: %d)\n", atualGrupo->nome, atualGrupo->numeroDeAlunos);
                        atualGrupo = atualGrupo->next;
                    }
                }
                printf("\n");

                atualTurma = atualTurma->next;
            }
        }

        printf("\n========================================\n");
        atualDisc = atualDisc->next;
    }
}

void listDisciplinas(ESCOLA *escola) {
    if (escola->startDisciplina == NULL) {
        printf("Nenhuma disciplina cadastrada.\n");
        return;
    }

    DISCIPLINAS *atualDisc = escola->startDisciplina;
    int countDisc = 1;

    while (atualDisc != NULL) {
        printf("%d. %s\n", countDisc++, atualDisc->nome);
        atualDisc = atualDisc->next;
    }
    printf("\n=========================================\n");
}

DISCIPLINAS *getDisciplinaByIndex(ESCOLA *escola, int index) {
    if (escola->startDisciplina == NULL) {
        printf("Disciplina não encontrada.\n");
        return NULL;
    }
    DISCIPLINAS *atualDisc = escola->startDisciplina;
    int i = 1;
    while (i != index) {
        atualDisc = atualDisc->next;
        i++;
    }
    return atualDisc;
}

void addDisciplinaEscola(ESCOLA *escola, char nome[N]) {
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

int addTurmaDisciplina(DISCIPLINAS *disciplina, int codigo) {
    TURMA *newTurma = (TURMA *) malloc(sizeof(TURMA));
    if (!newTurma) return ERROR;

    newTurma->codigo = codigo;
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

int addAlunoEscola(ESCOLA *escola, char nome[N], int idade) {
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

ALUNOS *getAlunoEscola(ESCOLA *escola, char nomeAluno[N]) {
    ALUNOS *aluno = escola->startAlunos;
    while (aluno && strcmp(aluno->nome, nomeAluno) != 0)
        aluno = aluno->next;

    if (!aluno) {
        printf("Aluno '%s' não encontrado na escola.\n", nomeAluno);
        return;
    }
    return aluno;
}

void addAlunoTurma(ESCOLA *escola, TURMA *turma, char nome[N]) {
    ALUNOS *alunoCopy = (ALUNOS *) malloc(sizeof(ALUNOS)), *aluno = getAlunoEscola(escola, nome);

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
}

void addGrupoTurma(TURMA *turma, char nome[N]) {
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

void moverAlunoParaGrupo(TURMA *turma, char nomeAluno[N], char nomeGrupo[N]) {
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

void menu_addDisciplina(ESCOLA *escola) {
    printf("Digite o nome da Disciplina: \n");
    getchar();
    char nomeDisciplina[N];
    fgets(nomeDisciplina, sizeof(nomeDisciplina), stdin);
    addDisciplinaEscola(escola, nomeDisciplina);
}

int menu_addTurma(ESCOLA *escola) {
    int numeroTurma, idx;

    listDisciplinas(escola);

    printf("Digite o número da Disciplina que deseja adicionar a turma: \n");
    scanf("%d", &idx);

    printf("Digite o número da Turma (Ex: 101, 204): \n");
    scanf("%d", &numeroTurma);

    return addTurmaDisciplina(getDisciplinaByIndex(escola, idx), numeroTurma);
}

int menu_addAluno(ESCOLA *escola) {
    char nome[N];
    int idade;

    printf("Digite o nome do aluno:\n");
    getchar();
    fgets(nome, sizeof(nome), stdin);

    printf("Digite a idade do aluno:\n");
    scanf("%d", &idade);

    return addAlunoEscola(escola, nome, idade);
}

int menu(ESCOLA *escola, char errorMsg[N]) {
    char msg = "";

    listAll(escola);

    if (strcmp(errorMsg, "") == 0) {
        printf("Erro ao executar a operacao: %s", errorMsg);
    }

    printf("Menu:\n");
    printf("1. Criar Disciplina\n");
    printf("2. Criar Turma\n");
    printf("3. Criar Aluno\n");
    printf("4. Adicionar Aluno em Turma\n");
    printf("5. Criar Grupo em Turma\n");
    printf("6. Adicionar Aluno em Grupo\n");
    printf("10. Exit\n");
    printf("Digite o número correspontente com a operação desejada (1, 2, 3, 4...) ou \"s\" para sair: \n");
    const int choice = getchar();
    fflush(stdin);
    printf("\n");

    switch (choice) {
        case '1':
            menu_addDisciplina(escola);
            menu(escola, "");
            break;
        case '2':
            if (menu_addTurma(escola) == ERROR) {
                strcpy(&msg, "Turma ja existe");
            }
            menu(escola, &msg);
            break;
        case '3':
            if (menu_addAluno(escola) == ERROR) {
                strcpy(&msg, "Aluno ja existe");
            }
            menu(escola, &msg);
            break;
        case 's':
            printf("Saindo do menu.\n");
            return OK;
            break;
        default:
            menu(escola, "Operacao invalida");
    }
}

int main() {
    ESCOLA escola = {.numeroDeDisciplina = 0, .startDisciplina = NULL, .endDisciplina = NULL};
    strcpy(escola.nome, "Universidade de Caxias do Sul");

    createListOfDisciplinas(&escola, "Matematica");
    createListOfTurmas(escola.andDisciplina, 100);
    createListOfAlunosTurma(escola.andDisciplina->andTurma, "Eduardo", 12);
    createListOfGruposTurma(escola.andDisciplina->andTurma, "Grupo 01");
    createListOfGruposTurma(escola.andDisciplina->andTurma, "Grupo 02");
    moverAlunoParaGrupo(escola.andDisciplina->andTurma, "Eduardo", "Grupo 01");
    createListOfTurmas(escola.andDisciplina, 101);

    createListOfDisciplinas(&escola, "Portugues");
    createListOfTurmas(escola.andDisciplina, 102);
    createListOfAlunosTurma(escola.andDisciplina->andTurma, "Ana", 20);
    createListOfAlunosTurma(escola.andDisciplina->andTurma, "Lucas", 22);
    createListOfGruposTurma(escola.andDisciplina->andTurma, "Grupo 01");
    createListOfTurmas(escola.andDisciplina, 103);

    createListOfDisciplinas(&escola, "Ingles");
    createListOfTurmas(escola.andDisciplina, 104);
    createListOfTurmas(escola.andDisciplina, 105);

    createListOfDisciplinas(&escola, "Geografia");
    createListOfTurmas(escola.andDisciplina, 106);
    createListOfTurmas(escola.andDisciplina, 107);

    return menu(&escola, "");
}
