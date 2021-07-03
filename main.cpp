//nogo.cpp

#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;

#define WT 0
#define BK 1
#define NWT1 2		//白子下必死，黑子的眼, [0][9]
#define NBK1 3		//黑子下必死，白子的眼, [1][9]
#define NWT2 9		//Type 2			[2][9]
#define NBK2 10 	//Type 2 			[3][9]
#define DIE 4		//两者都不可下
#define FREE 5		//					[4][9]
#define FR2 6		//4 corners 		[5][9]
#define FR3 7		//4 sides 			[6][9]
#define FR4 8		//四大皆空 			[7][9]

int board[9][10];
int rnd=1;
bool col=0; //人的 colour
const int dx[]={0, 1, 0, -1}, dy[]={1, 0, -1, 0}; //东南西北
const int qx[]={1, -1, 1, -1}, qy[]={1, 1, -1, -1}; //45度东南西北
bool dfs_air_visit[9][9];

void initialise ()
{
	board[0][0] = FR2;
	board[0][8] = FR2;
	board[8][0] = FR2;
	board[8][8] = FR2;

	for (int i = 1; i <= 7; i++)
	{
		board[0][i] = FR3;
		board[i][0] = FR3;
		board[8][i] = FR3;
		board[i][8] = FR3;
	}

	for (int i = 1; i <= 7; i++)
		for (int j = 1; j <= 7; j++)
			board[i][j] = FR4;

	for (int k=0; k<8; k++)
		board[k][9]=0;
}

void test_display ()
{
	/*cout << "  0 1 2 3 4 5 6 7 8 " << endl;
		for (int i = 0; i <= 8; i++) {
			cout << i<<" ";
			for (int j = 0; j <= 9; j++)
				cout<<board[i][j]<<" ";
			cout << endl;
		}

	cout<<endl<< "Round Number: "<< rnd <<endl;*/

	cout << "  0 1 2 3 4 5 6 7 8 " << endl <<"0 ";
		switch (board[0][0])
		{
			case 0:
				cout<<"○─";
				break;
			case 1:
				cout<<"●─";
				break;
			default:
				cout<<"┌─";
				break;
		}

		for (int k=1; k<8; k++){
			switch (board[0][k])
			{
				case 0:
					cout<<"○─";
					break;
				case 1:
					cout<<"●─";
					break;
				default:
					cout<<"┬─";
					break;
			}
		}

		switch (board[0][8])
		{
				case 0:
					cout<<"○"<<endl;;
					break;
				case 1:
					cout<<"●"<<endl;
					break;
				default:
					cout<<"┐"<<endl;
					break;
		}

		for (int i = 1; i <= 7; i++) {
			cout << i<<" ";
			for (int j = 0; j < 9; j++){
				switch (board[i][j])
				{
					case 0:
						if (j==8)
							cout<<"○";
						else
							cout<<"○─";
						break;
					case 1:
						if (j==8)
							cout<<"●";
						else
							cout<<"●─";
						break;
					default:
						if (j==0)
							cout<<"├─";
						else if (j==8)
							cout<<"┤";
						else
							cout<<"┼─";
						break;
				}
			}
			cout << endl;
		}
		cout<<"8 ";
		switch (board[8][0])
		{
				case 0:
					cout<<"○─";
					break;
				case 1:
					cout<<"●─";
					break;
				default:
					cout<<"└─";
					break;
		}

			for (int k=1; k<8; k++){
				switch (board[8][k])
				{
					case 0:
						cout<<"○─";
						break;
					case 1:
						cout<<"●─";
						break;
					default:
						cout<<"┴─";
						break;
				}
			}

			switch (board[8][8])
			{
					case 0:
						cout<<"○"<<endl;
						break;
					case 1:
						cout<<"●"<<endl;
						break;
					default:
						cout<<"┘"<<endl;
						break;
			}

		cout<<endl<< "Round Number: "<< rnd <<endl;
}

bool inBorder(int x, int y) {return x>=0 && x<9 && y>=0 && y<9;}

bool dfs_air (int board[9][10], int cx, int cy)
{
	dfs_air_visit[cx][cy]=true;
	bool flag=false;
	for (int dir = 0; dir < 4; dir++)
	{
		int fx=cx+dx[dir], fy=cy+dy[dir];
		if (inBorder(fx, fy))
		{
			if (board[fx][fy] > 1)
				flag=true;
			if (board[cx][cy] == board[fx][fy] && !dfs_air_visit[fx][fy])
				if (dfs_air(board, fx, fy))
					flag=true;
		}
	}
	return flag;
}

bool checkAir (int board[9][10], int cx, int cy, bool col)
{
	memset(dfs_air_visit, false, sizeof(dfs_air_visit));
	int temp=board[cx][cy];
	board[cx][cy]=col;
	bool flag = dfs_air(board, cx, cy);
	board[cx][cy]=temp;
	return flag;
}

void set_noBW (int board[9][10])
{
	for (int i=0; i<9; i++)
	for (int j=0; j<9; j++)
	{
		//check NWT1,NBK1
		if (board[i][j]==FREE) {
			if (checkAir(board, i, j, WT)==false)
				board[i][j]=NWT1;	//放白，看白是不是被黑包，用DFS, set NWT

			if (checkAir(board, i, j, BK)==false)
				board[i][j]=NBK1;	//放黑，看黑是不是被白包，用DFS, set NBK
		}

		//check NWT2,NBK2
		//放一个白，看他周围有没有黑，有黑看黑有没有气，没有气＝NWT
		bool tempAir = true;
		if (board[i][j]==FREE) {
			board[i][j]=WT;
			for (int d=0; d<4; d++){
				int fx=i+dx[d], fy=j+dy[d];
				if (inBorder(fx, fy))
					if (board[fx][fy]==BK){
						tempAir=checkAir(board, fx, fy, BK);
						if (!tempAir)
							break;
				}
			}
			if (tempAir)
				board[i][j]=FREE;
			else
				board[i][j]=NWT2;
		}
		//same for black
		tempAir=true;
		if (board[i][j]==FREE) {
			board[i][j]=BK;
			for (int d=0; d<4; d++){
				int fx=i+dx[d], fy=j+dy[d];
				if (inBorder(fx, fy))
					if (board[fx][fy]==WT){
						tempAir=checkAir(board, fx, fy, WT);
						if (!tempAir)
							break;
				}
			}
			if (tempAir)
				board[i][j]=FREE;
			else
				board[i][j]=NBK2;
		}

		//check DIE
		if (board[i][j]==NWT1 || board[i][j]==NWT2)
		{
			int status=board[i][j];
			if (checkAir(board, i, j, BK)==false)
				board[i][j]=DIE;	//放黑，是不是被白包，if yes, set DIE
			else
			{
				tempAir=true;
				board[i][j]=BK;
				for (int d=0; d<4; d++){
					int fx=i+dx[d], fy=j+dy[d];
					if (inBorder(fx, fy))
						if (board[fx][fy]==WT){
							tempAir=checkAir(board, fx, fy, WT);
							if (!tempAir)
								break;
						}
				}
				if (tempAir)
					board[i][j]=status;
				else
					board[i][j]=DIE;
			}
		}

		if (board[i][j]==NBK1 || board[i][j]==NBK2)
		{
			if (checkAir(board, i, j, WT)==false)
				board[i][j]=DIE; //放白，是不是被黑包，if yes, set DIE
			else
			{
				int status=board[i][j];
				tempAir=true;
				board[i][j]=WT;
				for (int d=0; d<4; d++){
					int fx=i+dx[d], fy=j+dy[d];
					if (inBorder(fx, fy))
						if (board[fx][fy]==BK){
							tempAir=checkAir(board, fx, fy, BK);
							if (!tempAir)
								break;
						}
				}
				if (tempAir)
					board[i][j]=status;
				else
					board[i][j]=DIE;
			}
		}
	}

	//initialise count column
	for (int k=0; k<8; k++)
		board[k][9]=0;

	//calculate count column
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
		{
			switch (board[i][j]) {
			case NWT1:
				board[0][9]++;
				break;
			case NBK1:
				board[1][9]++;
				break;
			case NWT2:
				board[2][9]++;
				break;
			case NBK2:
				board[3][9]++;
				break;
			case FREE:
				board[4][9]++;
				break;
			case FR2:
				board[5][9]++;
				break;
			case FR3:
				board[6][9]++;
				break;
			case FR4:
				board[7][9]++;
				break;
			}
		}

}

void move (int board[9][10], int x, int y, bool col)
{
	board[x][y]=col;
	for (int d=0; d<4; d++)
		if (inBorder(x+dx[d], y+dy[d]))
			if (board[x+dx[d]][y+dy[d]]==FR4 || board[x+dx[d]][y+dy[d]]==FR3
					|| board[x+dx[d]][y+dy[d]]==FR2)
				board[x+dx[d]][y+dy[d]]=FREE;

	//change FREE to NBK or NWT
	set_noBW(board);
}

void moveDisplay(int board[9][10], int x, int y, bool col)
{
	move(board, x, y, col);
	rnd++;
	if (col==WT)
		printf("White has moved to (%d, %d) \n", x, y);
	else
		printf("Black has moved to (%d, %d) \n", x, y);
	test_display();

	return;
}

void deepCopy (int tempB[9][10])
{
	for (int i=0; i<9; i++)
		for (int j=0; j<10; j++)
			tempB[i][j]=board[i][j];
}

int best_ai (bool col_ai)
{
	int tempB[9][10];
	int store=0; //for type 1
	for (int i=0; i<9; i++)
	for (int j=0; j<9; j++)
	{
		deepCopy(tempB);
		if (tempB[i][j]>=FREE && tempB[i][j]<=FR4){
			move(tempB, i, j, col_ai);
			if (col_ai==WT) { //AI is white, 看有没有产生多一个 NBK2
				if (tempB[3][9]>board[3][9] )
					return i*9+j;
				else if (tempB[1][9]>board[1][9])
					//return i*9+j;
					store=i*9+j; //看有没有产生多一个 NBK1
			} else { //AI is black, 看有没有产生多一个 NWT2
				if (tempB[2][9]>board[2][9])
					return i*9+j;
				if (tempB[0][9]>board[0][9])
					//return i*9+j;
					store=i*9+j; //看有没有产生多一个 NWT1
			}
		}
	}
	if (store!=0)
		return store;

	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
		{
			deepCopy(tempB);
			if (tempB[i][j]>=FREE && tempB[i][j]<=FR4){
				move(tempB, i, j, col_ai);
				if (col_ai==WT) { //AI is white, 看有没有产生少一个NWT2
					if (tempB[0][9]<board[0][9] || tempB[2][9]<board[2][9])
						return i*9+j;
				} else { //AI is black, 看有没有产生少一个NBK2
					if (tempB[1][9]<board[1][9] || tempB[3][9]<board[3][9])
						return i*9+j;
				}
			}
		}

	//保护我方 type 2, spiral in
	bool turn[9][9]={{false}};
	for (int cur = 0, dir = 0, i = 0, j = 0; cur < 9 * 9; cur++)
	{
	    int _i = i + dx[dir], _j = j + dy[dir];
	    turn[i][j]=true;
		if (col_ai==WT){ //AI is white
			int cnt=0;
			if (board[i][j]==NBK2 || board[i][j]==NWT2){
				for (int d=0; d<4; d++)
				if (inBorder(i+dx[d],j+dy[d]))
					if (board[i+dx[d]][j+dy[d]]==WT)
						cnt++;
				if (cnt<=1) //如果只有一个自己人，保护
					for (int d=0; d<4; d++)
					if (inBorder(i+dx[d],j+dy[d]))
						if (board[i+dx[d]][j+dy[d]]>=5 && board[i+dx[d]][j+dy[d]]<=8)
							return (i+dx[d])*9+(j+dy[d]);
			}
		} else { //AI is black
			int cnt=0;
			if (board[i][j]==NWT2 || board[i][j]==NBK2){
				for (int d=0; d<4; d++)
				if (inBorder(i+dx[d],j+dy[d]))
					if (board[i+dx[d]][j+dy[d]]==BK)
						cnt++;
				if (cnt<=1) //如果只有一个自己人，保护
					for (int d=0; d<4; d++)
					if (inBorder(i+dx[d],j+dy[d]))
						if (board[i+dx[d]][j+dy[d]]>=5 && board[i+dx[d]][j+dy[d]]<=8)
							return (i+dx[d])*9+(j+dy[d]);
			}
		}

		if (_i < 0 || _i >= 9 || _j < 0 || _j >= 9 || turn[_i][_j])
		{
			dir = (dir + 5) % 4;
			_i = i + dx[dir];
			_j = j + dy[dir]; //cout << "SPIRAL TURN" << endl;
		}
		i = _i;
		j = _j;
	}//spiral for

	//find 自己旁边的432大皆空
	for (int i=0; i<9; i++)
	for (int j=0; j<9; j++)
	if (board[i][j]==col_ai){
		for (int d=0; d<4; d++)
		if (inBorder(i+qx[d],j+qy[d]))
		{
			if (board[i+qx[d]][j+qy[d]]==FR4)
				return (i+qx[d])*9+(j+qy[d]);
			if (board[i+qx[d]][j+qy[d]]==FR3)
				return (i+qx[d])*9+(j+qy[d]);
			if (board[i+qx[d]][j+qy[d]]==FR2)
				return (i+qx[d])*9+(j+qy[d]);
		} //for d
	} //if

	//find 432大皆空
	for (int i=0; i<9; i++)
	for (int j=0; j<9; j++){
		if (board[i][j]==FR4)
			return i*9+j;
		if (board[i][j]==FR3)
			return i*9+j;
		if (board[i][j]==FR2)
			return i*9+j;
	}
	//find 空
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			if (board[i][j]==FREE)
				return i*9+j;

	return 0;
}

int comp_move (bool col_ai) {
	if (rnd==1) {
		moveDisplay(board, 5, 5, col_ai);
		return 1;
	} else {
		int sumFree=board[4][9]+board[5][9]+board[6][9]+board[7][9];
		if (sumFree>0){
			int moveNum=best_ai(col_ai);
			int i=moveNum/9;
			int j=moveNum%9;
			moveDisplay(board, i, j, col_ai);
			return 1;
		} else{
			if (col_ai==WT){
				if (board[3][9]>0)
					for (int i=0; i<9; i++)
					for (int j=0; j<9; j++)
						if (board[i][j]==NBK2){
							moveDisplay(board, i, j, col_ai);
							return 1;}
				if (board[1][9]>0)
					for (int i=0; i<9; i++)
					for (int j=0; j<9; j++)
						if (board[i][j]==NBK1){
							moveDisplay(board, i, j, col_ai);
							return 1;}
			}
			else{
				if (board[2][9]>0)
					for (int i=0; i<9; i++)
					for (int j=0; j<9; j++)
						if (board[i][j]==NWT2){
							moveDisplay(board, i, j, col_ai);
							return 1;}
				if (board[0][9]>0)
					for (int i=0; i<9; i++)
					for (int j=0; j<9; j++)
						if (board[i][j]==NWT1){
							moveDisplay(board, i, j, col_ai);
							return 1;}
			}
		} //inside else
		return 0;
	}//else
}

void save()
{
	ofstream outfile;
	outfile.open("saved.txt");
	for (int i=0; i<9; i++) {
		for (int j=0; j<10; j++)
			outfile<<board[i][j]<<" ";
		outfile<<endl;
	}
	outfile<<rnd<<endl;
	outfile<<col<<endl;
	outfile.close();
}

void resume ()
{
	ifstream infile;
	infile.open ("saved.txt");
	for (int i=0; i<9; i++)
		for (int j=0; j<10; j++)
			infile>>board[i][j];
	infile>>rnd;
	infile>>col;
	infile.close();

}


int main()
{
	int UI=0, x=0, y=0;
	cout<<"Welcome to NoGo! To start new game, press"<<endl<<
			"(0) Start as White, (1) Start as Black"<<endl<<
			"Press (2) to resume previous game"<<endl;
	cin>>UI;
	while (UI>2 || UI<0)
	{
		cout<<"ERROR: Please input again"<<endl;
		cin>>UI;
	}
	switch (UI)
	{
		case WT:
			col=WT;
			initialise();
			comp_move(BK);
			break;
		case BK:
			col=BK;
			initialise();
			test_display();
			break;
		case 2:
			resume();
			test_display();
			break;
	}

	bool game=true;
	while (game)
	{
		cout<<"Please input coordinates for x (0-8) space y (0-8)"<<endl<<
					"Press (9 9) to save or end game"<<endl;
		while (cin>>x>>y)
		{
			if (x==9 && y==9){
				save();
				game=false;
				break;
			}
			if (!inBorder(x, y)){
				cout<<"Out of board, please input again"<<endl;
				continue;
			}
			if (board[x][y]==WT || board[x][y]==BK){
				cout<<"Place taken, please input again"<<endl;
				continue;
			}
			if (rnd==1)
				if (x==4 && y==4){
					cout<<"First step cannot be (4, 4), please input again"<<endl;
					continue;
				}
			break;
		}

		if (!game)
			break;

		if (col==WT)
			if (board[x][y]==NWT1 || board[x][y]==NWT2 || board[x][y]==DIE)
			{
				board[x][y]=WT; rnd++;
				test_display();
				cout<<"Game over! White has lost."<<endl;
				game=false;
				break;
			}
		if (col==BK)
			if (board[x][y]==NBK1 || board[x][y]==NBK2 || board[x][y]==DIE)
			{
				board[x][y]=BK; rnd++;
				test_display();
				cout<<"Game over! Black has lost."<<endl;
				game=false;
				break;
			}
		moveDisplay (board, x, y, col);
		int res=comp_move(!col);
		if (res==0){
			game=res;
			cout<<"Game over! Congratulations! You won!"<<endl;
			break;
		}
	}

	return 0;
}
