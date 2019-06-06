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
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "EMAParser.h"
#include "EMA.h"
#include "SParser.h"
#define _MAXLEN 15

using namespace std;

typedef struct 
{
	char  key[_MAXLEN]; //Limite the length (< 10) of key word 
	int iKeyCnt;  //The times the key occurs
}ELEM;


int IsKeyExist(ELEM*, int , char*);  //Prototype
void DealKeyWord(ELEM*, int&, char*);//Prototype
void SortbyKey(ELEM*, int);//Prototype

int main()
{
	CEMAParser parser;
	int f = 0;
	char * pArticle = new char[3000];
    ofstream fout("Output.txt");
	ifstream fin("Input.txt");
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
	    cout << pElem[ m ].key << " occurrence: " << pElem[ m ].iKeyCnt << endl;
	    fout << pElem[ m ].key << " occurrence: " << pElem[ m ].iKeyCnt << endl;
	}
   	fout.close();
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
	