#include <stdio.h>

typedef struct TlistItem listItem;

struct TlistItem {

listItem *children;
listItem *previous;
listItem *next;
char name[20];
int index;

};

void Tconstructor(Tobject *ptrone){
Tobject *this = ptrone;
this->value=0;
this->myaddress=ptrone;
}

void Tincrement(Tobject *ptrone, int tvalue){
Tobject *this = ptrone;
this->value = this->value+tvalue;
}


void Tdecrement(Tobject *ptrone, int tvalue){
Tobject *this = ptrone;
this->value = this->value-tvalue;
}


int Tevaluate(Tobject *ptrone){
Tobject *this;
this = ptrone;
return this->value;

}



int new(Tobject *obj){
obj->constructor = &Tconstructor;
obj->evaluate    = &Tevaluate;
obj->increment   = &Tincrement;
obj->decrement   = &Tdecrement;

obj->constructor(obj);

return 1;
}


int main()
{

Tobject object;
Tobject object2;

new(&object);
new(&object2);

object.increment(&object,3);
object2.increment(&object2,99);

printf("%d\n", object.evaluate(&object) );
printf("%d\n", object2.evaluate(&object2) );

return 0;
}
