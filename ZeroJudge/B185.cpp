#include <iostream>

using namespace std;

const int maxrow=6;
const int maxcol=6;

bool a[maxrow][maxcol],aDFS[2][maxcol],aExe[maxrow][maxcol];
int t,row,col,minclicks;
char s[maxrow+1];

void clickD(int x,int y){
    aDFS[x][y]^=1;
    if(x>0) aDFS[x-1][y]^=1;
    if(y>0) aDFS[x][y-1]^=1;
    if(x<row-1) aDFS[x+1][y]^=1;
    if(y<col-1) aDFS[x][y+1]^=1;
}

void clickE(int x,int y){
    aExe[x][y]^=1;
    if(x>0) aExe[x-1][y]^=1;
    if(y>0) aExe[x][y-1]^=1;
    if(x<row-1) aExe[x+1][y]^=1;
    if(y<col-1) aExe[x][y+1]^=1;
}

void execute(int ct){
    int bct=ct;
    for(int i=1;i<row;i++)
        for(int j=0;j<col;j++)
            if(!aExe[i-1][j]){
                clickE(i,j);
                ct++;
            }
    for(int j=0;j<col;j++)
        if(!aExe[row-1][j]) return ;
    minclicks=min(minclicks,ct);
}

void dfs(int count,int clicks){
    if(count<col){
        dfs(count+1,clicks);
        clickD(0,count);
        dfs(count+1,clicks+1);
        clickD(0,count);
    }else{
        for(int i=0;i<2;i++)
            for(int j=0;j<col;j++)
                aExe[i][j]=aDFS[i][j];
        for(int i=2;i<row;i++)
            for(int j=0;j<col;j++)
                aExe[i][j]=a[i][j];
        execute(clicks);
    }
}

int main(){
    while(scanf("%d%d",&row,&col)==2)
    {
        gets(s);
        for(int i=0;i<row;i++)
        {
            gets(s);
            for(int j=0;j<col;j++)
            {
                a[i][j]=s[j]=='X';
                if(i<2) aDFS[i][j]=s[j]=='X';
            }
        }
        minclicks=2147483647;
        dfs(0,0);
        if(minclicks==2147483647) printf("Can not\n");
        else printf("Minimum Steps :%d\n",minclicks);
    }
    return 0;
}
