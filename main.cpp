#include <stdio.h>
#include<windows.h>
#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
#include <list>
using namespace std;

#define IZQUIERDA  75
#define DERECHA  77
#define ARRIBA  72
#define ABAJO  80

void gotoxy(int x, int y){  //imprime los "*" con coordenadas que nosotros le damos
	HANDLE hcon;
	
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	
	SetConsoleCursorPosition(hcon,dwPos);
}

void ocultarCursor(){
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize =2;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hcon,&cci);


}

void pintar_limites(){
	for(int i=2;i<78;i++){
		gotoxy(i,3); printf("%c",205);
		gotoxy(i,33); printf("%c",205);
	}
	
	for (int i=4; i<33; i++){
		gotoxy(2,i); printf("%c",186);
		gotoxy(77,i); printf("%c",186);
	}

	gotoxy(2,3); printf("%c",201);
	gotoxy(2,33); printf("%c",200);
	gotoxy(77,3); printf("%c",187);
	gotoxy(77,33); printf("%c",188);
}

class NAVE{
int x,y,corazones, vidas;
public:
	NAVE(int _x,int _y,int _corazaones, int _vidas):x(_x),y(_y),corazones(_corazaones), vidas(_vidas){}
	int X(){return x;}
	int Y(){return y;}
	void COR(){ corazones--;}
	void pintar();
	void borrar();
	void mover();
	void pintar_corazones();
	void morir();
	int VID(){return vidas;}
};

void NAVE::pintar_corazones() {
	gotoxy (50,2); printf("VIDAS %d", vidas);
	gotoxy(62,2);printf("vida");
	gotoxy(70,2);printf("    ");
	for (int i=0;i<corazones;i++){
		gotoxy(70+i,2);printf("%c",3);
	}
}	

void NAVE::morir(){
	if(corazones==0){
		borrar();
		gotoxy(x,y);   printf("   **   ");
		gotoxy(x,y+1); printf("  ****  ");
		gotoxy(x,y+2); printf("   **   ");
		Sleep(200);
		borrar();
		gotoxy(x,y);   printf(" * ** *");
		gotoxy(x,y+1); printf("  **** ");
		gotoxy(x,y+2); printf(" * ** *");
		Sleep(200);
		borrar();
		vidas--;
		corazones =3;
		pintar_corazones();
		pintar();
	}
}


void NAVE::pintar(){
	gotoxy(x,y); printf("  %c",30);
	gotoxy(x,y+1); printf(" %c%c%c",40,207,41);
	gotoxy(x,y+2); printf("%c%c %c%c",30,190,190,30);
}

void NAVE::borrar(){
	gotoxy(x,y);   printf("        ");
	gotoxy(x,y+1); printf("        ");
	gotoxy(x,y+2); printf("        ");
}
void NAVE::mover(){
	if(kbhit()){
			
			char tecla= getch();
			borrar();
			if(tecla == IZQUIERDA && x>3) x--;
			if(tecla == DERECHA && x+5<77) x++;
			if(tecla == ARRIBA && y>4) y--;
			if(tecla == ABAJO && y+3 <33) y++;
			pintar();
			pintar_corazones();
		}
}

class AST{
	int x,y;
	
	public:
		int X(){return x;}
		int Y(){return y;}
		AST(int _x,int _y):x(_x),y(_y){}
		void choque(class NAVE &N);
		void pintar();
		void mover();
};

void AST::pintar(){
	gotoxy (x,y); printf("%c",184);
}

void AST::mover(){
	gotoxy(x,y); printf(" ");
	y++;
	if(y>32){
		x=rand()%71 + 4;
		y =4; 
	}	
	pintar();
}

void AST::choque(class NAVE &N){
	if(x >=N.X() && x<N.X()+6 && y >=N.Y()&&y<N.Y()+2){ //toda la nave
		N.COR();	
		N.borrar();
		N.pintar();
		N.pintar_corazones();
		x=rand()%71 + 4;
		y =4;
	}
}

class BALA{
	int x,y;
	public:
		BALA(int _x,int _y): x(_x), y(_y){}
		int X(){return x;}
		int Y(){return y;}
		void mover();
		bool fuera();
};

void BALA::mover(){
	gotoxy (x,y); printf(" ");
	y--;
	gotoxy (x,y); printf("*");
}

bool BALA::fuera(){
	if(y==4) return true;
	return false;
}

int main() {
	ocultarCursor();
	pintar_limites();
	NAVE nave(37,30,3,3);
	nave.pintar();
	nave.pintar_corazones();
	list<BALA*>B;
	list<BALA*>:: iterator it;
	
	list<AST*> A;
	list<AST*>:: iterator itA;
	for (int i=0;i<5; i++){
		A.push_back(new AST (rand()%75+3, rand()%5+4));
	}
	
	bool game_over = false;
	int puntos =0;
	
	
	while (!game_over){	
		gotoxy(4,2); printf("Puntos %d", puntos);
		
		if(kbhit()){
			char tecla = getch();
			if(tecla == 'z')
			B.push_back(new BALA(nave.X()+2, nave.Y()-1));
		}
		
	for (it = B.begin(); it != B.end() ; it++){
		 //it es la direccion y *it accede a toda la informacion
		 (*it)->mover();
		 if((*it)->fuera()){
		 	gotoxy((*it)->X(), (*it)->Y()); printf(" ");
		 	delete (*it); //borramos la bala
		 	it= B.erase(it); 
		 }
	}
	
		
	
	for(itA = A.begin(); itA != A.end(); itA++){
		(*itA) -> mover();
		(*itA) -> choque(nave);
	}
	
	for(itA = A.begin(); itA != A.end(); itA++){
		for (it = B.begin(); it != B.end() ; it++){
			if((*itA)->X() == (*it)->X() && ((*itA)->Y()+1 == (*it)->Y() || (*itA) -> Y() == (*it)->Y())){
			gotoxy((*it)->X(), (*it)->Y()); printf(" ");
		 	delete (*it); //borramos la bala
		 	it= B.erase(it);
		 	
		 	A.push_back(new AST (rand()% 74+3,4));
		 	gotoxy((*itA)->X(), (*itA)->Y()); printf(" ");
		 	delete (*itA);
		 	itA= A.erase(itA);
		 	
		 	puntos++;
			}
		}
	}
	
	if(nave.VID()==0){game_over=true;}
	nave.morir();
	nave.mover();
	
	Sleep(30);
	}
	
	system("cls");
	gotoxy(25,10); printf("GRACIAS POR JUGAR! GAME OVER");
	return 0;
	
}
