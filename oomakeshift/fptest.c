#include <stdio.h>

typedef struct Tobj Tobject;

struct Tobj {
Tobject *self;
int value;
void (*constructor)(Tobject *ptrone);
void (*increment)(Tobject *ptrone, int tvalue);
void (*decrement)(Tobject *ptrone, int tvalue);
int (*evaluate)(Tobject *ptrone);
};

void Tconstructor(Tobject *ptrone){
Tobject *this = ptrone;
this->value=0;
this->self=ptrone;
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
Tobject object3;

new(&object);
new(&object2);
new(&object3);

object.increment(object.self,3);
object2.increment(object2.self,99);
object2.increment(object3.self,1);

printf("%d\n", object.evaluate(object.self) );
printf("%d\n", object2.evaluate(object2.self) );

object.increment(object.self,3);
object2.increment(object3.self,1);
printf("%d\n", object.evaluate(object.self) );
printf("%d\n", object3.evaluate(object3.self) );
return 0;
}
