//
//  main.cpp
//  IDAStar
//
//  Created by Fu Juo Wen on 2017/4/24.
//  Copyright © 2017年 ntnu. All rights reserved.
//

#include <iostream>
#include <deque>
#include <map>
#include <string>
#include <stdio.h>
#include <limits.h>

/*
 
 1 ** (ans)
 2 *
 3 **
 4 *
   *
 5 ** (ans)
   **
 
 */

using namespace std;


struct Point
{
    int x=0, y=0;
};

map<string, int> visited;
string size[20];
int vnodes=0;



int M=3;
int N=3;

class State
{
public:
    int state[10][10]={{2,1,1},{0,2,2},{0,2,2}};
    //int state[10][10]={{0,4,1,1},{0,4,2,2},{3,3,2,2}};
    //int state[10][10]={{2,4,5,5},{2,4,5,5},{0,2,3,3},{2,0,2,2}};
    //int state[10][10]={{2,4,3,3,1,1},{2,4,2,2,2,2},{2,2,2,4,2,2},{0,0,2,4,2,2}};
    
    string ss;
    int depth=0;
    
    State()
    {
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; ++j)
            {
                ss.append(to_string(state[i][j]));
            }
        }
    }
    
    State(int _state[10][10], int _depth)
    {
        
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; ++j)
            {
                state[i][j]=_state[i][j];
                ss.append(to_string(state[i][j]));
            }
        }
        depth=_depth;
    }
    
    void copy(int tmp[M][N])
    {
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; j++)
            {
                tmp[i][j]=state[i][j];
            }
        }
    }
    
    void changeback(State s)
    {
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; j++)
            {
                state[i][j]=s.state[i][j];
            }
        }
        ss=s.ss;
    }
    
    
    
    void print(){
        // cout << "-----------------" << endl;
        //printf("Depth=%d\n", depth);
        for(int i=0; i<M; ++i){
            for(int j=0; j<N; ++j)
            {
                cout << state[i][j] << "\t";
            }
            cout << endl;
        }
        cout << "-----------------" << endl;
    }
    
    int findzero(Point p[]){
        int n=0;
        for (int i=0; i<M; i++){
            for(int j=0; j<N; j++)
                if (state[i][j]==0 )// find two blank position
                {
                    p[n].x=i;
                    p[n].y=j;
                    //printf("i=%d j=%d\n", i, j);
                    n++;
                }
        }
        return 0;
    }
    
    void exch(int i1, int j1, int i2, int j2) // exchange two blocks
    {
        int t=state[i2][j2];
        state[i2][j2]=state[i1][j1];
        state[i1][j1]=t;
        
        ss.clear();
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; ++j)
            {
                ss.append(to_string(state[i][j]));
            }
        }
    }
    
    int goal()// check goal state
    {
        if(state[M-1][0]==1 && state[M-1][1]==1)// goal block is **
        {
            return 1;
        }
        else if (state[M-1][0]==5 && state[M-1][1]==5 && state[M-2][0]==5 && state[M-2][1]==5)// goal block is 2*2
        {
            return 1;
        }
        else return 0;
    }
    
    void setVisited(int gx)
    {
        if(visited[ss]==0 || visited[ss]>gx) visited[ss]=gx;
    }
    
    int isVisited(string fs)
    {
        auto it=visited.find(fs);
        if(it!=visited.end()) return 1;
        else return 0;
    }
    
    
    int canAdd(string fs, int gx)
    {
        auto it=visited.find(fs);
        
        if(it==visited.end()) //not found in closed set
        {
            vnodes+=1;
            return 1;
        }
        else if(it->second>gx)//state in closed set is deeper
        {
            vnodes+=1;
            return 1;
        }
        return 0;
    }

    
    int h1() //Manhatten
    {
        int h;
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; j++)
            {
                if(state[i][j]==1||state[i][j]==5)
                {
                    return h=abs(M-1-i)+j;
                }
            }
        }
        return 0; //wrong
    }
    
    int h2() // misplaced
    {
        int m=0, ans=0;
        for(int i=0; i<M; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                if(state[i][j]==1)
                {
                    ans=1;
                }
                else if(state[i][j]==5)
                {
                    ans=5;
                }
            }
        }
        
        if(ans==1)
        {
            if(state[M-1][0]!=1) m+=1;
            if(state[M-1][1]!=1) m+=1;
        }
        else if(ans==5)
        {
            if(state[M-1][0]!=5) m+=1;
            if(state[M-1][1]!=5) m+=1;
            if(state[M-2][0]!=5) m+=1;
            if(state[M-2][1]!=5) m+=1;
        }
        return m;
    }
    
    int IDAstar(int gx, int &flimit, bool &ans)
    {
        
        if(gx==0) //new round, clear all visited state
        {
            visited.clear();
            //printf("clear visited\n");
        }
        setVisited(gx);
        
        int hx=h2(); //count heuristic
        if(gx+hx>flimit)
        {
            //printf("gx+hx>f-limit gx=%d hx=%d\n", gx, hx);
            return gx+hx;
        }
        if(goal()) {ans=true; printf("gx(depth)= %d\n", gx); return gx;}
        
        int nextf=INT_MAX;
        
        Point p0[2];
        
        findzero(p0);
        
        int x[2]={0},y[2]={0};
        x[0]=p0[0].x;
        y[0]=p0[0].y;
        x[1]=p0[1].x;
        y[1]=p0[1].y;
        
        
        printf("%d %d %d %d\n", x[0],y[0], x[1], y[1]);
        printf("f-limit=%d\n", flimit);
        print();
        
        
        for(int i=0; i<2; ++i)
        {
            if(x[i]!=M-1)//not last row, can move up
            {
                // printf("1\n");
                State inits(state,depth+1);

                if(state[x[i]+1][y[i]]!=0)
                {
                    if(state[x[i]+1][y[i]]==2) //*
                    {
                        exch(x[i], y[i], x[i]+1, y[i]);
                    }
                    else if(state[x[i]+1][y[i]]==4) //*
                    {                               //*
                        exch(x[i], y[i], x[i]+2, y[i]);
                    }
                    else if(state[x[i]+1][y[i]]==1 || state[x[i]+1][y[i]]==3) //**
                    {
                        if(y[i]!=N-1)// not most right
                        {
                            if(state[x[i]][y[i]+1]==0 && state[x[i]+1][y[i]+1]==state[x[i]+1][y[i]])// 0's rhs is also 0
                            {
                                exch(x[i], y[i], x[i]+1, y[i]);
                                exch(x[i], y[i]+1, x[i]+1, y[i]+1);
                            }
                        }
                    }
                    else if(state[x[i]+1][y[i]]==5) // 2*2
                    {
                        if(y[i]!=N-1 && x[i]<M-2) // not most right and not bottom
                        {
                            if(state[x[i]][y[i]+1]==0 && state[x[i]+1][y[i]+1]==5 && state[x[i]+2][y[i]]==5 && state[x[i]+2][y[i]+1]==5)
                            {
                                exch(x[i], y[i], x[i]+2, y[i]);
                                exch(x[i], y[i]+1, x[i]+2, y[i]+1);
                            }
                        }
                    }
                    if(canAdd(ss, gx+1))
                    {
                        setVisited(gx+1);
                        int c = IDAstar(gx+1, flimit, ans);
                        if(ans) return c;
                        nextf=min(nextf,c);
                    }
                    changeback(inits);

                }
            }
            if(x[i]!=0)// not first row, can move down
            {
                //printf("2\n");
                
                if(state[x[i]-1][y[i]]!=0)
                {
                    State inits(state,depth+1);
                    
                    if(state[x[i]-1][y[i]]==2) //*
                    {
                        exch(x[i], y[i], x[i]-1, y[i]);
                    }
                    else if(state[x[i]-1][y[i]]==4) //*
                    {                               //*
                        exch(x[i], y[i], x[i]-2, y[i]);
                    }
                    else if(state[x[i]-1][y[i]]==1 || state[x[i]-1][y[i]]==3) //**
                    {
                        if(y[i]!=N-1)// not most right
                        {
                            if(state[x[i]][y[i]+1]==0 && state[x[i]-1][y[i]+1]==state[x[i]-1][y[i]])// 0's rhs is also 0
                            {
                                exch(x[i], y[i], x[i]-1, y[i]);
                                exch(x[i], y[i]+1, x[i]-1, y[i]+1);
                            }
                        }
                    }
                    else if(state[x[i]-1][y[i]]==5) //2*2
                    {
                        if(y[i]!=N-1 && x[i]>1)
                        {
                            if(state[x[i]][y[i]+1]==0 && state[x[i]-1][y[i]+1]==5 && state[x[i]-2][y[i]]==5 && state[x[i]-2][y[i]+1])// 0's rhs is also 0
                            {
                                exch(x[i], y[i], x[i]-2, y[i]);
                                exch(x[i], y[i]+1, x[i]-2, y[i]+1);
                            }
                        }
                    }
                    
                    if(canAdd(ss, gx+1))
                    {
                        setVisited(gx+1);
                        //printf("down %d\n", i);
                        //print();
                        int c = IDAstar(gx+1, flimit, ans);
                        if(ans) return c;
                        nextf=min(nextf,c);
                    }
                    changeback(inits);

                }
                
            }
            if(y[i]!=N-1)// not most right col, can move left
            {
                //printf("3\n");
                
                if(state[x[i]][y[i]+1]!=0)
                {
                    
                    State inits(state,depth+1);
                    
                    if(state[x[i]][y[i]+1]==2) //*
                    {
                        exch(x[i], y[i], x[i], y[i]+1);
                    }
                    else if (state[x[i]][y[i]+1]== 3 || state[x[i]][y[i]+1] == 1) //**
                    {
                        exch(x[i], y[i], x[i], y[i]+2);
                    }
                    else if (state[x[i]][y[i]+1]==4) //*
                    {                                //*
                        if(x[i]!=M-1)//not button row
                        {
                            if(state[x[i]+1][y[i]]==0 && state[x[i]+1][y[i]+1]==state[x[i]][y[i]+1])// below 0 is also 0
                            {
                                exch(x[i], y[i], x[i], y[i]+1);
                                exch(x[i]+1, y[i], x[i]+1, y[i]+1);
                            }
                        }
                    }
                    else if(state[x[i]][y[i]+1]==5) //2*2
                    {
                        if(x[i]!=M-1 && y[i]<N-2)//not button row
                        {
                            if(state[x[i]+1][y[i]]==0 && state[x[i]+1][y[i]+1]==5 && state[x[i]][y[i]+2]==5 && state[x[i]+1][y[i]+2]==5)// below 0 is also 0
                            {
                                exch(x[i], y[i], x[i], y[i]+2);
                                exch(x[i]+1, y[i], x[i]+1, y[i]+2);
                            }
                        }
                        
                    }
                    if(canAdd(ss, gx+1))
                    {
                        setVisited(gx+1);
                        //printf("left %d\n", i);
                        //print();
                        int c = IDAstar(gx+1, flimit, ans);
                        if(ans) return c;
                        nextf=min(nextf,c);
                    }
                    changeback(inits);

                }

            }
            if(y[i]!=0)//not most left col, can move right
            {
                //printf("4\n");
                
                if(state[x[i]][y[i]-1]!=0)
                {
                    State inits(state,depth+1);
                    
                    if(state[x[i]][y[i]-1]==2) //*
                    {
                        exch(x[i], y[i], x[i], y[i]-1);
                    }
                    else if (state[x[i]][y[i]-1]==3 || state[x[i]][y[i]-1]==1) //**
                    {
                        exch(x[i], y[i], x[i], y[i]-2);
                    }
                    else if(state[x[i]][y[i]-1]==4) //*
                    {                               //*
                        if(x[i]!=M-1)//not button row
                        {
                            if(state[x[i]+1][y[i]]==0 && state[x[i]+1][y[i]-1]==state[x[i]][y[i]-1])// below 0 is also 0
                            {
                                exch(x[i], y[i], x[i], y[i]-1);
                                exch(x[i]+1, y[i], x[i]+1, y[i]-1);
                            }
                        }
                    }
                    else if(state[x[i]][y[i]-1]==5) //2*2
                    {
                        if(x[i]!=M-1 && y[i]>1)//not button row
                        {
                            if(state[x[i]+1][y[i]]==0 && state[x[i]+1][y[i]-1]==5 && state[x[i]][y[i]-2]==5 && state[x[i]+1][y[i]-2]==5)// below 0 is also 0[
                            {
                                exch(x[i], y[i], x[i], y[i]-2);
                                exch(x[i]+1, y[i], x[i]+1, y[i]-2);
                            }
                        }
                    }
                    if(canAdd(ss, gx+1))
                    {
                        setVisited(gx+1);
                        //printf("right %d\n", i);
                        //print();
                        int c = IDAstar(gx+1, flimit, ans);
                        if(ans) return c;
                        nextf=min(nextf,c);
                    }
                    changeback(inits);

                }
            }
        }
        
        return  nextf;
    }
    
};

int main(int argc, const char * argv[]) {
    
    
    State initS;
    
    FILE *fp = fopen("level1.txt", "r");
    
    fscanf(fp, "%d %d", &M, &N);
    for(int i=0; i<M; ++i)
    {
        for(int j=0; j<N; ++j)
        {
            fscanf(fp, "%d", &initS.state[i][j]);
        }
    }
    
    clock_t t1,t2;
    t1=clock();
    
    bool ans=false;
    int flimit=initS.h1(), max_flimit=100;
    while(!ans && flimit<=max_flimit)
    {
        flimit=initS.IDAstar(0, flimit, ans);
    }
    if(!ans)
    {
        cout << "Cannot find answer within f-limit <= " << max_flimit << endl;
    }
    t2=clock();
    printf("Total time = %f seconds.\n", ((double)t2-(double)t1)/CLOCKS_PER_SEC);
    printf("Total nodes = %d\n", vnodes);

    return 0;
}
