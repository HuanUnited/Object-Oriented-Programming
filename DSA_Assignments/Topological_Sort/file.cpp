#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct Trailer;

struct Lider {
  int key;   // Номер вершины
  int count; // ( ) Число предшественников число входящих дуг
  Lider *nx; // (Lider) Указатель на следующий элемент в списке вершин
  Trailer *tr; // RAILER ( , указатель на Т список указателей на вершины которым
               // ведут исходящие дуги )
};

struct Trailer {
  Lider *ld; // LEADER, указатель на вершину в списке к которой ведет
  // , TRAILER исходящая дуга из вершины к которой прикреплен
  Trailer *nx; // указатель на следующий элемент
};

void PrintAllParentsVertex(Lider *vertexes, int Vx) {
  if (vertexes) {
    Lider *ptrVx;

    for (ptrVx = vertexes; ptrVx && ptrVx->key != Vx; ptrVx = ptrVx->nx)
      ;
    if (ptrVx) {
      Lider *ptrIx = vertexes;

      while (ptrIx) {
        Trailer *tmp = ptrIx->tr;
        while (tmp) {
          if (tmp->ld == ptrVx) {
            printf("(%d, parent %d)\n", ptrVx->key, ptrIx->key);
          }

          tmp = tmp->nx;
        }

        ptrIx = ptrIx->nx;
      }
    }

  } else
    printf("NoVertexes");
}

bool IsEdge(Lider *v_A, Lider *v_B) {
  if (v_A && v_B) {
    Trailer *tmp = v_A->tr;
    while (tmp && tmp->ld != v_B) {
      tmp = tmp->nx;
    }

    if (tmp)
      return true;
  }
  return false;
}
void AddEdge(Lider *&vertexes, int vertexA, int vertexB) {
  Lider *ptrA = NULL, *ptrB = NULL;

  for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->nx)
    ;

  for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->nx)
    ;

  // if(ptrA && ptrB)
  // {
  // if(IsEdge(ptrA,ptrB))
  // return;
  // }

  if (!ptrA) {
    ptrA = new Lider;

    ptrA->key = vertexA;
    ptrA->count = 0;
    ptrA->nx = NULL;
    ptrA->tr = NULL;

    ptrA->nx = vertexes;
    vertexes = ptrA;
  }

  if (!ptrB) {
    ptrB = new Lider;

    ptrB->key = vertexB;
    ptrB->count = 0;
    ptrB->nx = NULL;
    ptrB->tr = NULL;

    ptrB->nx = vertexes;
    vertexes = ptrB;
  }

  //!!!!!!!!!!!!!!!!!!ptrA!=NULL ptrB = !=NULL

  // Устанавливаем ребро

  ptrB->count++;
  //(*ptrB).count++;
  Trailer *elemTr = new Trailer;

  elemTr->ld = ptrB;

  Trailer *headTr = ptrA->tr;

  elemTr->nx = headTr;
  headTr = elemTr;
  ptrA->tr = headTr;
}

/*
 * PrintGraph Вывод графа
 * : Входные параметры
 * vertexes список вершин графа
 *
 */
void PrintGraph(Lider *vertexes) {
  if (vertexes == NULL)
    printf("Graph is Empty");
  else
    while (vertexes) {
      Trailer *trTmp = vertexes->tr;
      if (!trTmp)
        printf("(%d,%d)\n", vertexes->key, 0);
      else {
        while (trTmp) {
          printf("(%d,%d)\n", vertexes->key, trTmp->ld->key);
          trTmp = trTmp->nx;
        }
      }
      vertexes = vertexes->nx;
    }
}

int main() {
  Lider *graph = NULL;
  int vertexA = 0, vertexB = 0;

  AddEdge(graph, 1, 2);
  AddEdge(graph, 1, 2);
  AddEdge(graph, 1, 3);
  AddEdge(graph, 1, 4);
  AddEdge(graph, 2, 5);
  AddEdge(graph, 5, 3);
  AddEdge(graph, 5, 4);
  AddEdge(graph, 3, 4);

  puts("Print Graph");

  PrintGraph(graph);

  PrintAllParentsVertex(graph, 4);

  return 0;
}