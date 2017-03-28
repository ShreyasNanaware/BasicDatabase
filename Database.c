/*
 ============================================================================
 Name        : DynamicDatabase.c
 Authors     : Shreyas Nanaware
 Version     : 1.0
 Description : A Database that allows to choose field name and type.
 	 	 	   It loads data and fields from the file.
 ============================================================================
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define ST1 sizeof(int)
#define ST2 sizeof(double)
#define ST3 sizeof(char)*50
#define ST4 sizeof(char)*50+sizeof(int)
#define MT1 malloc(sizeof(int))
#define MT2 malloc(sizeof(double))
#define MT3 malloc(sizeof(char)*50)
#define DATAFILE "data.txt"
#define FIELDSFILE "fields.txt"

/*
 * Used to store a linked list of data for one entity
 */
typedef struct Object
{
	void* var;
	struct Object *nextvar;
}OBJECT;

/*
 * Use to store linked list of fields
 */
typedef struct Fields
{
    char name[50];
    int type;
    struct Fields *next;
}FIELDS;

/*
 * Used to connect the information of one entity to the next
 */
typedef struct Node
{
	OBJECT *start;
	struct Node *next;
}NODE;

/*
 * Used to add a Field at the end
 * Time Complexity O(n)
 */
void addField(FIELDS **p,char* name,int type)
{
	FIELDS *newnode,*temp;
	newnode=malloc(sizeof(FIELDS));
	strcpy(newnode->name,name);
	newnode->type=type;
	newnode->next=NULL;
	if(*p==NULL)
		*p=newnode;
	else
	{
		temp=*p;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=newnode;
	}
}

/*
 * Used to load Fields from a file
 * Time Complexity O(n)
 */
int getFields(FIELDS **p)
{
	FILE *fp=fopen(FIELDSFILE,"rb");
	char n[50];
	int t;
	if(fp==NULL)
		return 0;
	else
	{

		while(!feof(fp))
		{
			if((fread((void*)n,sizeof(n),1,fp))>0)
			{
				fread(&t,sizeof(t),1,fp);
				addField(p,n,t);
			}
			else
				break;
		}
		fclose(fp);
	}
	return 1;
}

/*
 * Used to add an Object at the end of objects for a particular Node
 * Time Complexity O(n)
 */
void addObject(OBJECT **p,int type,void* value)
{
	OBJECT *newnode,*temp;
	newnode=malloc(sizeof(OBJECT));
	switch(type)
	{
		case 1:
			newnode->var=MT1;

		*((int*)newnode->var)=*((int*)value);

			break;
		case 2:
			newnode->var=MT2;
			*((double*)newnode->var)=*((double*)value);
			break;
		case 3:
			newnode->var=MT3;
			strcpy((char*)newnode->var,(char*)value);
			break;
	}
	newnode->nextvar=NULL;
	if(*p==NULL)
		*p=newnode;
	else
	{
		temp=*p;
		while(temp->nextvar!=NULL)
			temp=temp->nextvar;
		temp->nextvar=newnode;
	}
}

/*
 * Used to add a Node at the end of the list of Nodes
 * Time Complexity O(n)
 */
OBJECT** addNode(NODE **p)
{
	NODE *newnode,*temp;
	newnode=(NODE*)malloc(sizeof(NODE));
	newnode->start=NULL;
	newnode->next=NULL;
	if(*p==NULL)
		*p=newnode;
	else
	{
		temp=*p;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=newnode;
	}
	return &(newnode->start);
}

/*
 * Deletes the last Node from the list
 * Time Complexity O(n)
 */
void delLast(NODE **n)
{
	NODE *temp=*n,*prev;
	if(temp->next==NULL)
	{
		*n=NULL;
		free(temp);
	}
	else
	{
		while(temp->next!=NULL)
		{
			prev=temp;
			temp=temp->next;
		}
		prev->next=NULL;
		free(temp);
	}
}

/*
 * Used to load data from a file
 * Time Complexity O(n2)
 */
int getData(NODE **n, FIELDS *f)
{
	OBJECT **o;
	void *val;
	FIELDS *ftemp=f;
	FILE *fp=fopen(DATAFILE,"rb");
	if(fp==NULL)
		return 0;
	while(!feof(fp))
	{
		o=addNode(n);
		ftemp=f;
		while(ftemp!=NULL)
		{
			switch(ftemp->type)
			{
				case 1:
					val=MT1;
					fread(val,ST1,1,fp);
					addObject(o,1,val);
					free(val);
					break;
				case 2:
					val=MT2;
					fread(val,ST2,1,fp);
					addObject(o,2,val);
					free(val);
					break;
				case 3:
					val=MT3;
					fread(val,ST3,1,fp);
					addObject(o,3,val);
					free(val);
					break;
			}
			ftemp=ftemp->next;
		}
	}
	delLast(n);
	return 1;
}

/*
 * Saves the Fields to a file
 * Time Complexity O(n)
 */
int saveFields(FIELDS *f)
{
	FILE *fp=fopen(FIELDSFILE,"wb");
	while(f!=NULL)
	{
		fwrite(f->name,sizeof(f->name),1,fp);
		fwrite(&f->type,sizeof(f->type),1,fp);
		f=f->next;
	}
	fclose(fp);
	return 1;
}

/*
 * Used to Data to a file
 * Time Complexity O(n2)
 */
int saveData(NODE *n,FIELDS *f)
{
	OBJECT *o;
	FIELDS *ftemp;
	FILE *fp=fopen(DATAFILE,"wb");
	while(n!=NULL)
	{
		ftemp=f;
		o=n->start;
		while(o!=NULL)
		{
			switch(ftemp->type)
			{
				case 1:
					fwrite(o->var,ST1,1,fp);
					break;
				case 2:
					fwrite(o->var,ST2,1,fp);
					break;
				case 3:
					fwrite(o->var,ST3,1,fp);
					break;
			}
			o=o->nextvar;
			ftemp=ftemp->next;
		}
		n=n->next;
	}
	fclose(fp);
	return 1;
}

/*
 * Displays the Fields and data on the screen
 * Time Complexity O(n2)
 */
void display(NODE *p,FIELDS *f)
{
	OBJECT *o;

	FIELDS *ftemp=f;
	while(ftemp!=NULL)
	{
		printf("%s\t",ftemp->name);
		ftemp=ftemp->next;
	}
	printf("\n");
	while(p!=NULL)
	{
		o=p->start;
		ftemp=f;
		while(o!=NULL)
		{
			switch(ftemp->type)
			{
				case 1:
					printf("%d\t",*((int*)o->var));
					break;
				case 2:
					printf("%.2lf\t",*((double*)o->var));
					break;
				case 3:
					printf("%s\t",(char*)o->var);
					break;
			}
			ftemp=ftemp->next;
			o=o->nextvar;
		}
		printf("\n");
		p=p->next;
	}
}

/*
 * Frees all the data and sets linked list pointers to NULL
 * Time Complexity O(n2)
 */
void freeSpace(NODE **n,FIELDS **f)
{
	NODE *ntemp;
	FIELDS *ftemp;
	OBJECT **o,*otemp;
	while(*f!=NULL)
	{
		ftemp=*f;
		*f=(*f)->next;
		free(ftemp);
	}
	while(*n!=NULL)
	{
		ntemp=*n;
		o=&(ntemp->start);
		while(*o!=NULL)
		{
			otemp=*o;
			*o=(*o)->nextvar;
			free(otemp->var);
			free(otemp);
		}
		*n=(*n)->next;
		free(ntemp);
	}
}

int main(void)
{
	NODE *n=NULL;
	OBJECT **o;
	FIELDS *f=NULL,*ftemp;
	char a[50];
	void *val;
	int ch,c=0;
	getFields(&f);
	if(f!=NULL)
		c=getData(&n,f);
	else
	{
		do
		{
			printf("Enter Field Type :\n1.Integer\n2.Double\n3.String\n4.Exit\n");
			scanf("%d",&ch);
			switch(ch)
			{
				case 1:
				case 2:
				case 3:
					printf("Enter name of Field:");
					scanf("%s",a);
					addField(&f,a,ch);
					break;
				case 4:
					break;
				default:
					printf("Invalid Choice");
			}
		}while(ch!=4);
	}
	if(c)
	{
		printf("Add Data?(1/0)\n");
		scanf("%d",&c);
	}
	else
		c=1;
	if(c)
	{
		do
		{
			if(f==NULL)
				return 0;
			printf("1.Enter Data\n2.Exit\n");
			scanf("%d",&ch);
			switch(ch)
			{
				case 1:
					printf("\nAdd Entry:\n\n");
					ftemp=f;
					o=addNode(&n);
					while(ftemp!=NULL)
					{
						printf("Enter %s:",ftemp->name);
						switch(ftemp->type)
						{
							case 1:
								val=MT1;
								scanf("%d",(int*)val);
								addObject(o,1,val);
								free(val);
								break;
							case 2:
								val=MT2;
								scanf("%lf",(double*)val);
								addObject(o,2,val);
								free(val);
								break;
							case 3:
								val=MT3;
								scanf("%s",(char*)val);
								addObject(o,3,val);
								free(val);
								break;
							default:
								printf("Error in Field Type");
								continue;
						}
						ftemp=ftemp->next;
					}
					break;
				case 2:break;
				default:
					printf("Invalid Choice");
					break;
			}
		}while(ch!=2);
	}
	display(n,f);
	saveFields(f);
	saveData(n,f);
	freeSpace(&n,&f);
	return 0;
}
