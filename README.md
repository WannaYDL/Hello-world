# Hello-world！ I'm a student in China!
#include<iostream>
#include<cmath>
using namespace std;
int main()
{
    int m,n,i;
    while(cin>>m>>n)
    {
        int j=0;
        int k=0;
    	for(i=m;i<=n;i++)
    	{
            int r,s,t;
            r=i%10;s=i/10%10;t=i/100;
    		if(i==r*r*r+s*s*s+t*t*t){
                if(j==0){
                    cout<<i;
                    j++;
                }
                else
                cout<<" "<<i;
                k++;
    		}
    	}
        if(k!=0)
            cout<<endl;
          if(k==0)
		cout<<"no"<<endl;
    }
    return 0;
}

