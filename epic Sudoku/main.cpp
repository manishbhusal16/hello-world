
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <string>
#include <time.h>

#define MODOS 1         //Modes
#define DIFICULDADE 2   //difficulty
#define MENU 3

using namespace std;
using namespace sf;

struct Entry{ //Entry
	int val;
	string valuee;   //value
	Text tile;
};

struct tiptype{
	int tip;   //tip
	bool exchange;
};
struct namee{
	string name; //name
	int time;
};

struct UserData{
	string name;
};

struct SoundData{
	Music bach;
	Music strt;
	Music button;
	Music enter;
	Music errorr;
};

struct TextData{
	string modos[5];       //difficulty.....
	string tips[10];
};

struct Data{
		SoundData music;
	TextData textts;
	UserData user;
	TextData modos[5];
	//namedata rank;
};
void loadsound(SoundData &music)
{
	//music.bach.openFromFile("Patakas World.wav");
	music.strt.openFromFile("mp/Patakas World.wav");
	music.button.openFromFile("mp/click.wav");
	music.enter.openFromFile("mp/click.wav");
	music.errorr.openFromFile("mp/wrng.wav");
}


//yesle Menu ma hunda mode ma hunda difficulty level ma hunda
//font dekhaune kaam garcha
void loadText(TextData &textt, int kind)
{
	if(kind == MENU)
	{
		textt.modos[0]="New Game";
		textt.modos[1]="About";
		textt.modos[2]="Quit";



		textt.tips[0]="Its a country in Asia";
		textt.tips[1]="The level of difficulty can give you time bonus";
		textt.tips[2]="The difficulty level can improve your placement";
		textt.tips[3]="ESC to return to the menu";
		textt.tips[4]="Your language is English";
		textt.tips[5]="ESC to return to the menu";
		textt.tips[6]="Fewer errorrrs = Less time";
		textt.tips[7]="The difficulty level can improve your placement";
		textt.tips[8]="Land of Flowers and Windmills";
		textt.tips[9]="Best of Luck";
	}
	if(kind == MODOS)
	{
		textt.modos[0]=" 6x6 ";
		textt.modos[1]=" 9x9 ";

	}
	if(kind == DIFICULDADE)
	{
		textt.modos[0]=" Easy ";
		textt.modos[1]="Medium ";
		textt.modos[2]="Hard";
	}


}
void screenMenu(RenderWindow &window, Data &media);
void screenmod(RenderWindow &window, Data &media);
int TreaTment(RenderWindow &window, int &j, Data &media, int kind);
void drawButtons(RenderWindow &window, Data &media, int j, int index);


bool validRoww(int **m, int size, int row, int valuee)    //check repetation for row
{
	for(int i=0; i<size; i++)
	{
		if(m[row][i]==valuee)
		{
			return false;
		}
	}
	return true;
}

bool validcolunm(int **m, int size, int column, int valuee)    //check repetation for column
{
	for(int i=0; i<size; i++)
	{
		if(m[i][column]==valuee)
		{
			return false;
		}
	}
	return true;
}
bool validDiagonal(int **m, int size,int row, int column, int valuee)     //check for repetation in diagnol
{
	if(row==column || row+column==size-1)
	{
		for(int i=0; i<size;i++)
		{
			for(int j=0; j<size;j++)
			{
				if((i==j && row==column) && (i!=row && j!=column))
				{
					if(m[i][j] == valuee)
					{
						return false;
					}
				}
				if((i+j==size-1 && row+column==size-1) && (i!=row && j!=column))
				{
					if(m[i][j] == valuee)
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	return true;
}

bool validGrid(int **m, int size,int row, int column, int valuee)         //checking for small grid
{
	int quadx, quady;

	switch(size)
	{
	case 6:
		quadx=3;
		quady=2;
		break;
	case 9:
		quadx=3;
		quady=3;
		break;

	}

	int yini=row/quady;         //intilizing x and y....
	int xini=column/quadx;


	yini=(yini*quady);
	xini=(xini*quadx);

	for(int i=yini; i<yini+quady; i++)
	{
		for(int j=xini; j<xini+quadx;j++)
		{
			if(m[i][j]==valuee)
			{
				return false;
			}
		}
	}
	return true;

}

bool reset(bool bol[16], int t)
{
	for(int i=0; i<t; i++)
	{
		if(bol[i])
		{
			return false;
		}
	}
	return true;
}

int chooseDifficulty(RenderWindow &window, Data &media)
{
    Texture picmenu;
    picmenu.loadFromFile("back/pic2.png");
    Sprite picmenu1(picmenu);
    picmenu1.setScale(2.0f,2.0f);

	int j=0;

	loadText(media.textts,DIFICULDADE);

	window.clear();



	window.display();

	while(window.isOpen())
	{

		Event event;
                                            // while there are pending events...

		while (window.pollEvent(event))
		{
                                            // check the type of the event...
			switch (event.type)
			{
                                            // window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					TreaTment(window, j, media, DIFICULDADE);
					if(Keyboard::isKeyPressed(Keyboard::Return))
					{
						//media.music.enter.play();                           //music after pressing enter.....
						return j;
					}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}

		window.clear();

		window.draw(picmenu1);



		drawButtons(window, media , j, 3);

		window.display();

	}
}

void selectTile(RenderWindow &window, int size,int &index, int **map, bool **bloc,Data &media, int &errors, bool diag, Entry &entry) //selecting block for entry..
{
	if(Keyboard::isKeyPressed(Keyboard::Left))
	{
	    media.music.button.play();
		index--;
		if(index<0)
		{
			index=size*size-1;
		}
		entry.valuee="";
	}

	if(Keyboard::isKeyPressed(Keyboard::Right))
	{
	    media.music.button.play();
		index++;
		if(index>size*size-1)
		{
			index=0;
		}
		entry.valuee="";
	}
	if(Keyboard::isKeyPressed(Keyboard::Up))
	{
	    media.music.button.play();
		index-=size;
		if(index<0)
		{
            index=((size*size))-size+((index+size)%size);
		}
		entry.valuee="";
	}

	if(Keyboard::isKeyPressed(Keyboard::Down))
	{
	    media.music.button.play();
		index+=size;
		if(index>size*size-1)
		{
			index=index%size;
		}
		entry.valuee="";
	}
	if(!bloc[index/size][index%size] && entry.valuee.size()<=1)
	{
		if(Keyboard::isKeyPressed(Keyboard::Num0) || Keyboard::isKeyPressed(Keyboard::Numpad0))
		{
			entry.valuee+='0';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num1) || Keyboard::isKeyPressed(Keyboard::Numpad1))
		{
			entry.valuee+='1';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num2) || Keyboard::isKeyPressed(Keyboard::Numpad2))
		{
			entry.valuee+='2';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num3) || Keyboard::isKeyPressed(Keyboard::Numpad3))
		{
			entry.valuee+='3';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num4) || Keyboard::isKeyPressed(Keyboard::Numpad4))
		{
			entry.valuee+='4';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num5) || Keyboard::isKeyPressed(Keyboard::Numpad5))
		{
			entry.valuee+='5';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num6) || Keyboard::isKeyPressed(Keyboard::Numpad6))
		{
			entry.valuee+='6';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num7) || Keyboard::isKeyPressed(Keyboard::Numpad7))
		{
			entry.valuee+='7';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num8) || Keyboard::isKeyPressed(Keyboard::Numpad8))
		{
			entry.valuee+='8';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num9) || Keyboard::isKeyPressed(Keyboard::Numpad9))
		{
			entry.valuee+='9';
		}
	}
	if(!bloc[index/size][index%size])
	{
		if(Keyboard::isKeyPressed(Keyboard::Return))
		{
			entry.val = atoi(entry.valuee.c_str());

			bool valid;
			if(diag)
			{
				valid=validcolunm(map,size,index%size,entry.val) && validRoww(map,size,index/size,entry.val) && validGrid(map,size,index/size,index%size, entry.val) && validDiagonal(map,size,index/size,index%size, entry.val);
			}
			else
			{
				valid=validcolunm(map,size,index%size,entry.val) && validRoww(map,size,index/size,entry.val) && validGrid(map,size,index/size,index%size, entry.val);
			}

			if(entry.val>0 && entry.val<=size && valid)
			{
				map[index/size][index%size]=entry.val;
				entry.valuee="";
			}
			else
			{
				entry.valuee="";
				errors++;
				media.music.errorr.play();
				media.music.errorr.setVolume(20);			}
		}
	}
}

string intTOstring(int number)
{
	if (number == 0)
	        return "0";
	    string temp="";
	    string returnvalue="";
	    while (number>0)
	    {
	        temp+=number%10+48;
	        number/=10;
	    }
	    for (int i=0;i<temp.length();i++)
	    {
	    	 returnvalue+=temp[temp.length()-i-1];
	    }

	    return returnvalue;
}

void createblock(RenderWindow &window, TextData textt, int **map,bool **bloc, int size, int index, int time, int wrong, int coeficientDificulty, tiptype &tip)
{                                                                               //creating blocks

    Texture backg;
    backg.loadFromFile("back/pic5.png");
    Sprite spritback(backg);
    spritback.setScale(2.f,2.f);
    window.draw(spritback);

	int quadx, quady;                         //FOR MAKING GRID 3X3 AND OTHER.....

		switch(size)
		{
		case 6:
			quadx=3;
			quady=2;
			//window.draw(sprittil6);
			break;
		case 9:
			quadx=3;
			quady=3;
			//window.draw(sprittil9);
			break;

		}
	int sizeTILE, sizeFONTE;

	if(size==6)
	{
		sizeTILE=40;
		sizeFONTE=20;
	}
	else
	{
		sizeTILE=40;
		sizeFONTE=20;
	}

	Font font;
	font.loadFromFile("font/JOKERMAN.TTF");

	string timepass= "Time: "+ intTOstring(time);    //position of time...
	Text clock(timepass, font, 20);
	clock.setPosition(650, 40);
	clock.setColor(Color(80, 80, 80));
	window.draw(clock);

	string errorstext ="Mistakes: " + intTOstring(wrong);   //position of mistake
	Text errors(errorstext, font, 20);
	errors.setPosition(650, 80);
	errors.setColor(Color::Red);

	string add= "- ";
	add += intTOstring(wrong*coeficientDificulty);         //position of negative points
	Text added(add, font, 20);
	added.setPosition(650, 120);
	added.setColor(Color::Red);


	window.draw(errors);
	window.draw(added);

	RectangleShape middlethick;                                      //Middle line for seperating grid
	middlethick.setOutlineThickness(3);
	middlethick.setSize(Vector2f(sizeTILE*quadx, sizeTILE*quady));
	middlethick.setOutlineColor(sf::Color(200,200,0));
	middlethick.setFillColor(sf::Color::Transparent);

	RectangleShape tileouter;                                       //outer line
	tileouter.setSize(Vector2f(sizeTILE*size, sizeTILE*size));
	tileouter.setOutlineThickness(10);
	//tileouter.setOutlineColor(sf::Color::Black);
	tileouter.setOutlineColor(sf::Color(200,200,0));
	tileouter.setFillColor(sf::Color::Transparent);
	tileouter.setPosition((400-size/2*sizeTILE), (300-size/2*sizeTILE));

	RectangleShape nametitle;                                               //thin lines (many)
	nametitle.setSize(Vector2f(sizeTILE, sizeTILE));
	nametitle.setOutlineThickness(1);
	//nametitle.setOutlineColor(Color::Red);
	nametitle.setOutlineColor(Color(200,200,0));
	nametitle.setFillColor(sf::Color::Transparent);

	for(int i=0; i<size; i++)                                           //setting blocks acc to grids.....
	{
		for(int j=0; j<size; j++)
		{
			nametitle.setPosition((400-size/2*sizeTILE)+j*sizeTILE,(300-size/2*sizeTILE)+i*sizeTILE);
			window.draw(nametitle);
			string textt;
			if(map[i][j]>0)
			{
				textt = intTOstring(map[i][j]);
			}
			else
			{
				textt="";
			}
			if(index%size==j && index/size==i)
			{
				nametitle.setFillColor(Color::Red);                     //transparent box........
				nametitle.setPosition((400-size/2*sizeTILE)+j*sizeTILE,(300-size/2*sizeTILE)+i*sizeTILE);
				window.draw(nametitle);
				nametitle.setFillColor(sf::Color::Transparent);
			}

			Text tile(textt, font, sizeFONTE);
			tile.setPosition((400-size/2*sizeTILE)+j*sizeTILE+sizeFONTE/2, (300-size/2*sizeTILE)+i*sizeTILE+sizeFONTE/2);
			if(bloc[i][j])
			{
				tile.setColor(Color(80, 80, 80));
			}
			else
			{
				tile.setColor(Color::Blue);
			}


			window.draw(tile);
		}
	}

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			if(i%quady==0 && j%quadx==0)
			{
				middlethick.setPosition((400-size/2*sizeTILE)+j*sizeTILE,(300-size/2*sizeTILE)+i*sizeTILE);
				window.draw(middlethick);
			}
		}
	}
	window.draw(tileouter);
}

void fillinDiagonal(int **m, int size)                              //generator1.....
{
	int val[16];
	bool valbol[16];

	for(int i=0; i<size; i++)
	{
		val[i]=i+1;
		valbol[i]=true;
	}

	int aux=(rand()%size);                                          //number generate garxa from 0 to size.......

	int resets=0;

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{

			if(i==j || i+j==size-1)
			{
				if(valbol[aux])
				{
					bool valid;

					valid = validRoww(m,size,i,val[aux])&& validcolunm(m,size,j,val[aux]) && validGrid(m,size,i,j,val[aux]) && validDiagonal(m,size,i,j,val[aux]);
					if(valid)
					{
						m[i][j]=val[aux];
						for(int i=0; i<size; i++)
						{
							valbol[i]=true;
						}
						aux=(rand()%size);
					}
					else
					{
						valbol[aux]=false;
						j--;
						aux=(aux+1)%size;
					}
				}
				else
				{
					aux=(aux+1)%size;
					j--;
					if(reset(valbol,size))
					{
						for(int x=0; x<size; x++)
						{
							valbol[x]=true;
							m[i][x]=0;
						}
						resets++;
						if(resets>1000)
						{
							i=-1;
							j=-1;
							resets=0;
							for(int i2 = 0; i2 < size; i2++)
							{
								for(int j2 = 0; j2 < size; j2++)
								{
									m[i2][j2]=0;
								}
							}
							break;
						}
					}
				}
			}
		}
	}
}

void fillInmap(int **m, int &size, RenderWindow &window, Data &media, bool &diag)                 //number generator....
{
	time_t time_initial, time_atual, time_drive;

	time(&time_initial);

	int val[size];
	bool valbol[size];
	if(size==10)
	{
		size=9;
		diag=true;
	}
	for(int i=0; i<size; i++)
	{
		val[i]=i+1;
		valbol[i]=true;
	}

	int aux=(rand()%size);
	int resets=0;
	if(diag)
	{
		fillinDiagonal(m, size);
	}
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			time(&time_atual);
			time_drive=(time_atual-time_initial);

			if(valbol[aux])
			{
				bool valid;
				if(diag)
				{
					valid=validRoww(m,size,i,val[aux])&& validcolunm(m,size,j,val[aux]) && validGrid(m,size,i,j,val[aux]) && validDiagonal(m,size,i,j,val[aux]);
				}
				else
				{
					valid=validRoww(m,size,i,val[aux])&& validcolunm(m,size,j,val[aux]) && validGrid(m,size,i,j,val[aux]);
				}

				if(valid)
				{
					m[i][j]=val[aux];
					for(int i=0; i<16; i++)
					{
						valbol[i]=true;
					}
					aux=(rand()%size);
				}
				else
				{
					valbol[aux]=false;
					j--;
					aux=(aux+1)%size;
				}
			}
			else
			{
				aux=(aux+1)%size;
				j--;
				if(reset(valbol,size))
				{
					for(int x=0; x<16; x++)
					{
						valbol[x]=true;
						m[i][x]=0;
					}
					resets++;
					j=-1;
					if(resets>1000)
					{
						i=-1;
						j=-1;
						resets=0;
						for(int i2 = 0; i2 < size; i2++)
						{
							for(int j2 = 0; j2 < size; j2++)
							{
								m[i2][j2]=0;
							}
						}
						if(diag)
						{
							fillinDiagonal(m, size);
						}
						break;
					}
				}
			}
		}
	}

}

void addDifficulty(int **map, bool **b, int size, int dificuldade)       //giving number according to difficulty
{
	int percentage;
	switch (dificuldade)
	{
	case 0:
		percentage=2;
		break;
	case 1:
		percentage=3;
		break;
    case 2:
		percentage=4;
		break;

    	}

	int items =(size*size);
	int stay=items/percentage;
	int deletee = items-stay;

	int row=rand()%size;
	int column=rand()%size;
	for(int i=0; i<deletee; i++)
	{
		if(map[row][column] > 0)
		{
			map[row][column]=0;
			b[row][column]=false;
		}
		else
		{
			row=rand()%size;
			column=rand()%size;
			i--;
		}
	}
}

bool testarwon(int **m, int size)     //final check garne.....won or lost...
{
	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			if(m[i][j]==0)
			{
				return false;
			}
		}
	}
	return true;
}



void ending(RenderWindow &window, Data &media, bool won)   //last window kholne..
{
	Font font;
	font.loadFromFile("font/impact.ttf");

	string textt;
	if(won)
	{
		textt="YOU WON!!";
	}
	else
	{
		textt="YOU LOST!!";
	}

	Text content(textt, font, 60);
	content.setPosition(225, 250);
	content.setColor(Color(200, 80, 80,255));
	while(window.isOpen())
	{
		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:

					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
					{
						media.music.strt.stop();
						screenMenu(window,media);
					}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}


		//window.draw(media.images.game_ground);
		window.draw(content);

		window.display();

	}
}

void screensize(RenderWindow &window, Data &media, int dificuldade, int tam)   //tam=size of grid 6 or 9...
{
	int coeficientDificulty=(3-dificuldade)*10;
	int size=tam;
	int **m = new int*[20];
	m[0] = new int[20*20];

	for (int i=1; i<20; i++)
	{
		m[i] = m[i-1]+20;
	}

	bool **b = new bool*[20];
	b[0] = new bool[20*20];

	for (int i=1; i<20; i++)
	{
		b[i] = b[i-1]+20;
	}

	for(int i=0;i<20;i++)
	{
		for(int j=0;j<20;j++)
		{
			b[i][j]=true;
		}

	}

	bool won=false;
	bool diag=false;
	fillInmap(m,size, window, media, diag);
	addDifficulty(m,b,size,dificuldade);

	int errors=0;

	time_t time_initial, time_atual, time_drive;

	time(&time_initial);

	int index = 0;
	tiptype tip;
	tip.tip=rand()%10;
	tip.exchange=false;

	Entry entry;
	entry.val=0;
	entry.valuee="";

	//Font font;
	Font font;
	font.loadFromFile("font/arial.ttf");

	int sizeTILE, sizeFONTE;

	if(size==16)
	{
		sizeTILE=30;
		sizeFONTE=15;
	}
	else
	{
		sizeTILE=40;
		sizeFONTE=20;
	}

	while(window.isOpen())
	{
		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					delete []m[0];
					delete []b[0];
					window.close();
					break;

				case Event::KeyPressed:

					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
					{
						media.music.strt.stop();
						delete []m[0];
						delete []b[0];
						screenMenu(window,media);
					}
					selectTile(window, size,index, m,b, media, errors, diag, entry);
				break;

				// we don't process other types of events
				default:
					break;
			}

		}
		time(&time_atual);

		time_drive=(time_atual-time_initial);
		window.clear();


		createblock(window,media.textts, m , b, size, index, time_drive, errors, coeficientDificulty, tip);

		entry.tile.setCharacterSize(20);
		entry.tile.setColor(Color(200, 80, 80,255));
		entry.tile.setFont(font);
		entry.tile.setString(entry.valuee);
		entry.tile.setPosition(10,10);
		entry.tile.setPosition((400-size/2*sizeTILE)+index%size*sizeTILE+sizeFONTE/2, (300-size/2*sizeTILE)+index/size*sizeTILE+sizeFONTE/2);

		window.draw(entry.tile);
		window.display();

		if(errors>20)      //how much errorrr to commite lose.....
		{
			ending(window, media,won);
		}
		if(testarwon(m, size))
		{
			delete []m[0];
			delete []b[0];
			won=true;
			int timeadded = time_drive+(coeficientDificulty*errors)-((10-errors)*dificuldade);
            ending(window,media,won);
		}
	}
}
void Aboutmod(RenderWindow &window,Data &media)
{
	Font font4;
	font4.loadFromFile("font/Hantu_Kom_Kom.ttf");
	Text titlee2("CREATED BY \n\nManish  Bhusal \nMilan   Thapa \nKaushal Dhakal \nPranish Shrestha",font4,20);
	titlee2.setPosition(200,100);
	titlee2.setColor(Color::Cyan);

	Text titlee3("SUPERVISOR: \n\nMr.Manoj Shakya",font4,20);
	titlee3.setPosition(200,300);
	titlee3.setColor(Color::Cyan);

    Texture texture;
    texture.loadFromFile("back/about.jpeg");
    texture.loadFromFile("back/about.jpeg");
    Sprite sprite(texture);
    sprite.setScale(.4f,.45f);
    //sprite.setPosition(300.f,300.f);

	while(window.isOpen())
	{
		Event event;
		// while there are pending events...



		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

                case Event::KeyPressed:
					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
					{
						screenMenu(window,media);
					}

				break;
			}
		}
		window.draw(sprite);
		window.draw(titlee2);
		window.draw(titlee3);
		window.display();
    }
}

int TreaTment(RenderWindow &window, int &j, Data &media, int kind)
{
	int index=0;

	if(kind==MENU)
	{
		index=2;
	}

	if(kind==MODOS)
	{
		index=4;
	}

	if(kind==DIFICULDADE)
	{
		index=2;
	}


	if( (Keyboard::isKeyPressed(Keyboard::RAlt))&& (Keyboard::isKeyPressed(Keyboard::F4)))
	{
		window.close();
	}

	if(Keyboard::isKeyPressed(Keyboard::Up))
	{
		media.music.button.play();
		j--;
		if(j<0)
		{
			j=index;
		}
	}

	if(Keyboard::isKeyPressed(Keyboard::Down))
	{
		media.music.button.play();
		j++;
		if(j>index)
		{
			j=0;
		}
	}

	if(Keyboard::isKeyPressed(Keyboard::Escape))
	{
		media.music.strt.stop();
		screenMenu(window, media);
	}

	if(Keyboard::isKeyPressed(Keyboard::Return))
	{
		media.music.enter.play();
		if(kind==MENU)
		{
			switch(j)                                       //function of newgame and quit.....
			{
				case 0:
					screenmod(window,media);
				break;
				case 1:
                    Aboutmod(window,media);
                break;
				case 2:
					 window.close();
				break;

			}
		}

		if(kind==MODOS)                     //options for 6x6,9x9 or else.....
		{
			switch(j)
			{
				case 0:
					screensize(window, media, chooseDifficulty(window, media),6);
				break;
				case 1:
					screensize(window, media, chooseDifficulty(window, media),9);
				break;
			}
		}

		if(kind==DIFICULDADE)
		{
			return j;
		}
	}
}


void drawyou(RenderWindow &window, TextData textt, int index)
{
	Font font;
	font.loadFromFile("font/HARLOWSI.TTF");

		int x=375, y=185;  //to set position of new games and else...

		for(int i=0; i<=index; i++)
		{
			Text temp(textt.modos[i], font, 30);
			temp.setPosition(x, y);
			temp.setColor(Color::Black);

			window.draw(temp);

			y+=80;
		}

}

void drawButtons(RenderWindow &window, Data &media, int j, int index)
{

	int x=350, y=175;       //position of text 6x6 and 9x9..

	for(int i=0; i<index; i++)
	{
		if(i!=j)
		{
    RectangleShape nametitlel;
	//nametitlel.setSize(Vector2f(600,50));
	nametitlel.setOutlineThickness(10);
	nametitlel.setOutlineColor(Color::Red);
	nametitlel.setFillColor(Color(255,0,0,100));
	nametitlel.setPosition(x,y+10);
	window.draw(nametitlel);

		}
		else
		{
			//media.images.buttonh.setPosition(x,y);
			//window.draw(media.images.buttonh);

		}

		y+=80;
	}

	drawyou(window, media.textts, index);

}

void screenmod(RenderWindow &window, Data &media)
{
      Texture picmenu;
    picmenu.loadFromFile("back/pic2.png");
    Sprite picmenu1(picmenu);
    picmenu1.setScale(0.9f,0.9f);


	loadText(media.textts,MODOS);
	int j=0;

	window.clear();



	window.display();

	//media.music.strt.play();

	while(window.isOpen())
	{

		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					TreaTment(window, j, media, MODOS);
				break;

				// we don't process other types of events
				default:
					break;
			}

		}

		window.clear();
		window.draw(picmenu1);



		drawButtons(window, media , j,5);

		window.display();

	}
}

void screenMenu(RenderWindow &window, Data &media)
{
     Texture picmenu;
    picmenu.loadFromFile("back/pic2.png");
    Sprite picmenu1(picmenu);
    picmenu1.setScale(1.0f,1.f);
	loadText(media.textts,MENU);
	int j=0;

	window.clear();

	window.display();

	media.music.strt.play();
	media.music.strt.setVolume(20);

	while(window.isOpen())
	{

		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					TreaTment(window, j, media, MENU);
				break;

				// we don't process other types of events
				default:
					break;
			}

		}

		window.clear();
		window.draw(picmenu1);

		drawButtons(window, media , j,2);

		window.display();

	}
}

void wear(RenderWindow &window, Data &media)                    //entering name block.....
{
	RectangleShape nametitle;
	nametitle.setSize(Vector2f(600,50));
	nametitle.setOutlineThickness(0);
	nametitle.setOutlineColor(Color::Red);
	nametitle.setFillColor(Color(255,0,0,100));
	nametitle.setPosition(100,290);

	Font font;
	font.loadFromFile("font/COOPBL.TTF");

    media.user.name="";

	std::string str;

	Text desc("ENTER YOUR NAME", font, 30);
	desc.setPosition(300, 230);
	desc.setColor(Color(80, 80, 80));

	while(window.isOpen())
	{
		Event event;
		// while there are pending events...



		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					if( (Keyboard::isKeyPressed(Keyboard::Return)) )
					{
						screenMenu(window,media);
					}

				break;

				// In event loop...
				case Event::TextEntered:
				    // Handle ASCII characters only
					if(event.text.unicode=='\b' && media.user.name.size()>0)
					{
						media.user.name.erase(media.user.name.size()-1,1);
					}
					else if(event.text.unicode < 128 && media.user.name.size()<35)
				    {
				    		media.user.name+= static_cast<char>(event.text.unicode);
				    }
				break;
				// we don't process other types of events
				default:
					break;
			}
		}


		Text name(media.user.name, font, 30);
		name.setPosition(400 - name.getLocalBounds().width/2, 300);
		name.setColor(Color(255,255,255));

		window.draw(desc);
		window.draw(nametitle);
		window.draw(name);
		window.display();


	}

}
void title(RenderWindow &window,Data &media)                                  //sudoko:the game/............
{
	Font font0;
	font0.loadFromFile("font/Blankblizzkriegi.ttf");
	Text titlee("EPIC   \n ",font0,80);
	titlee.setPosition(80,50);
	titlee.setColor(Color::Red);

	Text titlee2("EPIC SUDOKU  \n ",font0,40);
	titlee2.setPosition(50,400);
	titlee2.rotate(50);
	titlee2.setColor(Color::Black);

	Text titlee3("EPIC SUDOKU \n ",font0,30);
	titlee3.setPosition(500,400);
	titlee3.rotate(40);
	titlee3.setColor(Color::Blue);

    Texture texture;
    texture.loadFromFile("back/mainn.png");
    Sprite sprite(texture);
    sprite.setScale(1.3f,1.5f);
    //sprite.setPosition(300.f,300.f);

	while(window.isOpen())
	{
		Event event;
		// while there are pending events...



		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

                case Event::KeyPressed:
					if( (Keyboard::isKeyPressed(Keyboard::Return)) )
					{
						wear(window,media);
					}

				break;
			}
		}
		window.draw(sprite);
		window.draw(titlee);
		window.draw(titlee2);
		window.draw(titlee3);
		window.display();
    }
}


int main()
{
	srand(time(NULL));

	// Create the main window
	RenderWindow window(VideoMode(800, 600), "EPIC SUDOKU", Style::Default);
	window.setVerticalSyncEnabled(true);

	Data media;

//	loadimages(media.images);
	loadsound(media.music);
	loadText(media.textts,MODOS);
    title(window, media);
	wear(window, media);
	screenMenu(window, media);

	return 0;
}
