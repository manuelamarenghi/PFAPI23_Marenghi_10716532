#include <stdio.h>
#include <malloc.h>

typedef enum { RED, BLACK } Color;

typedef struct cars{
    int x;
    struct cars *prec, *succ;
}car_station;

typedef struct stations{
    int station;
    struct stations  *percorso_succ;
    struct stations  *tappa_succ;
    struct stations  *next;
    struct stations  *prec;
}percorso;

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

void add_successor(percorso** this_percorso,int station,int sposta){
    percorso* newSucc=(percorso*)malloc(sizeof(percorso));
    newSucc->tappa_succ=NULL;
    newSucc->prec=NULL;
    newSucc->station=station;
    if(*this_percorso==NULL){
        (*this_percorso)=newSucc;
        (*this_percorso)->percorso_succ=NULL;
    }
    else{
        if((*this_percorso)->tappa_succ==NULL){
            (*this_percorso)->tappa_succ=newSucc;
            newSucc->next=NULL;
        }
        else{
            newSucc->next=(*this_percorso)->tappa_succ;
            ((*this_percorso)->tappa_succ)=newSucc;
        }
        if(sposta==1){
            newSucc->prec=*(this_percorso);
            *(this_percorso)=newSucc;
        }
    }
}
void insert_percorso(percorso* station,percorso** last){
    percorso* head=last;
    if(head==NULL){
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
void delete_last_insert(percorso** this_percorso,int delete){
    percorso* cancel=*(this_percorso);
    (*this_percorso)=(*this_percorso)->prec;
    if(delete==1){
        free(cancel);
    }
}

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
    for(int i=0;i<dim;i++){
        newNode->max= insert_car(cars[i],&head,newNode->max);
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
      printf("station %d ",x->station);
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
int percorso_ricorsivo(percorso** this_percorso,node_t* node,int dist,int arrivo){
    node_t* next_tappa=node;
    int dist_max=node->max+node->station;
    //controllo se è ultimo giro o meno
    if(dist==2){
        int find=0;
        if(node->visited==1){
           //so che questo nodo non mi porta a destinazione
           delete_last_insert(&(this_percorso),1);
            return find;
        }
        else{
            next_tappa= rb_successore(next_tappa);
            //controllo solo ultimo giro senza ricorsione
            while(next_tappa->station>dist_max){
                if(next_tappa->station>arrivo){
                    add_successor( &(this_percorso), next_tappa->station, 0);
                    find=1;
                }
            }
            if(find==0){
                node->visited=1;
            }
            return find;
        }
    }
    else {
        int find;
        next_tappa= rb_successore(next_tappa);
        while(next_tappa->station<dist_max){
           //passo per i successori di node
           add_successor(this_percorso,next_tappa->station,1);
           if(percorso_ricorsivo(&(this_percorso),next_tappa,dist-1,arrivo)==1){
               find=1;
           }
           else{
               find=0;
           }
        }
        if(find==0){
            delete_last_insert(&(this_percorso),1);
            return 0;
        }
        else{
            delete_last_insert(&(this_percorso),0);
            return 1;
        }
    }
}
int restituisci_percorsi(percorso** last,node_t* node,int dist,int arrivo){
    int find=0;
    node_t* next_tappa= rb_successore(node);
    //esiste un percorso partendo da node facendo max #dist giri? se si lo salvo in this_percorso e aggiungo this percorso a tappe_succ di last
    int i=1;
    int dist_max=node->max+node->station;
    while(next_tappa->station<dist_max){
        //this_percorso diventerà successivo di last se ho trovato 1 o + percorsi per quella via
        percorso* this_percorso=NULL;
        //per ogni staz adiacente partendo dal succ di node cerco pecorsi fino a visitare #dist stazioni a percorso
        percorso_ricorsivo(&(this_percorso),next_tappa,dist,arrivo);
        if(this_percorso!=NULL){
            //se ho trovato almeno un percorso da this_percorso lo aggiungo a tappa_succ di last e metto flag a 1
            insert_percorso(&(this_percorso),&(last));
            find=1;
        }
        //passo al successivo adiacente di node
        i++;
        next_tappa=rb_successore(next_tappa);

    }
    return find;
}
void pianifica_percorso(int part,int arrivo){
    percorso* final;
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
         while(dist==0){
             node= rb_successore(node);
            while(node->station<=arrived){
                int find;
                if(actual_dist==1){
                    int dist_max_caseone=node->max+node->station;
                    if(dist_max_caseone>arrivo){
                        find=1;
                        printf("%d %d %d\n",part,node->station,arrivo);
                        break;
                    }
                }
                else{
                    //cerco da node se c'é un percorso a distanza actual_dist e se c'è lo restituisco in last e lo aggiungo alla lista di puntatori
                    find=restituisci_percorsi(&final,node,actual_dist-1,arrivo);
                }
                if(find==1){
                    //aggiungere lista/e a matrice di risultati
                }
                if(dist==0 && find==1){
                    dist=actual_dist;
                }
            }
            //incremento la distanza che sto visitando
            actual_dist++;
        }}}
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

int main() {
    int cars[]={150,2,4,3,2};
    int staz[]={1340,256,180,43,10,5,1,3};
    nil = (node_t *)malloc(sizeof(node_t));
    nil->station=NULL;
    nil->color=BLACK;
    root=nil;
    for(int i=0;i<8;i++){
        int x=staz[i];
        node_t* node= createNode(x,cars,5);
        insert_station(node);
    }
    int aut[]={80,3,2,16,};
    aggiungi_stazione(253,4,aut);
    percorso* this_percorso=NULL;
    add_successor(&(this_percorso),10,0);
    add_successor(&(this_percorso),20,1);
    add_successor(&(this_percorso),43,0);
    add_successor(&(this_percorso),59,0);
    delete_last_insert(&(this_percorso),0);
    add_successor(&(this_percorso),12,1);
    add_successor(&(this_percorso),122,0);
    add_successor(&(this_percorso),122,0);
    return 0;
}
