#include <stdio.h>
#include <stdlib.h>
typedef struct cell{
	int state[2]; //state[0]=current iteration; state[1]=previous iteration
}cell;
void read(int lines,int cols,cell*** matrix,const char* input){
	FILE *f;
	f=fopen(input,"r");
	if(!f){
		printf("Input file not found");
		exit(1);
	}
	int x,y;
	(*matrix) =(cell**)malloc((lines+2)*sizeof(cell*));
	for(int i=0;i<=lines+1;i++){
		(*matrix)[i]=(cell*)malloc((cols+2)*sizeof(cell));
		for(int j=0;j<=cols+1;j++){
			(*matrix)[i][j].state[0]=0;
			(*matrix)[i][j].state[1]=0;}
	}
	while(!feof(f)){
		fscanf(f,"%d%d",&x,&y);
		(*matrix)[x+1][y+1].state[0]=1;
		if(x==0 || x==lines-1)
			(*matrix)[lines-x+1][y+1].state[0]=1;
		if(y==0 || y==cols-1)
			(*matrix)[x+1][cols-y+1].state[0]=1;
	}
	fclose(f);
	(*matrix)[0][0].state[0]=(*matrix)[lines][cols].state[0];
	(*matrix)[0][cols+1].state[0]=(*matrix)[lines][1].state[0];
	(*matrix)[lines+1][0].state[0]=(*matrix)[1][cols].state[0];
	(*matrix)[lines+1][cols+1].state[0]=(*matrix)[1][1].state[0];
	for(int i=0;i<=lines+1;i++){
		for(int j=0;j<=cols+1;j++){
			(*matrix)[i][j].state[1]=(*matrix)[i][j].state[0];
		}
	}
}
int neighbors(int lines,int cols,int x,int y,cell** matrix){
	int neighbor=0;
	neighbor+=matrix[x-1][y].state[1]+matrix[x-1][y-1].state[1]+matrix[x-1][y+1].state[1]+matrix[x][y-1].state[1]+matrix[x][y+1].state[1]+matrix[x+1][y-1].state[1]+matrix[x+1][y].state[1]+matrix[x+1][y+1].state[1];
	return neighbor;
}
void printBoard(int lines,int cols,cell** matrix){
	for(int i=1;i<=lines;i++){
		for(int j=1;j<=cols;j++){
			if(matrix[i][j].state[0]==1)
				printf("%c",219);
			else
				printf(" ");
		}
		printf("\n");
	}
	system("cls");
}
void simulation(int lines,int cols,int iterations,cell*** matrix){
	int a;
	for(int k=0;k<iterations;k++){
		for(int i=1;i<=lines;i++){
			for(int j=1;j<=cols;j++){
				int alive=neighbors(lines,cols,i,j,*matrix);
				if((*matrix)[i][j].state[1]==0)
					if(alive==3)
						(*matrix)[i][j].state[0]=1;
					else 
						(*matrix)[i][j].state[0]=0;
				if((*matrix)[i][j].state[1]==1 && alive<2)
					(*matrix)[i][j].state[0]=0;
				if((*matrix)[i][j].state[1]==1 && alive>3)
					(*matrix)[i][j].state[0]=0;
				if((*matrix)[i][j].state[1]==1 && (alive==3 || alive==2))
					(*matrix)[i][j].state[0]=1;
				if(i==1 || i==lines)
					(*matrix)[lines-i][j].state[0]=(*matrix)[i][j].state[1];
				if(j==1 || j==cols)
					(*matrix)[i][cols-j].state[0]=(*matrix)[i][j].state[1];
			}
		}
		(*matrix)[0][0].state[0]=(*matrix)[lines][cols].state[0];
		(*matrix)[0][cols+1].state[0]=(*matrix)[lines][1].state[0];
		(*matrix)[lines+1][0].state[0]=(*matrix)[1][cols].state[0];
		(*matrix)[lines+1][cols+1].state[0]=(*matrix)[1][1].state[0];
		printBoard(lines, cols,*matrix);
		for(int i=0;i<=lines+1;i++){
			for(int j=0;j<=cols+1;j++){
				(*matrix)[i][j].state[1]=(*matrix)[i][j].state[0];
			}
		}
	}
}
int main(int argc,const char *argv[]){
	if(argc!=5){
		printf("%s [inputfile] [lines] [columns] [nr-iterations]\n",argv[0]);
		exit(1);
	}
	int lines=atoi(argv[2]);
	int cols=atoi(argv[3]);
	int iterations=atoi(argv[4]);
	if(lines<1 || cols<1 || iterations<1){
		printf("%s [lines] [columns] [inputfile] [nr-iterations]\n",argv[0]);
		exit(1);	
	}
	cell **matrix;
	read(lines,cols,&matrix,argv[1]);
	printBoard(lines,cols,matrix);
	simulation(lines,cols,iterations,&matrix);
	for(int i=0;i<lines;i++)
		free(matrix[i]);
	free(matrix);
	return 0;
}