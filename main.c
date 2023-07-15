#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RED, BLACK } Color;

typedef struct cars{
    int x;
    struct cars *prec, *succ;
}car_station;

typedef struct stations{
    int station;
    struct stations  *succ;
    struct stations  *prec;
}percorso;

typedef struct testa{
    int station;
    struct testa  *percorso_succ;
    struct stations  *pointer_pos;
}testa_percorso;

typedef struct _node{
    char color;
    struct _node *left, *right, *parent;
    int station;
    car_station *cars;
    int max;
    int num_cars;
    int visited;
}node_t;

node_t* nil;
node_t* root;
percorso* temp=NULL;


// list functions
int insert_car(int car,car_station** head,int max){
    //inserimento in testa
    car_station *newCar=(car_station*)malloc(sizeof(car_station));
    if(*head==NULL){
        newCar->x=car;
        newCar->succ=NULL;
        newCar->prec=NULL;
        *head=newCar;
        return car;
    }
    else{
        newCar->x=car;
        newCar->succ=*head;
        newCar->prec=NULL;
        (*head)->prec=newCar;
        *head=newCar;
        if(car>max)
            return car;
        else return max;
    }}
void stampa_car(node_t* node){
    car_station* head=node->cars;
    while(head!=NULL){
        printf("macchina %d\n",head->x);
        head=head->succ;
    }
}
void rimozione(node_t* node,int autonomy){
    int change_max;
    int res=0;
    int dim=node->num_cars;
    car_station *car=node->cars;
    if (dim != 0) {
        if (autonomy == node->max) { change_max = 1; }
        else change_max = 0;
        while (car != NULL) {
            if (car->x == autonomy) {
                if(car->prec!=NULL)
                    car->prec->succ = car->succ;
                else node->cars=car->succ;
                if(car->succ!=NULL)
                    car->succ->prec = car->prec;
                free(car);
                node->num_cars--;
                printf("rottamata\n");
                res = 1;
                if(change_max==0){
                    break;
                }
                else car->x=0;
            }
            if (change_max == 1) {
                if (car->prec == NULL || node->max < car->x)
                    node->max = car->x;
            }
            car = car->succ;
        }
        if (res == 0){
            printf("non rottamata\n");
        }
    }
}

void insert_tappa(int station,percorso** tail){
    if((*tail)==NULL){
        percorso *primatappa= (percorso*)malloc(sizeof(percorso));
        primatappa->station=station;
        primatappa->succ=NULL;
        primatappa->prec=NULL;
        (*tail)=primatappa;
    }
    else{
        if((*tail)->prec==NULL){
            percorso *tappa= (percorso*)malloc(sizeof(percorso));
            tappa->station=station;
            tappa->succ=(*tail);
            tappa->prec=NULL;
            (*tail)->prec=tappa;
        }
        else{
            percorso *trasp=(*tail);
            percorso *tappa= (percorso*)malloc(sizeof(percorso));
            tappa->station=station;
            tappa->prec=NULL;
            while(trasp->prec!=NULL){
                trasp=trasp->prec;
            }
            trasp=tappa;
        }
    }
}
void stampa_percorso(testa_percorso** testa,int dest){
    testa_percorso *perc=(*testa)->percorso_succ;
    percorso* g=perc->pointer_pos;
    printf("%d ",(*testa)->station);
    while(perc!=NULL){
        printf("%d ",perc->station);
        while(g->prec!=NULL){
            g=g->prec;
        }
        while(g!=NULL){
            printf("%d ",g->station);
            g=g->succ;
        }
        perc=perc->percorso_succ;
        if(perc!=NULL){
            g=perc->pointer_pos;
        }
    }
    printf("%d ",dest);
    printf("\n");
}
void insert_percorso(testa_percorso** testa,testa_percorso* thispercorso,int part){
    if((*testa)==NULL){
        (*testa)=(testa_percorso*)malloc(sizeof(testa_percorso));
        (*testa)->station=part;
        (*testa)->pointer_pos=NULL;
        (*testa)->percorso_succ=thispercorso;
    }
    else{
        testa_percorso* pos=(*testa)->percorso_succ;
        while(pos!=NULL){
            pos=pos->percorso_succ;
        }
        pos=thispercorso;
    }
}
/*
void add_successor(int station,testa_percorso** thispercorso,int sposta){
    if((*thispercorso)->station==NULL){
        testa_percorso *creotesta=(testa_percorso*)malloc(sizeof(testa_percorso));
        percorso *primosucc= (percorso*)malloc(sizeof(percorso));
        primosucc->station=station;
        primosucc->tappa_succ=NULL;
        primosucc->next=NULL;
        primosucc->nextinline=NULL;
        primosucc->prec=NULL;
        creotesta->pointer_pos=primosucc;
        creotesta->percorso_succ=NULL;
        creotesta->station=1;
        (*thispercorso)=creotesta;
    }
    else{
        percorso* newSucc=(percorso*)malloc(sizeof(percorso));
        newSucc->tappa_succ=NULL;
        newSucc->next=NULL;
        newSucc->prec=NULL;
        newSucc->station=station;
        if((*thispercorso)->pointer_pos->tappa_succ==NULL){
            newSucc->prec=(*thispercorso)->pointer_pos;
            (*thispercorso)->pointer_pos->tappa_succ=newSucc;
        }
        else{
            if((*thispercorso)->pointer_pos->next==NULL){
                (*thispercorso)->pointer_pos->next=newSucc;
                (*thispercorso)->pointer_pos->tappa_succ->nextinline=(*thispercorso)->pointer_pos->next;
            }
            else{
                percorso* pos=(*thispercorso)->pointer_pos->next;
                while(pos!=NULL){
                    pos=pos->nextinline;
                }
                pos=newSucc;
                pos->nextinline=NULL;
            }
        }
        if(sposta==1){
            newSucc->prec=(*thispercorso)->pointer_pos;
            (*thispercorso)->pointer_pos=newSucc;
        }
    }
}
void insert_percorso(testa_percorso* station,testa_percorso** last){
    testa_percorso* head=last;
    if(head==NULL){
        station->percorso_succ=NULL;
        *(last)=station;
    }
    else{
        while(head->percorso_succ!=NULL){
            head=head->percorso_succ;
        }
        //aggiungo in coda il nuovo percorso
        head->percorso_succ=station;
    }
}
void delete_last_insert(testa_percorso** this_percorso,int delete){
     percorso* cancel=(*this_percorso)->pointer_pos;
     (*this_percorso)->pointer_pos=(*this_percorso)->pointer_pos->prec;
     if(delete==1 && cancel!=NULL){
            free(cancel);
        }
}
*/
//red black functions
void left_rotate(node_t* node){
    node_t* y=node->right;
    node->right=y->left;
    if(y->left!=nil){
        y->left->parent=node;
    }
    y->parent=node->parent;
    if(node->parent==nil)
    {  root=y;}
    else if( node== node->parent->left){
        node->parent->left=y;
    }
    else{
        node->parent->right=y;
    }
    y->left=node;
    node->parent=y;
}
void right_rotate(node_t* node){
    node_t* y=node->left;
    node->left=y->right;
    if(y->right!=nil){
        y->right->parent=node;
    }
    y->parent=node->parent;
    if(node->parent==nil){
        root=y;
    }
    else if(node==node->parent->right){
        node->parent->right=y;
    }
    else {
        node->parent->left=y;
    }
    y->right=node;
    node->parent=y;
}
void rb_fixup(node_t* newNode){
    node_t* x=nil;
    node_t* y=nil;
    if(newNode==root){
        root->color=BLACK;
    }
    else{
        x = newNode->parent;
        if(x->color==RED){
            if(x==x->parent->left){
                y=x->parent->right;
                if( y->color==RED){
                    x->parent->color=RED;
                    x->color=BLACK;
                    y->color=BLACK;
                    rb_fixup(x->parent);
                }
                else{
                    if(newNode==x->right){
                        newNode=x;
                        left_rotate(newNode);
                        x=newNode->parent;
                    }
                    x->color=BLACK;
                    x->parent->color=RED;
                    right_rotate(x->parent);
                }
            }
            else{
                y=x->parent->left;
                if( y->color==RED){
                    x->parent->color=RED;
                    x->color=BLACK;
                    y->color=BLACK;
                    rb_fixup(x->parent);
                }
                else{
                    if(newNode==x->left){
                        newNode=x;
                        right_rotate(newNode);
                        x=newNode->parent;
                    }
                    x->color=BLACK;
                    x->parent->color=RED;
                    left_rotate(x->parent);
                }
            }
        }}
}
node_t *createNode(int station,int cars[],int dim){
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->station = station;
    newNode->color = RED;
    newNode->parent = nil;
    newNode->left = nil;
    newNode->right = nil;
    newNode->num_cars=dim;
    newNode->visited=0;
    car_station* head=NULL;
    if(dim!=0){
        for(int i=0;i<dim;i++){
            newNode->max= insert_car(cars[i],&head,newNode->max);
        }
    }
    else{
        newNode->max=0;
    }
    newNode->cars=head;
    return newNode;
}
void insert_station(node_t* newNode){
    node_t* y=nil;
    node_t* x=root;
    while( x!=nil){
        y=x;
        if(newNode->station< y->station){
            x=x->left;
        }
        else x=x->right;
    }
    newNode->parent=y;
    if(y==nil){
        root=newNode;
    }
    else {
        if (newNode->station < y->station) {
            newNode->parent->left = newNode;
        } else { newNode->parent->right = newNode; }
    }
    rb_fixup(newNode);
}
void stampa(node_t* x){
    if(x!=nil){
        stampa(x->left);
        int y=x->station+x->max;
        printf("station %d arrivo fino %d",x->station,y);
        printf("\n");
        stampa(x->right);
    }
}
node_t* rb_search(int station){
    node_t* x=root;
    while(x!=nil){
        if(x->station==station)
        { return x;}
        else{
            if(station<x->station){ x=x->left;}
            else x=x->right;
        }
    }
    return nil;
}
node_t* tree_min(node_t* node){
    while(node->left!= nil){
        node=node->left;
    }
    return node;
}
node_t* tree_max(node_t* node){
    while(node->right!= nil){
        node=node->right;
    }
    return node;
}
node_t* rb_successore(node_t* node){
    if(node->right!= nil)
        return tree_min(node->right);
    node_t* y=node->parent;
    while(y!=nil && node==y->right){
        node=y;
        y=y->parent;
    }
    return y;
}
node_t* rb_predecessore(node_t* node){
    if(node->left!= nil)
        return tree_max(node->left);
    node_t* y=node->parent;
    while(y!=nil && node==y->left){
        node=y;
        y=y->parent;
    }
    return y;
}
void rb_delete_fixup(node_t* x){
    node_t* w;
    while(x != root && x->color == BLACK){
        if(x == x->parent->left){
            w = x->parent->right;
            if(w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }

            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else{
                if(w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(x->parent);
                x = root;
            }
        }
        else{
            w = x->parent->left;
            if(w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(x->parent);
                w = x->parent->left;
            }

            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else{
                if(w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}
void delete_station(node_t* node){
    node_t* y;
    node_t* x;
    if(root!= nil){
        if(node->left==nil || node->right==nil){
            y=node;
        }
        else y= rb_successore(node);
        if(y->left!= nil){
            x=y->left;
        }
        else x=y->right;
        x->parent=y->parent;
        if(y->parent==nil){
            root=x;
        }
        else if(y==y->parent->left){
            y->parent->left=x;
        }
        else y->parent->right=x;
        if(y!= node){
            node->station=y->station;
        }
        if(y->color==BLACK){
            rb_delete_fixup(x);
        }
        if(y != nil){
            free(y);
        }}
}
void inordertreewalkavanti(node_t* part,node_t* arrivo){
    part= rb_successore(part);
    while(part->station<arrivo->station){
        part->visited=0;
        part= rb_successore(part);
    }
}
void inordertreewalkindietro(node_t* part,node_t* arrivo){
    part= rb_predecessore(part);
    while(part->station>arrivo->station){
        part->visited=0;
        part= rb_predecessore(part);
    }
}

// cars functions
void rottama_auto(int station, int autonomy){
    node_t* node= rb_search(station);
    if(node!= nil){
        rimozione(node,autonomy);
    }
}
void aggiungi_auto(int station, int autonomy){
    node_t* node= rb_search(station);
    if(node!= nil){
        node->max=insert_car(autonomy,&(node->cars),node->max);
        printf("aggiunta\n");
    }
    else printf("non aggiunta\n");
}

// functions station
int percorso_ricorsivo(testa_percorso** thispercorso,node_t* node,int dist,int arrivo){
    node_t* next_tappa=node;
    int dist_max=node->max+node->station;
    //controllo se è ultimo giro o meno
    if(dist==2){
        int find=0;
        if(node->visited==1){
            //so che questo nodo non mi porta a destinazione
            //delete_last_insert(&thispercorso,1);
            return find;
        }
        else{
            next_tappa= rb_successore(next_tappa);
            //controllo solo ultimo giro senza ricorsione
            while(next_tappa->station<=dist_max && find==0){
                int arrived=next_tappa->station+next_tappa->max;
                if(arrived>arrivo){
                    percorso *prova=(*thispercorso)->pointer_pos;
                    if(prova==NULL){
                        insert_tappa(next_tappa->station,&prova);
                        (*thispercorso)->pointer_pos=prova;
                        find=1;
                    }
                    else{
                        if(next_tappa->station<prova->station){
                            while(prova!=NULL){
                                percorso* cancel=prova;
                                prova=prova->prec;
                                free(cancel);
                            }
                            prova=NULL;
                            insert_tappa(next_tappa->station,&prova);
                            (*thispercorso)->pointer_pos=prova;
                            find=1;
                        }
                        else if(next_tappa->station==prova->station){
                            insert_tappa(next_tappa->station,&temp);
                            find=1;
                        }
                        else{
                            find=0;
                        }
                    }
                    //add_successor( next_tappa->station, &prova, 0);
                }
                next_tappa= rb_successore(next_tappa);
            }
            if(find==0){
                node->visited=1;
            }
            return find;
        }
    }
    else {
        int find=0;
        next_tappa= rb_successore(next_tappa);
        while(next_tappa->station<arrivo && next_tappa->station<=dist_max){
            int continuo=0;
            //passo per i successori di node
            //add_successor(next_tappa->station, &thispercorso,1);
            if((*thispercorso)->pointer_pos!=NULL){
                percorso* pos=(*thispercorso)->pointer_pos;
                if(next_tappa->station>=pos->station){
                    continuo=1;
                }
            }
            if(continuo==0){
                testa_percorso* tent=(*thispercorso);
                if(percorso_ricorsivo(&tent,next_tappa,dist-1,arrivo)==1){
                    if(temp!=NULL){
                        percorso* pos=(*thispercorso)->pointer_pos;
                        while(pos->prec!=NULL && pos->prec->station==next_tappa->station){
                            pos=pos->prec;
                        }
                        if(pos->station<next_tappa->station){
                            pos=temp;
                            while(pos!=NULL){
                                percorso* cancel=pos;
                                pos=pos->prec;
                                free(cancel);
                            }
                            temp=NULL;
                        }
                        else{
                            percorso* cancel=(*thispercorso)->pointer_pos;
                            while(cancel!=NULL){
                                percorso* cancello=cancel;
                                cancel=cancel->prec;
                                free(cancello);
                            }
                            temp=NULL;
                        }
                    }
                    else{
                        insert_tappa(next_tappa->station,&((*thispercorso)->pointer_pos));
                    }
                    find=1;
                }
                else{
                    find=0;
                }
            }
            next_tappa= rb_successore(next_tappa);
        }
        if(find==0){
            return 0;
        }
        else return 1;
    }
}
void pianifica_percorso(int part,int arrivo){
    testa_percorso* final=NULL;
    int dist=0;
    node_t* partenza= rb_search(part);
    node_t* dest= rb_search(arrivo);
    if(dest!= nil && partenza!= nil){
        node_t* node= partenza;
        if(part<arrivo){
            int arrived=partenza->station+partenza->max;
            if(arrived>arrivo){
                printf("%d %d\n",part,arrivo);
            }
            else{
                int actual_dist=1;
                while(dist==0 && node!=dest){
                    node= rb_successore(partenza);
                    if(node->station<=arrived){
                        while(node->station<=arrived && node!=dest){
                            int find=0;
                            if(actual_dist==1){
                                int dist_max_caseone=node->max+node->station;
                                if(dist_max_caseone>=arrivo){
                                    find=1;
                                    printf("%d %d %d\n",part,node->station,arrivo);
                                    break;
                                }
                            }
                            else{
                                testa_percorso* thispercorso=(testa_percorso*)malloc(sizeof(testa_percorso));
                                thispercorso->station=node->station;
                                thispercorso->pointer_pos=NULL;
                                thispercorso->percorso_succ=NULL;
                                //cerco da node se c'é un percorso a distanza actual_dist e se c'è lo restituisco in last e lo aggiungo alla lista di puntatori
                                find=percorso_ricorsivo(&(thispercorso),node,actual_dist,arrivo);
                                if(find==1){

                                    //aggiungere lista/e a matrice di risultati
                                    insert_percorso(&final,thispercorso,part);
                                }
                                else{
                                    free(thispercorso);
                                }
                            }
                            if(dist==0 && find==1){
                                dist=actual_dist;
                            }
                            node= rb_successore(node);
                        }
                        //incremento la distanza che sto visitando
                        actual_dist++;
                    }else{
                        break;
                    }}
                if(dist==0){
                    printf("nessun percorso\n");
                }
                else{
                    stampa_percorso(&final,arrivo);
                }
            }
            inordertreewalkavanti(partenza,dest);
        }
        else{
        }
    }
    else printf("nessun percorso\n");
}
void aggiungi_stazione(int station,int numcar,int autonomie[]){
    node_t* node= rb_search(station);
    if(node == nil){
        node_t* node= createNode(station,autonomie,numcar);
        insert_station(node);
        printf("aggiunta\n");
    }
    else printf("non aggiunta\n");
}
void demolisci_stazione(int station){
    node_t* node= rb_search(station);
    if(node != nil){
        delete_station(node);
        printf("demolita\n");
    }
    else printf("non demolita\n");
}

int main(int argc, char * argv[]) {
    nil = (node_t *)malloc(sizeof(node_t));
    nil->color=BLACK;
    root=nil;
    char *insert=(char*)malloc(16 * sizeof(char));
    char str1[] = "aggiungi-stazione",str2[] = "aggiungi-auto",str4[] = "rottama-auto",str5[] = "demolisci-stazione";
    while(scanf("%s",insert)== 1){
        if (strcmp(insert, str1) == 0) {
            //aggiungi_stazione
            int val1,val2;
            scanf("%d",&val1);
            scanf("%d",&val2);
            if(val2!=0){
                int *cars=(int*)malloc(val2 * sizeof(int));
                for (int i = 0; i < val2; i++) {
                    int car;
                    scanf("%d",&car);
                    cars[i] = car;
                }
                aggiungi_stazione(val1, val2, cars);
            }
            else aggiungi_stazione(val1,val2,NULL);
        } else if (strcmp(insert, str2) == 0) {
            //aggiungi_auto
            int val1,val2;
            scanf("%d",&val1);
            scanf("%d",&val2);
            aggiungi_auto(val1, val2);
        } else {
            if(strcmp(insert, str4) == 0){
                //rottama_auto
                int val1,val2;
                scanf("%d",&val1);
                scanf("%d",&val2);
                rottama_auto(val1,val2);
            }
            else if(strcmp(insert, str5) == 0){
                //demolisci_stazione
                int val1;
                scanf("%d",&val1);
                demolisci_stazione(val1);
            }
            else{
                //pianifica_percorso
                int val1,val2;
                scanf("%d",&val1);
                scanf("%d",&val2);
                pianifica_percorso(val1, val2);
            }
        }
    }
    stampa(root);
    return 0;
}

