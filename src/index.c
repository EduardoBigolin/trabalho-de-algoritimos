#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 80

typedef struct Alunos {
    char nome[N];
    int idade;

    struct Alunos* next;
    struct Alunos* prev;
} ALUNOS;

typedef struct Grupos {
    char nome[N];
    int numeroDeAlunos;

    struct Grupos* next;
    struct Grupos* prev;

    ALUNOS* start;
    ALUNOS* and;
} GRUPOS;

typedef struct Turma {
    int codigo;
    int numeroGrupo;
    int numeroAlunosSemGrupo;
    int numeroAlunosTotal;

    ALUNOS* startAlunos;
    ALUNOS* andAlunos;

    struct Turma* next;
    struct Turma* prev;

    GRUPOS* startGrupos;
    GRUPOS* andGrupos;
} TURMA;

typedef struct Disciplina {
    struct Disciplina* next;
    struct Disciplina* prev;

    char nome[N];
    int numeroDeTurma;

    TURMA* startTurma;
    TURMA* andTurma;
} DISCIPLINAS;

typedef struct Escola {
    char nome[N];
    int numeroDeDisciplina;

    DISCIPLINAS* startDisciplina;
    DISCIPLINAS* andDisciplina;
} ESCOLA;

void printDisciplinas(ESCOLA* escola) {
    if (escola->startDisciplina == NULL) {
        printf("Nenhuma disciplina cadastrada.\n");
        return;
    }

    printf("Escola: %s\n", escola->nome);
    printf("Total de disciplinas: %d\n", escola->numeroDeDisciplina);
    printf("========================================\n");

    DISCIPLINAS* atualDisc = escola->startDisciplina;
    int countDisc = 1;

    while (atualDisc != NULL) {
        printf("%d. Disciplina: %s\n", countDisc++, atualDisc->nome);
        printf("   Total de turmas: %d\n", atualDisc->numeroDeTurma);

        TURMA* atualTurma = atualDisc->startTurma;
        if (atualTurma == NULL) {
            printf("   Nenhuma turma cadastrada.\n");
        } else {
            while (atualTurma != NULL) {
                printf("   -> Turma Codigo: %d Numero total de alunos: %d\n", atualTurma->codigo, atualTurma->numeroAlunosTotal);
                printf("      Alunos sem grupos(total: %d):\n", atualTurma->numeroAlunosSemGrupo);

                ALUNOS* atualAluno = atualTurma->startAlunos;
                if (atualAluno == NULL) {
                    printf("         Nenhum aluno cadastrado.\n");
                } else {
                    while (atualAluno != NULL) {
                        printf("         - %s (idade: %d)\n", atualAluno->nome, atualAluno->idade);
                        atualAluno = atualAluno->next;
                    }
                }

                GRUPOS* atualGrupo = atualTurma->startGrupos;
                printf("      Grupos (total: %d):\n", atualTurma->numeroGrupo);
                if (atualGrupo == NULL) {
                    printf("         Nenhum grupo cadastrado.\n");
                } else {
                    while (atualGrupo != NULL) {
                        printf("         - Grupo: %s (Alunos: %d)\n", atualGrupo->nome, atualGrupo->numeroDeAlunos);
                        atualGrupo = atualGrupo->next;
                    }
                }

                atualTurma = atualTurma->next;
            }
        }

        printf("========================================\n");
        atualDisc = atualDisc->next;
    }
}

void createListOfDisciplinas(ESCOLA* escola, char nome[N]) {
    DISCIPLINAS* newDisciplina = (DISCIPLINAS*)malloc(sizeof(DISCIPLINAS));
    if (!newDisciplina) return;

    strcpy(newDisciplina->nome, nome);
    newDisciplina->numeroDeTurma = 0;
    newDisciplina->startTurma = NULL;
    newDisciplina->andTurma = NULL;
    newDisciplina->next = NULL;
    newDisciplina->prev = NULL;

    if (escola->andDisciplina == NULL) {
        escola->startDisciplina = newDisciplina;
        escola->andDisciplina = newDisciplina;
    } else {
        DISCIPLINAS* aux = escola->andDisciplina;
        aux->next = newDisciplina;
        newDisciplina->prev = aux;
        escola->andDisciplina = newDisciplina;
    }

    escola->numeroDeDisciplina++;
}

void createListOfTurmas(DISCIPLINAS* disciplina, int codigo) {
    TURMA* newTurma = (TURMA*)malloc(sizeof(TURMA));
    if (!newTurma) return;

    newTurma->codigo = codigo;
    newTurma->numeroAlunosTotal = 0;
    newTurma->numeroAlunosSemGrupo = 0;
    newTurma->numeroGrupo = 0;

    newTurma->startAlunos = NULL;
    newTurma->andAlunos = NULL;
    newTurma->startGrupos = NULL;
    newTurma->andGrupos = NULL;

    newTurma->next = NULL;
    newTurma->prev = NULL;

    if (disciplina->andTurma == NULL) {
        disciplina->startTurma = newTurma;
        disciplina->andTurma = newTurma;
    } else {
        TURMA* aux = disciplina->andTurma;
        aux->next = newTurma;
        newTurma->prev = aux;
        disciplina->andTurma = newTurma;
    }

    disciplina->numeroDeTurma++;
}

void createListOfAlunosTurma(TURMA* turma, char nome[N], int idade) {
    ALUNOS* newAluno = (ALUNOS*)malloc(sizeof(ALUNOS));
    if (!newAluno) return;

    strcpy(newAluno->nome, nome);
    newAluno->idade = idade;
    newAluno->next = NULL;
    newAluno->prev = NULL;

    if (turma->andAlunos == NULL) {
        turma->startAlunos = newAluno;
        turma->andAlunos = newAluno;
    } else {
        ALUNOS* aux = turma->andAlunos;
        aux->next = newAluno;
        newAluno->prev = aux;
        turma->andAlunos = newAluno;
    }

    turma->numeroAlunosTotal++;
    turma->numeroAlunosSemGrupo++;
}

void createListOfGruposTurma(TURMA* turma, char nome[N]) {
    GRUPOS* newGrupo = (GRUPOS*)malloc(sizeof(GRUPOS));
    if (!newGrupo) return;

    strcpy(newGrupo->nome, nome);
    newGrupo->numeroDeAlunos = 0;
    newGrupo->start = NULL;
    newGrupo->and = NULL;
    newGrupo->next = NULL;
    newGrupo->prev = NULL;

    if (turma->andGrupos == NULL) {
        turma->startGrupos = newGrupo;
        turma->andGrupos = newGrupo;
    } else {
        GRUPOS* aux = turma->andGrupos;
        aux->next = newGrupo;
        newGrupo->prev = aux;
        turma->andGrupos = newGrupo;
    }

    turma->numeroGrupo++;
}

void moverAlunoParaGrupo(TURMA* turma, char nomeAluno[N], char nomeGrupo[N]) {
    ALUNOS* aluno = turma->startAlunos;
    while (aluno && strcmp(aluno->nome, nomeAluno) != 0)
        aluno = aluno->next;

    if (!aluno) {
        printf("Aluno '%s' não encontrado na turma.\n", nomeAluno);
        return;
    }

    GRUPOS* grupo = turma->startGrupos;
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
    else turma->andAlunos = aluno->prev;

    aluno->next = NULL;
    aluno->prev = NULL;

    turma->numeroAlunosSemGrupo--;

    // Adiciona no grupo
    if (!grupo->and) {
        grupo->start = aluno;
        grupo->and = aluno;
    } else {
        grupo->and->next = aluno;
        aluno->prev = grupo->and;
        grupo->and = aluno;
    }

    grupo->numeroDeAlunos++;
}

int main() {
    ESCOLA escola = { .numeroDeDisciplina = 0, .startDisciplina = NULL, .andDisciplina = NULL };
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

    printDisciplinas(&escola);

    return 0;
}
