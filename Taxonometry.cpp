#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEPTH 100
#define CLEAR "clear"
//#define CLEAR "cls"
using namespace std;

typedef char* DataType;
//This is just description of BTNode (BT means binary tree in this case) class, which 
//will have a string of question/answer and two pointers to BTNode.
class BTNode{

private:

	DataType data;
	BTNode* pLeft;
	BTNode *pRight;

public:

	BTNode(const DataType& _data=DataType(),BTNode *_pLeft=NULL,BTNode *_pRight=NULL){
	data=_data;
	pLeft=_pLeft;
	pRight=_pRight;
	}
	
	DataType getData() const{return data;};
	void setData(const DataType& _data){data=_data;};
	const BTNode* getpLeft() const{return pLeft;};
	BTNode* getpLeft(){return pLeft;};
	void setpLeft(BTNode* _pLeft){pLeft=_pLeft;};
	const BTNode* getpRight() const{return pRight;};
	BTNode* getpRight(){return pRight;};
	void setpRight(BTNode* _pRight){pRight=_pRight;};
};

/*Let's init a tree with a couple of questions and suggestions*/
void initTree(BTNode*& root){
	BTNode *tmp_left=new BTNode("Is it stripy");
	BTNode *tmp_right=new BTNode("Is it blind");
	root->setpLeft(tmp_left);
	root->setpRight(tmp_right);
	BTNode *tmp_left_left=new BTNode("Tiger");
	BTNode *tmp_left_right=new BTNode("Cat");
	BTNode *tmp_right_left=new BTNode("Mole");
	BTNode *tmp_right_right=new BTNode("Dog");
	tmp_left_left->setpLeft(NULL);
	tmp_left_left->setpRight(NULL);
	tmp_right_left->setpLeft(NULL);
	tmp_right_right->setpRight(NULL);
	tmp_left->setpLeft(tmp_left_left);
	tmp_left->setpRight(tmp_left_right);
	tmp_right->setpLeft(tmp_right_left);
	tmp_right->setpRight(tmp_right_right);
}

bool isnLeaf(BTNode* root){
	return (root->getpLeft())||(root->getpRight());
}

/*So, we don't know the right answer, let's ask a user for it*/
void start_education(BTNode*& root){
	char* right_answer = new char[20];
	char* question = new char[80];
	char ans;
	question[0]='\0';
	right_answer[0]='\0';
	cout<<"What was the right answer?"<<endl;
	while(!strlen(right_answer))gets(right_answer);
	cout<<"What question I should ask to tell one from another?"<<endl;
	while(!strlen(question))gets(question);
	cout<<"Is it true for "<<right_answer<<"?"<<endl;
	cin>>ans;
	
		BTNode* tmp_right;
		BTNode* tmp_left;
		if (ans=='y') {
		tmp_right = new BTNode(root->getData());
		tmp_left = new BTNode(right_answer);
		}
		else
		{
		tmp_left = new BTNode(root->getData());
		tmp_right = new BTNode(right_answer);
		}
		
	root->setData(question);
	root->setpLeft(tmp_left);
	root->setpRight(tmp_right);
}

/*Save evrything we have in file*/
void save_the_tree(FILE* f,BTNode *root, char *& s){
	if(!root)return;
	char * tmp_str=new char[DEPTH];
	strcpy(tmp_str,s);
	fprintf(f,"[%s]%s\n",s,root->getData());
	strcat(s,"y");
	save_the_tree(f,root->getpLeft(), s);
	strcpy(s,tmp_str);
	strcat(s,"n");	
//	printf("%s\n--",s);
	save_the_tree(f,root->getpRight(), s);
}

/*If we have a file data.txt, let's restore information from it*/
void restore(FILE *f,BTNode * root){
	while(getc(f)!=EOF){;
	char *path = new char[DEPTH];
	char *data = new char[80];
	char tmp;
	int i=0;
/*Getting the path*/
	while((tmp=getc(f))!=']')path[i++]=tmp;
	path[i]='\0';
/*Getting the data*/
	i=0;
	while((tmp=getc(f))!='\n')data[i++]=tmp;
	data[i]='\0';
/**/
	BTNode * next = new BTNode(data);
	BTNode * ptmp = root;
	for(int j=0;j<strlen(path)-1;j++){
	if(path[j]=='y')ptmp = ptmp->getpLeft();
	if(path[j]=='n')ptmp = ptmp->getpRight();
}
	if(path[strlen(path)-1]=='y')ptmp->setpLeft(next);
	if(path[strlen(path)-1]=='n')ptmp->setpRight(next);
}
}


int main(int argc, char ** argv){
	BTNode* root=new BTNode("Does it say \"Meow\"");
	BTNode* tmp_root=root;
	char ans;
	FILE* f;
	f=fopen("data","r");
	if(f)restore(f,root);
	else initTree(root);
	fclose(f);
do{
	system(CLEAR);
	root=tmp_root;
while(isnLeaf(root)){
	cout<<root->getData()<<"[y/n]?"<<endl;
	cin>>ans;
	ans=='y'?root=root->getpLeft():root=root->getpRight();
};
cout<<endl<<"Your animal is "<<root->getData()<<endl<<"Am I right?"<<endl;
	cin>>ans;
	if (ans=='y')cout<<"I win!"<<endl;
	else start_education(root);
	cout<<"One more time?"<<endl;
	cin>>ans;
}while(ans=='y');	
	root=tmp_root;
	char* s=new char[DEPTH];
	int level=1;
	f=fopen("data","w");
	s[0]='y';
	s[1]='\0';
	save_the_tree(f,root->getpLeft(),s);
	s[0]='n';
	s[1]='\0';
	save_the_tree(f,root->getpRight(),s);
	fclose(f);
}
