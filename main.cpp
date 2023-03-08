#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;

const int n=8;
const int width=87;
const int offsetx=10;
const int offsety=10;

int potez=-1;//beli zapocinju igru

bool rookA1=false;
bool rookH1=false;
bool rookA8=false;
bool rookH8=false;

bool whiteKingHasMoved=false;
bool blackKingHasMoved=false;

sf::RenderWindow window(sf::VideoMode(n*width, n*width), "CHESS");

//deklaracija belih figura
sf::Image whitePawn;
sf::Image whiteRook;
sf::Image whiteKnight;
sf::Image whiteBishop;
sf::Image whiteQueen;
sf::Image whiteKing;

//deklaracija crnih figura
sf::Image blackPawn;
sf::Image blackRook;
sf::Image blackKnight;
sf::Image blackBishop;
sf::Image blackQueen;
sf::Image blackKing;

vector<vector<int>> startna_poz={{2,3,4,5,6,4,3,2},			//pozitivne figure su crni
								 {1,1,1,1,1,1,1,1},
								 {0,0,0,0,0,0,0,0},
								 {0,0,0,0,0,0,0,0},
								 {0,0,0,0,0,0,0,0},
								 {0,0,0,0,0,0,0,0},
								 {-1,-1,-1,-1,-1,-1,-1,-1},
								 {-2,-3,-4,-5,-6,-4,-3,-2}};//negativne figure su beli

class table{
	vector<vector<int>> tabla;
	
	//pojedena figura
	int taken = 0;
	
	public:
		table();
	
		bool rookMove(int istart, int jstart, int iwanted, int jwanted);
		bool knightMove(int istart, int jstart, int iwanted, int jwanted);
		bool bishopMove(int istart, int jstart, int iwanted, int jwanted);
		bool queenMove(int istart, int jstart, int iwanted, int jwanted);

		bool blackAttack(int ia, int ja);
		bool whiteAttack(int ia, int ja);

		bool whiteKingShortCastle(int istart, int jstart, int iwanted, int jwanted);
		bool whiteKingQueenCastle(int istart, int jstart, int iwanted, int jwanted);
		bool blackKingShortCastle(int istart, int jstart, int iwanted, int jwanted);
		bool blackKingQueenCastle(int istart, int jstart, int iwanted, int jwanted);

		bool whiteKingMove(int istart, int jstart, int iwanted, int jwanted);
		bool blackKingMove(int istart, int jstart, int iwanted, int jwanted);

		bool whiteKingCheck();
		bool blackKingCheck();
		
		bool whiteKingMated();
		bool blackKingMated();
		
		bool whitePawnMove(int istart, int jstart, int iwanted, int jwanted);
		bool blackPawnMove(int istart, int jstart, int iwanted, int jwanted);
		bool whitePawnAttack(int istart, int jstart, int iwanted, int jwanted);
		bool blackPawnAttack(int istart, int jstart, int iwanted, int jwanted);
		
		int dist(int ib, int jb, int iw, int jw);
		int figura(int x, int y);

		void dodela(int figura, int x, int y);
		void kingSafe(int uhvacenaFigura, int pocX, int pocY, int kordX, int kordY);
		void promotion(int who, int x, int y);
		void krajIgre();
		void print();
};

table::table(){
	//1-pawn
	//2-rook
	//3-knight
	//4-bishop
	//5-queen
	//6-king
	tabla = startna_poz;
}	

int table::dist(int ib, int jb, int iw, int jw){
	int distance=sqrt((ib-iw)*(ib-iw) + (jb-jw)*(jb-jw));
	return distance;
}
int table::figura(int x, int y){
	return tabla[y][x];
}

bool table::rookMove(int istart, int jstart, int iwanted, int jwanted){
	//provera da ne skoci na sebe i provera za destinaciju topa
	if((istart==iwanted && jstart==jwanted) || (istart!=iwanted && jstart!=jwanted)){
		return false;
	}

	//provera za kretanje topa po horizontali
	if(istart==iwanted){
		if(jwanted>jstart){
			for(int j=jstart+1;j<jwanted;j++){
				if(tabla[istart][j]!=0){
					return false;
				}
			}
		}else{
			for(int j=jstart-1;j>jwanted;j--){
				if(tabla[istart][j]!=0){
					return false;
				}
			}
		}
	}

	//provera za kretanje topa po vertikali
	if(jstart==jwanted){
		if(iwanted>istart){
			for(int i=istart+1;i<iwanted;i++){
				if(tabla[i][jstart]!=0){
					return false;
				}
			}
		}else{
			for(int i=istart-1;i>iwanted;i--){
				if(tabla[i][jstart]!=0){
					return false;
				}
			}
		}
	}

	return true;
}
bool table::knightMove(int istart, int jstart, int iwanted, int jwanted){
	//provera za kretanje konja
	if(iwanted==istart+1 && jwanted==jstart+2)return true;
	else if(iwanted==istart+1 && jwanted==jstart-2)return true;
	else if(iwanted==istart-1 && jwanted==jstart-2)return true;
	else if(iwanted==istart-1 && jwanted==jstart+2)return true;
	else if(iwanted==istart+2 && jwanted==jstart-1)return true;
	else if(iwanted==istart+2 && jwanted==jstart+1)return true;
	else if(iwanted==istart-2 && jwanted==jstart-1)return true;
	else if(iwanted==istart-2 && jwanted==jstart+1)return true;

	return false;
}
bool table::bishopMove(int istart, int jstart, int iwanted, int jwanted){
	//provera da ne skoci na sebe i provera destinacije lovca
	if((istart==iwanted && jstart==jwanted) || abs(jwanted-jstart)!=abs(iwanted-istart)){
		return false;
	}

	//provera za kretanje lovca
	if(jwanted>jstart){
		if(iwanted<istart){
			for(int k=1;k<abs(iwanted-istart);k++){
				if(tabla[istart-k][jstart+k]!=0){
					return false;
				}
			}
		}else{
			for(int k=1;k<abs(iwanted-istart);k++){
				if(tabla[istart+k][jstart+k]!=0){
					return false;
				}
			}
		}
	}else{
		if(iwanted<istart){
			for(int k=1;k<abs(iwanted-istart);k++){
				if(tabla[istart-k][jstart-k]!=0){
					return false;
				}
			}
		}else{
			for(int k=1;k<abs(iwanted-istart);k++){
				if(tabla[istart+k][jstart-k]!=0){
					return false;
				}
			}
		}
	}

	return true;
}
bool table::queenMove(int istart, int jstart, int iwanted, int jwanted){
	if(rookMove(istart, jstart, iwanted, jwanted) || bishopMove(istart, jstart, iwanted, jwanted)){
		return true;
	}
	return false;
}

bool table::whiteAttack(int ia, int ja){
	//negativne figure napadaju
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(tabla[i][j]==-1){
				if(whitePawnAttack(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==-2){
				if(rookMove(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==-3){
				if(knightMove(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==-4){
				if(bishopMove(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==-5){
				if(queenMove(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==-6){
				if(dist(i,j,ia,ja)==1){
					return true;
				}
			}
		}
	}

	return false;
}
bool table::blackAttack(int ia, int ja){
	//pozitivne figure napadaju
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(tabla[i][j]==1){
				if(blackPawnAttack(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==2){
				if(rookMove(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==3){
				if(knightMove(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==4){
				if(bishopMove(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==5){
				if(queenMove(i,j,ia,ja)==true){
					return true;
				}
			}else if(tabla[i][j]==6){
				if(dist(i,j,ia,ja)==1){
					return true;
				}
			}
		}
	}

	return false;
}

bool table::whitePawnAttack(int istart, int jstart, int iwanted, int jwanted){
	if(iwanted==istart-1){
		if(jwanted==jstart-1 || jwanted==jstart+1){
			return true;
		}
	}
	
	return false;
}
bool table::blackPawnAttack(int istart, int jstart, int iwanted, int jwanted){
	if(iwanted==istart+1){
		if(jwanted==jstart-1 || jwanted==jstart+1){
			return true;
		}
	}

	return false;
}
bool table::whitePawnMove(int istart, int jstart, int iwanted, int jwanted){
	//provera za kretanje belih figura
	if(iwanted==istart-1){
		if(whitePawnAttack(istart,jstart,iwanted,jwanted)==true && tabla[iwanted][jwanted]>0 && tabla[iwanted][jwanted]!=6){
			return true;
		}
		if(jstart==jwanted && tabla[iwanted][jwanted]==0){
			return true;
		}
	}

	//pocetni potez pijunom
	if(istart==n-2 && iwanted==istart-2 && jstart==jwanted){
		if(tabla[istart-1][jstart]==0 && tabla[istart-2][jstart]==0){
			return true;
		}
	}

	return false;
}
bool table::blackPawnMove(int istart, int jstart, int iwanted, int jwanted){
	//provera za kretanje crnih figura
	if(iwanted==istart+1){
		if(blackPawnAttack(istart,jstart,iwanted,jwanted)==true && tabla[iwanted][jwanted]<0 && tabla[iwanted][jwanted]!=-6){
			return true;
		}
		if(jstart==jwanted && tabla[iwanted][jwanted]==0){
			return true;
		}
	}

	//pocetni potez pijunom
	if(istart==1 && iwanted==istart+2 && jstart==jwanted){
		if(tabla[istart+1][jstart]==0 && tabla[istart+2][jstart]==0){
			return true;
		}
	}

	return false;
}	

bool table::whiteKingShortCastle(int istart, int jstart, int iwanted, int jwanted){
	if(tabla[n-1][n-3]==0 && tabla[n-1][n-2]==0){
		if(rookH1==false && whiteKingHasMoved==false){
			if(istart==n-1 && jstart==4 && iwanted==n-1 && jwanted==6){
				//pozitivne/crne figure napadaju
				for(int k=4;k<=6;k++){
					if(blackAttack(iwanted,k)==true){
						return false;
					}
				}
				return true;
			}
		}
	}

	return false;
}
bool table::blackKingShortCastle(int istart, int jstart, int iwanted, int jwanted){
	if(tabla[0][n-3]==0 && tabla[0][n-2]==0){
		if(rookH8==false && blackKingHasMoved==false){
			if(istart==0 && jstart==4 && iwanted==0 && jwanted==6){
				//negativne/bele figure napadaju
				for(int k=4;k<=6;k++){
					if(whiteAttack(iwanted,k)==true){
						return false;
					}
				}
				return true;
			}
		}
	}

	return false;
}
bool table::whiteKingQueenCastle(int istart, int jstart, int iwanted, int jwanted){
	if(tabla[n-1][1]==0 && tabla[n-1][2]==0 && tabla[n-1][3]==0){
		if(rookA1==false && whiteKingHasMoved==false){
			if(istart==n-1 && jstart==4 && iwanted==n-1 && jwanted==2){
				//pozitivne/crne figure napadaju
				for(int k=4;k>=2;k--){
					if(blackAttack(iwanted,k)){
						return false;
					}
				}
				return true;
			}
		}
	}

	return false;
}
bool table::blackKingQueenCastle(int istart, int jstart, int iwanted, int jwanted){
	if(tabla[0][1]==0 && tabla[0][2]==0 && tabla[0][3]==0){
		if(rookA8==false && blackKingHasMoved==false){
			if(istart==0 && jstart==4 && iwanted==0 && jwanted==2){
				//negativne/bele figure napadaju
				for(int k=4;k>=2;k--){
					if(whiteAttack(iwanted,k)){
						return false;
					}
				}
				return true;
			}
		}
	}

	return false;
}

bool table::whiteKingMove(int istart, int jstart, int iwanted, int jwanted){
	//provera da ne skoci na samog sebe
	if(iwanted==istart && jwanted==jstart){
		return false;
	}

	//provera da ne pojede svoju figuru
	if(tabla[iwanted][jwanted]<0){
		return false;
	}

	//kralj moze da se pomeri samo jedno mesto u svim smerovima
	if(dist(istart, jstart, iwanted, jwanted)!=1){
		return false;
	}

	//pozitivne/crne figure napadaju
	if(blackAttack(iwanted, jwanted)==true){
		return false;
	}

	return true;
}
bool table::blackKingMove(int istart, int jstart, int iwanted, int jwanted){
	//provera da ne skoci na samog sebe
	if(iwanted==istart && jwanted==jstart){
		return false;
	}

	//provera da ne pojede svoju figuru
	if(tabla[iwanted][jwanted]>0){
		return false;
	}

	//kralj moze da se pomeri samo jedno mesto u svim smerovima
	if(dist(istart, jstart, iwanted, jwanted)!=1){
		return false;
	}

	//negativne/bele figure napadaju
	if(whiteAttack(iwanted, jwanted)==true){
		return false;
	}

	return true;
}

bool table::whiteKingCheck(){
	//pronalazenje belog kralja na tabli
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(tabla[i][j]==-6){
				if(blackAttack(i, j)==true){
					return true;
				}
			}
		}
	}

	return false;
}
bool table::blackKingCheck(){
	//pronalazenje crnog kralja na tabli
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(tabla[i][j]==6){
				if(whiteAttack(i, j)==true){
					return true;
				}
			}
		}
	}

	return false;
}

void table::dodela(int figura, int x, int y){
	taken=tabla[y][x];
	tabla[y][x]=figura;
}
void table::promotion(int who, int x, int y){
	//promocija belog
	if(who==-1){
		if(y==0){
			cout << "Unesite figuru za promociju: ";
			string new_figure = "";
			bool successful = false;
			while(!successful){
				cin >> new_figure;
				if(new_figure=="R"){
					dodela(-2, x, y);
					successful = true;
				}else if (new_figure=="K"){
					dodela(-3, x, y);
					successful = true;
				}else if (new_figure=="B"){
					dodela(-4, x, y);
					successful = true;
				}else if (new_figure=="Q"){
					dodela(-5, x, y);
					successful = true;
				}else{
					cout<<"Neispravan unos! Pokusajte ponovo: ";
				}
			}
		}
	}else if(who==1){
		if(y==n-1){
			cout << "Unesite figuru za promociju: ";
			string new_figure = "";
			bool successful = false;
			while(!successful){
				cin >> new_figure;
				if(new_figure=="R"){
					dodela(2, x, y);
					successful = true;
				}else if (new_figure=="K"){
					dodela(3, x, y);
					successful = true;
				}else if (new_figure=="B"){
					dodela(4, x, y);
					successful = true;
				}else if (new_figure=="Q"){
					dodela(5, x, y);
					successful = true;
				}else{
					cout<<"Neispravan unos! Pokusajte ponovo: ";
				}
			}
		}
	}
}
void drawImage(sf::Image image, int x, int y){
	sf::Vector2u v = image.getSize();
	sf::Texture texture;
	texture.loadFromImage(image);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	
	float scalex=float(width-offsetx*2)/v.x;
	float scaley=float(width-offsety*2)/v.y;
	sprite.scale(scalex, scaley);
	sprite.setPosition(x, y);

	window.draw(sprite);
}
void drawFigure(int figura, int x, int y){
	if(figura==1)drawImage(blackPawn, x, y);
	else if(figura==2)drawImage(blackRook,  x, y);
	else if(figura==3)drawImage(blackKnight,  x, y);
	else if(figura==4)drawImage(blackBishop,  x, y);
	else if(figura==5)drawImage(blackQueen,  x, y);
	else if(figura==6)drawImage(blackKing,  x, y);
	else if(figura==-1)drawImage(whitePawn,  x, y);
	else if(figura==-2)drawImage(whiteRook,  x, y);
	else if(figura==-3)drawImage(whiteKnight,  x, y);
	else if(figura==-4)drawImage(whiteBishop,  x, y);
	else if(figura==-5)drawImage(whiteQueen,  x, y);
	else if(figura==-6)drawImage(whiteKing,  x,y);
}
void table::kingSafe(int uhvacenaFigura, int pocX, int pocY, int kordX, int kordY){
	//beli je na potezu
	if(potez==-1){
		if(whiteKingCheck()==true){
			dodela(taken, kordX, kordY);
			dodela(uhvacenaFigura, pocX, pocY);
			cout<<"Ilegalan potez! Beli kralj je pod sahom."<<endl;
		}else potez*=-1;
	}else{//crni je na potezu
		if(blackKingCheck()==true){
			dodela(taken, kordX, kordY);
			dodela(uhvacenaFigura, pocX, pocY);
			cout<<"Ilegalan potez! Crni kralj je pod sahom."<<endl;
		}else potez*=-1;
	}
}
void table::print(){
	string vertikala="87654321";
	string horizontala="ABCDEFGH";
	
	sf::Font font;
	if(!font.loadFromFile("Roboto-Black.ttf")){
		cout<<"Greska prilikom ucitavanja odgovarajuceg fonta"<<endl;
	}
		
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			//iscrtavanje sahovske table			
			sf::RectangleShape shape;
			shape.setSize(sf::Vector2f(width,width));
			shape.setPosition(i*width,j*width);
			if((i+j)%2==1)shape.setFillColor(sf::Color{0, 125, 0});
			else shape.setFillColor(sf::Color{240, 255, 240});
			window.draw(shape);

			//numerisanje polja sahovske table
			sf::Text text;
			text.setFont(font);
			if(j==0){
				text.setString(vertikala[i]);
				text.setCharacterSize(10);
				text.setFillColor(sf::Color::Black);
				text.setPosition(3, i*width + 3);
				window.draw(text);
			}
			if(i==n-1){
				text.setString(horizontala[j]);
				text.setCharacterSize(10);
				text.setFillColor(sf::Color::Black);
				text.setPosition((j+1)*width - 13, n*width - 13);
				window.draw(text);
			}

			//crtanje figura
			for(int i=0;i<n;i++){
				for(int j=0;j<n;j++){
					if(tabla[i][j]==1)drawImage(blackPawn, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==2)drawImage(blackRook, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==3)drawImage(blackKnight, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==4)drawImage(blackBishop, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==5)drawImage(blackQueen, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==6)drawImage(blackKing, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==-1)drawImage(whitePawn, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==-2)drawImage(whiteRook, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==-3)drawImage(whiteKnight, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==-4)drawImage(whiteBishop, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==-5)drawImage(whiteQueen, j*width + offsetx, i*width + offsety);
					else if(tabla[i][j]==-6)drawImage(whiteKing, j*width + offsetx, i*width + offsety);
				}
			}
		}
	}
}

int main(){

    table Game;

	//ucitavanje belih figura
	whitePawn.loadFromFile("./whiteFigures/whitePawn.png");
	whiteRook.loadFromFile("./whiteFigures/whiteRook.png");
	whiteKnight.loadFromFile("./whiteFigures/whiteKnight.png");
	whiteBishop.loadFromFile("./whiteFigures/whiteBishop.png");
	whiteQueen.loadFromFile("./whiteFigures/whiteQueen.png");
	whiteKing.loadFromFile("./whiteFigures/whiteKing.png");

	//ucitavanje crnih figura
	blackPawn.loadFromFile("./blackFigures/blackPawn.png");
	blackRook.loadFromFile("./blackFigures/blackRook.png");
	blackKnight.loadFromFile("./blackFigures/blackKnight.png");
	blackBishop.loadFromFile("./blackFigures/blackBishop.png");
	blackQueen.loadFromFile("./blackFigures/blackQueen.png");
	blackKing.loadFromFile("./blackFigures/blackKing.png");

	bool dragging = false;

	int mouseX=0;
	int mouseY=0;
	int pocX=0;
	int pocY=0;

	int uhvacenaFigura=0;

	while(window.isOpen()){
		sf::Event event;
		
		while(window.pollEvent(event)){
			
			if(event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::MouseButtonPressed){
				if(event.mouseButton.button == sf::Mouse::Left){
					int kordX=mouseX/width;
					int kordY=mouseY/width;
					uhvacenaFigura=Game.figura(kordX, kordY);
					if(uhvacenaFigura!=0 && uhvacenaFigura*potez>0){
						Game.dodela(0, kordX, kordY);
						pocX=kordX;
						pocY=kordY;
						dragging = true;
					}else uhvacenaFigura=0;
				}
			}

			if(event.type == sf::Event::MouseButtonReleased){
				if(event.mouseButton.button == sf::Mouse::Left){
					dragging = false;
					int kordX=mouseX/width;
					int kordY=mouseY/width;
					if(uhvacenaFigura!=0){
						if(uhvacenaFigura==-1){
							if(Game.whitePawnMove(pocY, pocX, kordY, kordX)==true){
								Game.dodela(uhvacenaFigura, kordX, kordY);
								//potez*=-1
								Game.kingSafe(uhvacenaFigura, pocX, pocY, kordX, kordY);
								if(!Game.whiteKingCheck()){
									Game.promotion(-1, kordX, kordY);
								}
							}else Game.dodela(uhvacenaFigura, pocX, pocY);
						}else if(uhvacenaFigura==1){
							if(Game.blackPawnMove(pocY, pocX, kordY, kordX)==true){
								Game.dodela(uhvacenaFigura, kordX, kordY);
								//potez*=-1;
								Game.kingSafe(uhvacenaFigura, pocX, pocY, kordX, kordY);
								if(!Game.blackKingCheck()){
									Game.promotion(1, kordX, kordY);
								}
							}else Game.dodela(uhvacenaFigura, pocX, pocY);
						}else if(abs(uhvacenaFigura)==2){
							if(Game.rookMove(pocY, pocX, kordY, kordX)==true && Game.figura(kordX, kordY)*uhvacenaFigura<=0 && abs(Game.figura(kordX, kordY))!=6){
								if(pocY==n-1 && pocX==0)rookA1=true;
								else if(pocY==n-1 && pocX==n-1)rookH1=true;
								else if(pocY==0 && pocX==0)rookA8=true;
								else rookH8=true;
								Game.dodela(uhvacenaFigura, kordX, kordY);
								//potez*=-1;
								Game.kingSafe(uhvacenaFigura, pocX, pocY, kordX, kordY);
							}else Game.dodela(uhvacenaFigura, pocX, pocY);
						}else if(abs(uhvacenaFigura)==3){
							if(Game.knightMove(pocY, pocX, kordY, kordX)==true && Game.figura(kordX, kordY)*uhvacenaFigura<=0 && abs(Game.figura(kordX, kordY))!=6){
								Game.dodela(uhvacenaFigura, kordX, kordY);
								//potez*=-1;
								Game.kingSafe(uhvacenaFigura, pocX, pocY, kordX, kordY);
							}else Game.dodela(uhvacenaFigura, pocX, pocY);
						}else if(abs(uhvacenaFigura)==4){
							if(Game.bishopMove(pocY, pocX, kordY, kordX)==true && Game.figura(kordX, kordY)*uhvacenaFigura<=0 && abs(Game.figura(kordX, kordY))!=6){
								Game.dodela(uhvacenaFigura, kordX, kordY);
								//potez*=-1;
								Game.kingSafe(uhvacenaFigura, pocX, pocY, kordX, kordY);
							}else Game.dodela(uhvacenaFigura, pocX, pocY);
						}else if(abs(uhvacenaFigura)==5){
							if(Game.queenMove(pocY, pocX, kordY, kordX)==true && Game.figura(kordX, kordY)*uhvacenaFigura<=0 && abs(Game.figura(kordX, kordY))!=6){
								Game.dodela(uhvacenaFigura, kordX, kordY);
								//potez*=-1;
								Game.kingSafe(uhvacenaFigura, pocX, pocY, kordX, kordY);
							}else Game.dodela(uhvacenaFigura, pocX, pocY);
						}else if(uhvacenaFigura==-6){
							if(Game.whiteKingMove(pocY, pocX, kordY, kordX)==true){
								whiteKingHasMoved=true;
								Game.dodela(uhvacenaFigura, kordX, kordY);
								//potez*=-1;
								Game.kingSafe(uhvacenaFigura, pocX, pocY, kordX, kordY);
							}else if(Game.whiteKingShortCastle(pocY, pocX, kordY, kordX)==true){
								whiteKingHasMoved=true;
								Game.dodela(uhvacenaFigura, kordX, kordY);
								Game.dodela(-2, n-3, n-1);
								Game.dodela(0, n-1, n-1);
								potez*=-1;
							}else if(Game.whiteKingQueenCastle(pocY, pocX, kordY, kordX)==true){
								whiteKingHasMoved=true;
								Game.dodela(uhvacenaFigura, kordX, kordY);
								Game.dodela(-2, 3, n-1);
								Game.dodela(0, 0, n-1);
								potez*=-1;
							}else Game.dodela(uhvacenaFigura, pocX, pocY);
						}else if(uhvacenaFigura==6){
							if(Game.blackKingMove(pocY, pocX, kordY, kordX)==true){
								blackKingHasMoved=true;
								Game.dodela(uhvacenaFigura, kordX, kordY);
								//potez*=-1;
								Game.kingSafe(uhvacenaFigura, pocX, pocY, kordX, kordY);
							}else if(Game.blackKingShortCastle(pocY, pocX, kordY, kordX)==true){
								blackKingHasMoved=true;
								Game.dodela(uhvacenaFigura, kordX, kordY);
								Game.dodela(2, n-3, 0);
								Game.dodela(0, n-1, 0);
								potez*=-1;
							}else if(Game.blackKingQueenCastle(pocY, pocX, kordY, kordX)==true){
								blackKingHasMoved=true;
								Game.dodela(uhvacenaFigura, kordX, kordY);
								Game.dodela(2, 3, 0);
								Game.dodela(0, 0, 0);
								potez*=-1;
							}else Game.dodela(uhvacenaFigura, pocX, pocY);
						}
					}
				}
			}

			if(event.type == sf::Event::MouseMoved){
				mouseX = event.mouseMove.x;
				mouseY = event.mouseMove.y;
				//cout<<mouseX<<" "<<mouseY<<endl;
			}
		}

		window.clear();
		Game.print();
		//window.draw(sprite);
		
		if(dragging==true){
			//sprite.setPosition((mouseX/width)*width + offsetx, (mouseY/width)*width + offsety);
			drawFigure(uhvacenaFigura, mouseX-width/2, mouseY-width/2);
		}

		window.display();
    }

    return 0;
}
