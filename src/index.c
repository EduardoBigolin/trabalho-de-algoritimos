#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Alunos {
  char nome[20];
  int idade;

  struct Alunos * next;
  struct Alunos * prev;
} typedef ALUNOS;

struct Grupos {
  char nome[20];
  int numeroDeAlunos;

  struct Grupos* next;
  struct Grupos* prev;

  ALUNOS* start;
  ALUNOS* and;
} typedef GRUPOS;

struct Turma {
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
} typedef TURMA;

struct Disciplina {
  struct Disciplina* next;
  struct Disciplina* prev;

  char nome[20];
  int numeroDeTurma;

  TURMA* startTurma;
  TURMA* andTurma;
} typedef DISCIPLINAS;

struct Escola {
  char nome[20];
  int numeroDeDisciplina;

  DISCIPLINAS* startDisciplina;
  DISCIPLINAS* andDisciplina;
} typedef ESCOLA;
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
              printf("   -> Turma Código: %d Numero total de alunos: %d\n", atualTurma->codigo, atualTurma->numeroAlunosTotal);
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

              // Grupos da Turma
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
void createListOfDisciplinas(ESCOLA* escola, char nome[20]){
  DISCIPLINAS* newDisciplina = (DISCIPLINAS*) malloc(sizeof(DISCIPLINAS));
  if(newDisciplina == NULL) {
    //! add error
  }
  strcpy(newDisciplina->nome, nome);
  newDisciplina->numeroDeTurma = 0;
  newDisciplina->andTurma = NULL;
  newDisciplina->startTurma = NULL;
  if(escola->andDisciplina == NULL) {
    escola->startDisciplina = newDisciplina;
    escola->andDisciplina = newDisciplina;
  }else {
    DISCIPLINAS* aux = escola->andDisciplina;
    escola->andDisciplina = newDisciplina;
    newDisciplina->prev = aux;
    aux->next = newDisciplina;
  }
  escola->numeroDeDisciplina++;
}

void createListOfTurmas(DISCIPLINAS* disciplina, int codigo) {
  if (disciplina == NULL) {
      printf("Erro: disciplina nula.\n");
      return;
  }

  TURMA* newTurma = (TURMA*) malloc(sizeof(TURMA));
  if (newTurma == NULL) {
      printf("Erro ao alocar memória para nova turma.\n");
      return;
  }

  newTurma->codigo = codigo;
  newTurma->next = NULL;
  newTurma->prev = NULL;

  if (disciplina->andTurma == NULL) {
      disciplina->startTurma = newTurma;
      disciplina->andTurma = newTurma;
  } else {
      TURMA* aux = disciplina->andTurma;
      disciplina->andTurma = newTurma;
      newTurma->prev = aux;
      aux->next = newTurma;
  }

  disciplina->numeroDeTurma++;
}


void createListOfAlunosTurma(TURMA* turma, char nome[20], int idade) {
  if (turma == NULL) {
      printf("Erro: turma nula.\n");
      return;
  }

  ALUNOS* newAluno = (ALUNOS*) malloc(sizeof(ALUNOS));
  if (newAluno == NULL) {
      printf("Erro ao alocar memória para nova aluno.\n");
      return;
  }

  strcpy(newAluno->nome, nome);
  newAluno->idade = idade;

  newAluno->next = NULL;
  newAluno->prev = NULL;

  if (turma->andAlunos == NULL) {
    turma->startAlunos = newAluno;
    turma->andAlunos = newAluno;
  } else {
      ALUNOS* aux = turma->andAlunos;
      turma->andAlunos = newAluno;
      newAluno->prev = aux;
      aux->next = newAluno;
  }

  turma->numeroAlunosTotal++;
  turma->numeroAlunosSemGrupo++;
}

void createListOfGruposTurma(TURMA* turma, char nome[20]) {
  if (turma == NULL) {
    printf("Erro: turma nula.\n");
      return;
  }

  GRUPOS* newGrupo = (GRUPOS*) malloc(sizeof(GRUPOS));
  if (newGrupo == NULL) {
      printf("Erro ao alocar memória para nova aluno.\n");
      return;
  }

  strcpy(newGrupo->nome, nome);

  newGrupo->next = NULL;
  newGrupo->prev = NULL;

  if (turma->andGrupos == NULL) {
    turma->startGrupos = newGrupo;
    turma->andGrupos = newGrupo;
  } else {
      GRUPOS* aux = turma->andGrupos;
      turma->andGrupos = newGrupo;
      newGrupo->prev = aux;
      aux->next = newGrupo;
  }

  turma->numeroGrupo++;
}
void moverAlunoParaGrupo(TURMA* turma, char nomeAluno[20], char nomeGrupo[20]) {
  if (turma == NULL) {
      printf("Erro: turma nula.\n");
      return;
  }

  // 1. Procurar o aluno na lista da turma
  ALUNOS* aluno = turma->startAlunos;
  while (aluno != NULL && strcmp(aluno->nome, nomeAluno) != 0) {
      aluno = aluno->next;
  }

  if (aluno == NULL) {
      printf("Aluno '%s' não encontrado na turma.\n", nomeAluno);
      return;
  }

  // 2. Procurar o grupo pelo nome
  GRUPOS* grupo = turma->startGrupos;
  while (grupo != NULL && strcmp(grupo->nome, nomeGrupo) != 0) {
      grupo = grupo->next;
  }

  if (grupo == NULL) {
      printf("Grupo '%s' não encontrado na turma.\n", nomeGrupo);
      return;
  }

  // 3. Remover aluno da lista geral da turma
  if (aluno->prev != NULL) aluno->prev->next = aluno->next;
  else turma->startAlunos = aluno->next;
  
  if (aluno->next != NULL) aluno->next->prev = aluno->prev;
  else turma->andAlunos = aluno->prev;
  
  aluno->next = NULL;
  aluno->prev = NULL;

  turma->numeroAlunosSemGrupo--;

  // 4. Adicionar aluno ao grupo (fim da lista do grupo)
  if (grupo->and == NULL) {
      grupo->start = aluno;
      grupo->and = aluno;
  } else {
      aluno->prev = grupo->and;
      grupo->and->next = aluno;
      grupo->and = aluno;
  }

  grupo->numeroDeAlunos++;
}

int main() {
  // Criando Escola
  ESCOLA escola;
  // Solicitar o nome 
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