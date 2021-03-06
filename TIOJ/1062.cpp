#include <iostream>

using namespace std;

int sum[101][101], a[101][101];

int main(){
    int i, j, l, r, n, ans, x=1, y=1;
    
    memset(sum, 0, sizeof(sum));
    a[1][1]=0;
    scanf("%d%d", &l, &r);
    for( i=1; i<=l; i++ )
        for( j=1; j<=r; j++ )
        {
            scanf("%d", &n);
            sum[i][j]=sum[i-1][j]+sum[i][j-1]-sum[i-1][j-1]+n;
            a[1][1]+=n*(i+j-2);   // (i-1)+(j-1)
        }
    ans=a[1][1];
    for( i=2; i<=l; i++ )
    {
        a[i][1]=a[i-1][1]+ sum[i-1][r]*2-sum[l][r];  // sum[i-1][r]-(sum[l][r]-sum[i-1][r])
        if( a[i][1]<ans ) { ans=a[i][1]; x=i; y=1; }
    }
    for( i=1; i<=l; i++ )
        for( j=2; j<=r; j++ )
        {
            a[i][j]=a[i][j-1]+ sum[l][j-1]*2-sum[l][r];  // sum[l][j-1]-(sum[l][r]-sum[l][j-1])
            if( a[i][j]<ans ) { ans=a[i][j]; x=i; y=j; }
        }
    printf("%d %d\n", x, y);
    return 0;
}