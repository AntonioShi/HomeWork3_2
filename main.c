/**
 * 作业3-2复盘记录
[ 发布人： 曲超   截止时间：2017-10-26 23:59:59  ]
棋类比赛后需要“复盘”，即将比赛双方的每一步操作重现，但不包括“悔棋”操作，请设计一种结构能够在比赛过程进行记录，在比赛后能够将操作保存下来进行复盘。
 （以围棋为例，每一步操作包括记录落子的行、列、颜色）
要求:
1、设计存储结构
2、对多次输入进行记录（输入:三个整形，分别代表行、列、颜色）
3、若输入-1，则将前一步的输入删除（注意连续多个-1的情况）
4、将所有输出按输入顺序输出

 */
#include <stdio.h>
#include <malloc.h>

typedef int DataType ;
#define ReLodg -1 //悔棋的标志
#define GameOver -100//游戏结束标志

typedef struct Piece{
    DataType raw ;
    DataType col ;
    DataType color ;//0白 或者 1黑
}NPiece;

typedef struct Node{
    NPiece piece ;//棋子数据的结构体

    struct Node *next ;
    struct Node *prior ;
}MockBoard;//模拟棋盘


void DLNodeInitiate(MockBoard **head){
    *head = (MockBoard *)malloc(sizeof(MockBoard)) ;
    (*head)->next = *head ;
    (*head)->prior = *head ;
    (*head)->piece.raw = -1 ;
    (*head)->piece.col = -1 ;
    (*head)->piece.color = -1 ;
}

int DLNodeInsert(MockBoard *head, int i, NPiece pe){
    MockBoard *p, *q ;
    int j = 0 ;
    //p = head ;,error!!
    p = head->next ;
    while(p != head && j < i){
        p = p->next ;
        j ++ ;
    }

    if(j != i){
        printf("Insert参数错误\n") ;
        return 0 ;
    }

    q = (MockBoard *)malloc(sizeof(MockBoard)) ;
    q->piece.raw = pe.raw ;
    q->piece.col = pe.col ;
    q->piece.color = pe.color ;

    //插入默认是插在目前元素的后面，课本上是在前面。
//    q->next = p->next ;
//    p->next->prior = q ;
//    p->next = q ;
//    q->prior = p ;

    //前面:better
    q->prior = p->prior ;
    p->prior->next = q ;


    q->next = p ;
    p->prior = q ;
    //都可以，且结果一样

    return 1 ;
}


int DLNodeDelete(MockBoard *head, int i, NPiece *pe){
    //删除i位置处的节点信息
    MockBoard *p ;
    int j = 0 ;
    //p = head ;
    p = head->next ;
    //while(p != NULL && j < i){ error!!
    while(p->next != head && j < i){
        p = p->next ;
        j ++ ;
    }

    if(j != i){
        printf("Delete参数错误\n") ;
        return 0 ;
    }

    (*pe).raw = p->piece.raw ;
    (*pe).col = p->piece.col ;
    (*pe).color = p->piece.color ;

    p->prior->next = p->next ;//
    p->next->prior = p->prior ;

    free(p) ;

    return 1 ;
}

int DLNodeLength(MockBoard *head){
    MockBoard *p = head ;
    int size = 0 ;

    while(p->next != head){//**
        p = p->next ;
        size ++ ;
    }

    return size ;
}

void Destroy(MockBoard **head){
    MockBoard *p, *q ;
    p = *head ;
    int i, n = DLNodeLength(*head) ;
    //for (i = 0; i < n; ++i) {!!
    for (i = 0; i <= n; ++i) {
        q = p ;
        p = p->next ;
        free(q) ;
    }

    *head = NULL ;

}

int DLNodeGet(MockBoard *head, int i, NPiece *pe){
    MockBoard *p = head->next ;
    int j = 0 ;

    while(p != head && j < i){
        p = p->next ;
        j ++ ;
    }

    if(j != i){
        printf("参数错误2\n") ;
        return 0 ;
    }

    (*pe).raw = p->piece.raw ;
    (*pe).col = p->piece.col ;
    (*pe).color = p->piece.color ;

    return 1 ;
}

void GameStart(MockBoard *head){
    int Flag = 1 ;
    NPiece pe ;

    MockBoard *p = head ;
    MockBoard *q = NULL ;

    printf("----------------------------------------------------------------------\n") ;
    printf("请按提示下棋. 行  列  颜色(0白1黑) 悔棋(-1 -1 -1)  结束标志(-100 -100 -100)\n") ;
    printf("----------------------------------------------------------------------\n") ;


    for (int i = 0; Flag != -1; i++){

        scanf("%d %d %d", &pe.raw, &pe.col, &pe.color) ;

        if(pe.raw == ReLodg || pe.col == ReLodg || pe.color == ReLodg){
            //悔棋操作
            printf("请问悔棋者打算悔几步?(根据规则最多5步)\n") ;
            int retemp ;
            scanf("%d", &retemp) ;
            for (int i = 0; i < retemp; ++i) {
                DLNodeDelete(head, DLNodeLength(head)-1, &pe) ;//删除当前步骤的数据
            }

            printf("重新出棋吧\n") ;
            for (int i = 0; i < retemp; ++i){
                scanf("%d %d %d", &pe.raw, &pe.col, &pe.color) ;
                DLNodeInsert(head, DLNodeLength(head), pe) ;//重新输入-给你个后悔药吧:问题处在pe上,pe是刚删除的数据,还没有更新就是用了
            }
            printf("悔棋结束,请继续开始吧.\n") ;

        }
        else if(pe.raw == GameOver || pe.col == GameOver || pe.color == GameOver) {

            Flag = -1 ;
        }
        else{

            DLNodeInsert(head, DLNodeLength(head), pe);
        }
    }

    printf("复棋.\n") ;
    for (int i = 0; i < DLNodeLength(head); i++) {
        DLNodeGet(head, i, &pe) ;
        printf("%-3d%-3d%-3d\n", pe.raw, pe.col, pe.color) ;
    }
    printf("\n") ;

}

int main() {
    printf("Hello, World!\n");

    MockBoard *head ;//模拟棋盘
    NPiece pe ;

    DLNodeInitiate(&head) ;

    GameStart(head) ;

    Destroy(&head) ;

    printf("游戏结束!\n") ;
//    pe.color = 0 ;
//    pe.col = 0 ;
//    pe.raw = 0 ;
//
//    int i, s ;
//    int overFlag = ReLodg ;
//    int reLodg = GameOver ;
//
//    GameStart(head) ;
//
//    for (i = 0; i < 10; ++i) {
//        DLNodeInsert(head, i, pe) ;
//    }
//
//
//    int n = DLNodeLength(head) ;
//    printf("n = %4d\n", n) ;
//    for (i = 0; i < DLNodeLength(head); ++i) {
//        DLNodeGet(head, i, &pe) ;
//        printf("%d %d %d", pe.raw, pe.col, pe.color) ;
//    }
//    printf("\n") ;
//    DLNodeDelete(head, 4, &pe) ;
//
//    n = DLNodeLength(head) ;
//    printf("n = %4d\n", n) ;
//
//    for (i = 0; i < DLNodeLength(head); ++i) {
//        DLNodeGet(head, i, &pe) ;
//        printf("%d %d %d", pe.raw, pe.col, pe.color) ;
//    }
//    printf("\n") ;

    return 0;
}


