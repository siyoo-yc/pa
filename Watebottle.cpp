/****************************************************************************************************************************
*  Parser���: ����һƪ�����г��ֵĹؼ����Լ����ؽ��ʵĸ���. �Թؼ���������Ҫ��:
*  1. ���ʿ�ͷ��ĸ�Ĵ�Сд��ͬһ�ؼ���.
*  2. �޶��ؼ���. ( ��ȥ�򵥴�: ȥ���, ȥ���, ȥһЩ���ô�, �� I, you, he, she, a,...
*  3. ��ͬʱ̬��ͬһ�ʸ��ĵ�������ͬһ�ؼ���. ( �Զ���ȥʱ̬���� )
*  4. ���������ֵ���ͬ��������ͬһ�ؼ���. ( ������ȥ���������� )
*  5. ����Ž������ԭ���ʸ�Ϊ׼����ͬһ�ؼ���.
*  �������: ֧�ִ�Դ�ļ� Input.txt ��������, ����ɴ�DOS��Ŀ���ļ� Output.txt ���.
*****************************************************************************************************************************/
#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iomanip>
#include "EMAParser.h"
#include "EMA.h"
#include "SParser.h"
#define _MAXLEN 15
#define NUM 40
#define K 4

using namespace std;

typedef struct 
{
	char  key[_MAXLEN]; //Limite the length (< 10) of key word 
	int iKeyCnt;  //The times the key occurs
}ELEM;

typedef struct  //����tfidf
{
	ELEM elem[1000];
	int count; //�����йؼ��ʵ�����
	float tf[1000];
	float idf[1000];
	float tf_idf[1000];
}tfidf;
tfidf ti[NUM];

double sum=0;    //��Ϊ���
double pro1=0;   
double pro2=0;
static cluster[NUM]; //��žۺϽ��
static vector <char *>  key_x;
static vector <float> key_y;   //key is x; tfidf is y
static vector<vector<float> > key_words(NUM);  //
double cosine[K][NUM]={0};  //ѡ�е��ĵ��������ĵ���cosֵ

int IsKeyExist(ELEM*, int , char*);  //Prototype
void DealKeyWord(ELEM*, int&, char*);//Prototype
void SortbyKey(ELEM*, int);//Prototype
void tf_idf_work(tfidf ti[NUM]);
void kmeans(tfidf ti[NUM]);
void iteration(); //����

int main()
{
	CEMAParser parser;
	int f = 0;
for(int s=0;s<NUM;s++)
{
    char str[20];
	sprintf(str,"%d.txt",s);     //sprintf�����͸�ʽ�������str���ַ���
	char * pArticle = new char[3000];
    ofstream fout("Output.txt");
	ifstream fin(str);
	if(!fout)
	    printf( "Can't open file Output.txt.\n" );
    else
	{
	  cout << "Open file Output.txt successfully." << endl;
	  fout << "Open file Output.txt successfully." << endl;
	} 
    if(!fin)
	{
	    cout << "Can't open file Input.txt." << endl;
        fout << "Can't optn file Input.txt." << endl;
	}
	else
	{
	    cout << "Open file Input.txt successfully.\n" << endl;
        fout << "Open file Input.txt successfully.\n" << endl;
	}
    //fgets( pArticle, 2000, fp ); //���ļ������� n - 1���ַ�, ��n��'\0'
	char ch;
	for(int i = 0;  i < 3000 && (fin.get(ch) != 0); i++)
	{
		cout << ch;
        fout << ch;
		if(ch != '\n')
            pArticle[i] = ch;
	    else
	        pArticle[i] = ' ';

	}
	pArticle[i] = '\0';   // Imitate the pArticle as a string
    fin.close();
	cout << endl;
	fout << endl;
	parser.Open(); // pen Parser
    parser.GetStems(pArticle);// Analyse the Parser
    int kn = parser.GetKeyNumber(); // Get the total number of key words
	cout << "\nThere are" << kn << "key words in the article without caring about redundance.\n";
	fout << "\n\nThere are" << kn << "key words in the article without caring about redundance.\n";

	char * pKey;
	ELEM pElem[1000];  // Limite the key words less than 500
    int Maxkeyfrequence = 0;
	int iElemCnt = 0;
    for( int j = 0; j < kn; j++ )
	{
	    pKey = parser.GetKey(j); //Get a key word
	    DealKeyWord(pElem, iElemCnt, pKey);
	}
    SortbyKey( pElem, iElemCnt  );
   	cout << "There are" << iElemCnt << "key words in the article after geting rid of redundance.\n";
	fout << "There are" << iElemCnt << "key words in the article after geting rid of redundance.\n";
	for( int m = 0; m < iElemCnt; m++ )
	{
		strcpy(ti[s].elem[m].key,pElem[m].key);
		ti[s].elem[m].iKeyCnt=pElem[m].iKeyCnt;
		ti[s].count=m;
		if(Maxkeyfrequence<pElem[ m ].iKeyCnt)
			Maxkeyfrequence=pElem[m].iKeyCnt;
/*	    cout << pElem[ m ].key << " occurrence: " << pElem[ m ].iKeyCnt << endl;
	    fout << pElem[ m ].key << " occurrence: " << pElem[ m ].iKeyCnt << endl;
*/	}
	cout<<"Maxkeyfrequence:"<<Maxkeyfrequence<<endl;
	for(i=0;i<ti[s].count;i++)//���tf
	{
		ti[s].tf[i]=(float)pElem[i].iKeyCnt/(float)Maxkeyfrequence;
		//cout<<ti[s].elem[i].key<<" tf="<<ti[s].tf[i]<<endl;
	}
	
   	fout.close();
}
	tf_idf_work(ti);
	kmeans(ti);
    return 0;
}

int IsKeyExist(ELEM* pElem, int iElemCnt, char* pKey)
{
	for(int i = 0; i < iElemCnt; i++)  //iElemCnt�ǵ�ǰ�ṹ���ŵĸ���
	{
		if(strcmp( pElem[i].key, pKey) == 0)
			return i;
	}
	return -1;
}

void DealKeyWord( ELEM* pElem, int& iElemCnt, char* pKey )
{
	int iIndex = IsKeyExist( pElem, iElemCnt, pKey );
	if( iIndex > -1 ) // if it exists
	   pElem[iIndex].iKeyCnt++;
	else    
	{
	/*	
		int length = strlen(pKey); 
		length = (length < 10 ? length: 10);
		strncpy(pElem[iElemCnt].key, pKey, length);	
		pElem[iElemCnt].key[length] = '\0';
	*/
		memcpy(pElem[iElemCnt].key, pKey, _MAXLEN); //Memory copy
		pElem[iElemCnt].iKeyCnt = 1;
		iElemCnt++;
	}
}

void SortbyKey( ELEM *pElem, int iElemCnt ) //Sort by key word
{
	for( int i = 1; i <= iElemCnt; i++ )
		for( int j = 0; j < iElemCnt - i - 1; j++ )
		{
			if(strcmp(pElem[ j ].key, pElem[ j + 1 ].key) > 0)
			{
				ELEM temp = pElem[j + 1];
			    pElem[j + 1] = pElem[j];
				pElem[j] = temp;
			}
		}
}
	
void tf_idf_work(tfidf ti[NUM])
{
	int co = 0;

	ofstream fout("Output.txt");
	for (int x = 0; x < 40; x++)
		{
			for (int y = 0; y < ti[x].count; y++)
			{
				co = 0;
				for (int x1 = 0; x1 < 40; x1++) 
				{
					if (x1 == x) continue;
					for (int y1 = 0; y1 < ti[x1].count; y1++)
					{
						if (strcmp(ti[x].elem[y].key, ti[x1].elem[y1].key) == 0)
						{
							co++;
							break;
						}
					}
				}
				ti[x].idf[y] = log( (float)40/ (1+co) )   /   log(2);
				ti[x].tf_idf[y] = ti[x].tf[y] * ti[x].idf[y];
				cout << "NUM:"<<x<<" "<<ti[x].elem[y].key << " occurrence: " << ti[x].elem[y].iKeyCnt <<" tf="<<ti[x].tf[y]<<" idf="<<ti[x].idf[y]<< " tf_idf="<<ti[x].tf_idf[y]<<endl;
				fout << "NUM:"<<x<<" "<<ti[x].elem[y].key << " occurrence: " << ti[x].elem[y].iKeyCnt <<" tf="<<ti[x].tf[y]<<" idf="<<ti[x].idf[y]<< " tf_idf="<<ti[x].tf_idf[y]<<endl;

			}
	}
}

void kmeans(tfidf ti[NUM])
{
	
	
	for(int m=0;m<NUM;m++)
	{
		for(int n=0;n<ti[m].count;n++)
		{	
			key_words[m][n]=ti[m].tf_idf[n];
		}
	}

	//����ѡ�е�4ƪ�ĵ��������ĵ���cos
	cout<<"Enter "<<K<<" centers"<<endl;
	int center[K];
	for(int i=0;i<K;i++)
	{
		cin>>center[i];
		cout<<"next"<<endl;
	}
	
	for(int x=0;x<NUM;x++)
	{
		for(int k=0;k<K;k++)
		{
			for(int y=0;y<ti[x].count;y++)
			{
				sum+=key_words[x][y]*key_words[center[k]][y];
				pro1+=key_words[x][y]*key_words[x][y];
				pro2+=key_words[center[k]][y]*key_words[center[k]][y];
			}
		cosine[k][x]=sum/( sqrt(pro1)*sqrt(pro2) );
		cout<<"sum="<<sum;
		cout<<"pro1="<<pro1;
		cout<<"pro2="<<pro2;
		cout<<endl;
		sum=0;
		pro1=0;
		pro2=0;

		}
	}
	for(int k=0;k<K;k++)
	{
		for(x=0;x<NUM;x++)
		{
			cout<<setprecision(5)<<fixed<<cosine[k][x]<<" ";
		}
	}

	cout<<endl;
	iteration();
	
}


//���е��������ϵõ��µľۺ�����
void iteration()
{
	static double cosmax=0;
	static int KD=0; //����kֵ
	static int count=0;
	static double newcenter[K][1000]; 
	int clu1[NUM];
	int bf=20;
	while(bf)
	{
		for(int x=0;x<NUM;x++)
		{
			for(int k=0;k<K;k++)
			{
				if(cosine[k][x]>cosmax)
				{
					cosmax=cosine[k][x];
					KD=k;
				}
			}
			cosmax=0;
			cluster[x]=KD;
		}
/*		for(x=0;x<NUM;x++)
		{
			cout<<cluster[x];
		}
*/
	
	//ѡ���µľۺ�����
		for(int k=0;k<K;k++)
		{
			for(int m=0;m<NUM;m++)
			{
				if(cluster[m]==k)
				{
					count++;
					for(int n=0;n<ti[m].count;n++)
						newcenter[k][n]+=key_words[m][n];

				}
				for(int n=0;n<ti[m].count;n++)
				{
					if(count)
						newcenter[k][n]/=count;	
				//	cout<<"new "<<newcenter[k][n];
				}
			}
			count=0;
		}

		//����cosֵ
		for( x=0;x<NUM;x++)
		{
			for(int k=0;k<K;k++)
			{
				for(int y=0;y<ti[x].count;y++)
				{
					sum+=key_words[x][y]*newcenter[k][y];
					pro1+=key_words[x][y]*key_words[x][y];
					pro2+=newcenter[k][y]*newcenter[k][y];
				}
			cosine[k][x]=sum/( sqrt(pro1)*sqrt(pro2) );
			sum=0;
			pro1=0;
			pro2=0;
			cout<<setprecision(5)<<fixed<<cosine[k][x]<<" ";
			}
		}
		bf--;

	}
	cout<<endl;
	
		
		for(int x=0;x<NUM;x++)
		{
			cout<<cluster[x]<<"  ";
		}
		cout<<endl;
		cout<<"cluster1:";
		for(int n=0;n<NUM;n++)
		{
			if(cluster[n]==0)
				cout<<n+1<<" ";
		}
		cout<<endl;
		cout<<"cluster2:";
		for( n=0;n<NUM;n++)
		{
			if(cluster[n]==1)
				cout<<n+1<<" ";
		}
		cout<<endl;
		cout<<"cluster3:";
		for(n=0;n<NUM;n++)
		{
			if(cluster[n]==2)
				cout<<n+1<<" ";
		}
		cout<<endl;
		cout<<"cluster4:";
		for( n=0;n<NUM;n++)
		{
			if(cluster[n]==3)
				cout<<n+1<<" ";
		}

}
