#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RED, BLACK } Color;

typedef struct cars{
    int x;
    struct cars *prec, *succ;
}car_station;

typedef struct _node{
    char color;
    struct _node *left, *right, *parent;
    int station;
    car_station *cars;
    int max;
    int num_cars;
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
        newCar->succ=(*head);
        newCar->prec=NULL;
        (*head)->prec=newCar;
        (*head)=newCar;
        if(car>max)
            return car;
        else return max;
    }
}
void stampa_car(node_t* node){
    car_station* head=node->cars;
    while(head!=NULL){
        printf("macchina %d\n",head->x);
        head=head->succ;
    }
}
void rimozione(node_t* node,int autonomy){
    int change_max=0;
    int res=0;
    int dim=node->num_cars;
    car_station *car=node->cars;
    car_station *p=NULL;
    if (dim != 0){
        if (autonomy == node->max) {  change_max = 1; }
        while (car != NULL && res!=2){
            if (car->x == autonomy){
                if(res == 0){
                    p=car;
                    if(car->prec!=NULL)
                        car->prec->succ = car->succ;
                    else node->cars=car->succ;
                    if(car->succ!=NULL)
                        car->succ->prec = car->prec;
                    node->num_cars--;
                    printf("rottamata\n");
                    if(change_max==0){
                        res=2;
                    }
                    else res = 1;
                }
                else{
                    node->max=autonomy;
                    res=2;
                }
            }
            if (change_max == 1 && res==1){
                if (car->prec == NULL || node->max < car->x){
                    node->max = car->x;
                }
            }
            car = car->succ;
        }
        if (res == 0){
            printf("non rottamata\n");
        }
        else free(p);
    }
    else printf("non rottamata\n");
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
    newNode->max=0;
    car_station* head=NULL;
    for(int i=0;i<dim;i++){
        newNode->max= insert_car(cars[i],&head,newNode->max);
    }
    newNode->cars=head;
    return newNode;
}
node_t *createNode_noauto(int station){
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->station = station;
    newNode->color = RED;
    newNode->parent = nil;
    newNode->left = nil;
    newNode->right = nil;
    newNode->num_cars=0;
    newNode->max=0;
    car_station* head=NULL;
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
        int y=x->max;
        printf("station %d max  %d",x->station,y);
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
    node_t* w = nil;
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
    node_t* y = nil;
    node_t* x = nil;
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
            node->max=y->max;
            node->num_cars=y->num_cars;
            node->cars=y->cars;
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
    else printf("non rottamata\n");
}
void aggiungi_auto(int station, int autonomy){
    node_t* node= rb_search(station);
    if(node!= nil){
        node->max=insert_car(autonomy,&(node->cars),node->max);
        node->num_cars=node->num_cars+1;
        printf("aggiunta\n");
    }
    else printf("non aggiunta\n");
}

// station functions
int percorsoricorsivoavanti(node_t* partenza,int arrivo,int dstmax){
    int node=partenza->station;
    node_t* nodo = rb_successore(partenza);
    int p=partenza->station;
    int tot=0;
    int i=0,j=-2;
    int find=0;
    int x=-1;
    while(partenza->station!=arrivo){
        tot++;
        partenza= rb_successore(partenza);
    }
    int autonomie[tot];
    int station[tot];
    int distanze[tot];
    for(int y=0;y<tot;y++){
        station[y]=-1;
        distanze[y]=0;
        autonomie[y]=0;
    }
    do{     int count=0;
        while(find!=1 && nodo->station<=dstmax){
            station[i]=nodo->station;
            autonomie[i]=nodo->station+nodo->max;
            distanze[i]=x;
            count++;
            i++;
            if(nodo->station==arrivo){
                find=1;
                if(j==-2){
                    printf("%d %d\n",p,nodo->station);
                    return 1;
                }
            }
            else nodo= rb_successore(nodo);
        }
        if(j==-2 && i==0){
            return 0;
        }
        else j++;
        x++;
        if(count==0 && station[x]==-1){
            return 0;
        }
        node=station[x];
        dstmax=autonomie[x];
    }while(find==0 && node!=arrivo);
    if(find==1){
        i=i-1;
        int finale[i],z=0;
        while(i>=0){
            finale[z]=station[i];
            z++;
            i=distanze[i];
        }
        printf("%d ",p);
        z=z-1;
        while(z>=0){
            printf("%d ",finale[z]);
            z--;
        }
        printf("\n");
    }
    return find;
}
int percorsoricorsivoindietro(node_t* partenza,int arrivo,int dstmax){
    int node=partenza->station;
    node_t* nodo = rb_predecessore(partenza);
    int p=partenza->station;
    int tot=0;
    int i=0,j=-2;
    int find=0;
    int x=-1;
    while(partenza->station!=arrivo){
        tot++;
        partenza= rb_predecessore(partenza);
    }
    int autonomie[tot];
    int station[tot];
    int distanze[tot];
    for(int y=0;y<tot;y++){
        station[y]=-1;
        distanze[y]=0;
        autonomie[y]=0;
    }
    do{
        int count=x+1;
        if(find==1){
            if(arrivo>=dstmax){
                count=distanze[tot-1];
                int t=0,w=0;
                int l=x;
                while(count!=-1 || l!=-1){
                    if(count!=-1){
                        t++;
                        count=distanze[count];
                    }
                    if(l!=-1){
                        w++;
                        l=distanze[l];
                    }
                }
                if(w<=t){
                    distanze[tot-1]=x;
                }
            }
        }
        count=x+1;
        while(count<tot && station[count]!=-1 && station[count]>=dstmax){
            int indix=distanze[count];
            if(indix != -1){
                if(station[indix]>node && distanze[indix]==distanze[x]){
                    distanze[count]=x;
                }
                else{
                    j=distanze[count];
                    int t=0,w=0;
                    int l=x;
                    while(j!=-1 || l!=-1){
                        if(j!=-1){
                            t++;
                            j=distanze[j];
                        }
                        if(l!=-1){
                            w++;
                            l=distanze[l];
                        }
                    }
                    if(w<=t){
                        distanze[count]=x;
                    }
                }
            }
            count++;
        }
        count=0;
        while(find!=1 && nodo->station>=dstmax){
            station[i]=nodo->station;
            int d=nodo->station-nodo->max;
            if(d<=0){
                autonomie[i]=0;
            }
            else autonomie[i]=d;
            distanze[i]=x;
            count++;
            i++;
            if(nodo->station==arrivo){
                find=1;
            }
            else nodo= rb_predecessore(nodo);
        }
        if(j==-2 && i==0){
            return 0;
        }
        else j++;
        x++;
        if(count==0 && station[x]==-1){
            return 0;
        }
        node=station[x];
        dstmax=autonomie[x];
    }while(node!=arrivo);
    if(find==1){
        i=i-1;
        int z=0,finale[i];
        while(i>=0){
            finale[z]=station[i];
            z++;
            i=distanze[i];
        }
        printf("%d ",p);
        z=z-1;
        while(z>=0){
            printf("%d ",finale[z]);
            z--;
        }
        printf("\n");
    }
    return find;
}

void pianifica_percorso(int part,int arrivo){
    node_t* partenza= rb_search(part);
    node_t* dest= rb_search(arrivo);
    if(dest!= nil && partenza!= nil){
        if(part<arrivo){
            int dist=partenza->station+partenza->max;
            if(dist>=arrivo){
                printf("%d %d\n",part,arrivo);
            }
            else{
                if(!percorsoricorsivoavanti(partenza,arrivo,dist)){
                    printf("nessun percorso\n");
                }
            }
        }
        else{
            if(part==arrivo){
                printf("%d \n",part);
            }
            else{
                int dist=partenza->station-partenza->max;
                if(dist<=0){
                    printf("%d %d\n",part,arrivo);
                }
                else{
                    if(!percorsoricorsivoindietro(partenza,arrivo,dist)){
                        printf("nessun percorso\n");
                    }
                }
            }
        }
    }
    else printf("nessun percorso\n");
}
void aggiungi_stazione(int station,int numcar,int autonomie[]){
    node_t* node= rb_search(station);
    if(node == nil){
        node_t* n = createNode(station,autonomie,numcar);
        insert_station(n);
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
void aggiungi_stazione_noauto(int station){
    node_t* node= rb_search(station);
    if(node == nil){
        node_t* n = createNode_noauto(station);
        insert_station(n);
        printf("aggiunta\n");
    }
    else printf("non aggiunta\n");
}

int main(int argc, char * argv[]){
    nil = (node_t *)malloc(sizeof(node_t));
    nil->color=BLACK;
    root=nil;
    char *insert=(char*)malloc(20 * sizeof(char));
    char str1[] = "aggiungi-stazione",str2[] = "aggiungi-auto",str4[] = "rottama-auto",str5[] = "demolisci-stazione";
    int val1=0,val2=0;
    while(scanf("%s",insert)== 1){
        if (strcmp(insert, str1) == 0) {
            //aggiungi_stazione
            if(scanf("%d",&val1) && scanf("%d",&val2)){
                if(val2!=0){
                    int cars[val2];
                    for (int i = 0; i < val2; i++){
                        if(!scanf("%d",&cars[i])){
                            return 0;
                        }
                    }
                    aggiungi_stazione(val1, val2, cars);
                }
                else aggiungi_stazione_noauto(val1);
            }
        } else if (strcmp(insert, str2) == 0){
            //aggiungi_auto
            if(scanf("%d",&val1) && scanf("%d",&val2)){
                aggiungi_auto(val1, val2);
            }
        } else {
            if(strcmp(insert, str4) == 0){
                //rottama_auto
                if(scanf("%d",&val1) && scanf("%d",&val2)){
                    rottama_auto(val1,val2);
                }
            }
            else if(strcmp(insert, str5) == 0){
                //demolisci_stazione
                if(scanf("%d",&val1)){
                    demolisci_stazione(val1);
                }
            }
            else{
                //pianifica_percorso
                if(scanf("%d",&val1) && scanf("%d",&val2)){
                    pianifica_percorso(val1, val2);
                }
            }
        }
    }
    return 0;
}
