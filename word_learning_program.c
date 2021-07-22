#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


typedef struct a_node{ /* antonyms node */

    char *word;
    char **antonyms;
    int num; /* this number indicates the num of antonyms */
    struct a_node *next;
}a_node;

typedef struct s_node{ /* synonym node */
    
    char *word;
    char **synonyms;
    int num; /* this number indicates the num of synonyms */
    struct s_node *next;
}s_node;

typedef struct u_node{ /* user file node we will read the user file with this node */

    char c;
    int order;
    int isTrue;
    int priority;
}u_node;

void insert_antonyms( a_node **head, a_node *newnode){ /* this function insert antonym node to the full node at the end */

        a_node *last;

        last = *head;

        if( (*head) == NULL){/* in the case *head has no member */
                (*head) = newnode;
                return;
        }

        while( (last)->next != NULL) (last) = (last)->next; /* coming to the last element pointer */

        (last)->next = newnode;

}
void insert_synonyms( s_node **head, s_node *newnode){ /* this function insert synonym node to the full node at the end */

        s_node *last;

        last = *head;

        if( (*head) == NULL){/* in the case *head has no member */
                (*head) = newnode;
                return;
        }

        while( (last)->next != NULL) (last) = (last)->next;/* coming to the last element pointer */

        (last)->next = newnode;

}
int pull_synonyms( s_node **_head_s){ /* pull means we read the synonym files datas */

    char word[__WORDSIZE]; /* just a word use for all the words inside the file */
    char c;
    int i=0, wordct=0, count=0;
    
    s_node *head_s = *_head_s;
    s_node *ns;

    FILE *fp;
    fp = fopen("synonyms.txt","r");
    while( c != EOF){ 

        count++;
        ns = ( s_node *) malloc( sizeof( s_node)); /* we create node */
        ns->next = NULL;
        ns->num = 0;
        do{

            c = fgetc( fp); /* reading letter */
            if( c != '=' && c != '\n' && c != EOF ) word[i] = c; /* we will assign the letter if they are wanted ones */
            if( c == '=' || c == 13 ) i--;/* skipping = because we don't want space in our word string */

            if( c == ' ' || c == '\n' || c == EOF ){ /* these are conditions we understand we have completed a word */

                word[i] = '\0';/* we put null character at the end */
                if( strlen( word) > 1){ 
                    
                    if( wordct == 0){

                        wordct++;
                        ns->word = ( char *) calloc( (strlen( word) +1), sizeof( char));/* allocating for the word which has synonyms */
                        strcpy( ns->word, word);/*and put the word inside it*/
                    }
                    else{

                        if( wordct == 1) ns->synonyms = ( char **) malloc( wordct* sizeof( char *)); /* for the first synonym word we use malloc */
                        else ns->synonyms = ( char **) realloc( ns->synonyms, sizeof( char *)* (wordct));/* then we use realloc*/
                        /* putting the all the word to the synonym 2d array after the main word */
	                    ns->synonyms[wordct-1] = (char *) calloc( (strlen( word) +1), sizeof( char));
                        strcpy( ns->synonyms[wordct-1], word);
                        ns->num = wordct;
                        wordct++;
                    }

                }

                i=-1;/* we assing it to -1 because we will already make it zero next step */
            }
        
            i++;
        }while( c != '\n' && c != EOF);/* this means we ended a row in the file */

        insert_synonyms( _head_s, ns);/* since we ended the row it means we completed a node so far so we insert it */
        wordct = 0;/* and making the wordct 0 because wordct indicates the synonym words number */
    }
    fclose( fp);
    return count;
}


int pull_antonyms( a_node **_head_a){/* pull means we read the antonym files datas */

    char word[__WORDSIZE];/* just a word use for all the words inside the file */
    char c;
    int i=0, wordct=0, count =0;
    
    a_node *head_a = *_head_a;
    a_node *na;
    FILE *fp;

    fp = fopen("antonyms.txt","r");
    while( c != EOF){

        count++;
        na = ( a_node *) malloc( sizeof( a_node)); /* we create node */
        na->next = NULL;
        na->num = 0;
        do{

            c = fgetc( fp);/* reading letter */
            
            if( c != '<' && c != '>' && c != '\n' && c != EOF) word[i] = c;/* we will assign the letter if they are wanted ones */
            if( c == '<' || c == '>' || c == 13) i--;/* skipping < and > and vertical tab because we don't want space in our word string */

            if( c == ' ' || c == '\n' || c == EOF){/* these are conditions we understand we have completed a word */

                word[i] = '\0';/* we put null character at the end */
                
                if( strlen( word) > 1){

                    if( wordct == 0){

                        wordct++;
                        na->word = ( char *) calloc( (strlen( word) +1), sizeof( char));/* allocating for the word which has antonyms */
                        strcpy( na->word, word);/*and put the word inside it*/
                    }
                    else{

                        if( wordct == 1) na->antonyms = ( char **) malloc( wordct* sizeof( char *));/* for the first antonym word we use malloc */
                        else na->antonyms = ( char **) realloc( na->antonyms, sizeof( char *)* (wordct));/* then we use realloc*/
                        /* putting the all the word to the antonym 2d array after the main word */
	                    na->antonyms[wordct-1] = ( char*) calloc( (strlen( word) +1), sizeof( char));
                        strcpy( na->antonyms[wordct-1], word);
                        na->num = wordct;
                        wordct++;
                    }
                }

                
                i=-1;/* we assing it to -1 because we will already make it zero next step */
            }
        
            i++;
        }while( c != '\n' && c != EOF);/* this means we ended a row in the file */
        
        insert_antonyms( _head_a, na);/* since we ended the row it means we completed a node so far so we insert it */
        wordct = 0;/* and making the wordct 0 because wordct indicates the antonym words number */
    }
    fclose( fp);
    return count;
}

char *userfile(){/* this is user interface and returns the name of the userfile properly */

    FILE *fusers, *fp;
    char word[__WORDSIZE];
    char name[__WORDSIZE];
    char *filename = calloc( __WORDSIZE, sizeof(char));
    int flag = 0, control;

    printf("Currently users are: \n");
    fusers = fopen("users.txt","a+");/* firstly we're showing the user to be selected*/
    while( !feof( fusers)){

        control = fscanf( fusers,"%s",word);
        if( control == 1) printf("%s\n", word);
    }

    printf("Enter the user name you want to select or create: ");
    scanf("%s", name);/* taking the name from the user*/

    fseek(fusers, 0, SEEK_SET);/* taking the user.txt at the beginning */
    while( !feof( fusers)){

        fscanf( fusers,"%s",word);
        if( strcmp( word, name) == 0) flag = 1;
    }
    if( flag != 1){/* if the user new we adding it to the users.txt*/
        fprintf( fusers,"%s\n", name);
    }
    fclose(fusers);

    strcpy( filename, name);
    strcat( filename, ".worddat");/* making the .worddat file name with strcat */

    printf("Welcome to the system %s\n", name);
    return filename;/* returning the username.worddat */
}

void update_antonyms(a_node *head){ /* we're updating the files with new antonym linked list */

    FILE *fp;
    a_node *last = head;
    int i;

    fp = fopen("antonyms.txt", "w");
    while( last != NULL && last->word != NULL){

        fprintf( fp,"%s <> ", last->word);/* main word is written with <> */

        for( i=0; i<(last->num); ++i){/* then we printing the antonym words */

            if( i == ((last->num)-1)){/*last antonym word*/
                fprintf( fp,"%s\n", last->antonyms[i]);
            }
            else{
                fprintf(fp,"%s ", last->antonyms[i]);/* antonym words except the last one */
            }
            
        }
        last = last->next;
    }
    fclose( fp);
}
void update_synonyms(s_node *head){ /* we're updating the files with new synonym linked list */

    FILE *fp;
    s_node *last = head;
    int i;

    fp = fopen("synonyms.txt", "w");
    while( last != NULL && last->word != NULL){

        fprintf( fp,"%s = ", last->word);/* main word is written with = */

        for( i=0; i<(last->num); ++i){/* then we printing the synonym words */

            if( i == ((last->num)-1)){/*last synonym word*/
                fprintf(fp,"%s\n", last->synonyms[i]);
            }
            else{
                fprintf(fp,"%s ", last->synonyms[i]);/* synonym words except the last one */
            }
            
        }
        last = last->next;
    }
    fclose( fp);
}
void write_status( char *userfile, char c, int order, int isTrue, int priority){ /* this function writes the status of the user */

    u_node node;
    int temp;
    FILE *fp;
    fp = fopen( userfile, "ab+");/* we are opening our binary file appending plus mode */

    while( !feof( fp)){/* we are scanning the file because in case of match we will update it */

        fread( &(node.c),sizeof( char),1,fp);
        fread( &(node.order),sizeof(int),1,fp);

        /* we searching it according to order because order is unique for the words */
        if( node.order == order){/* in case of match we update it */

            fwrite(&isTrue, sizeof( int), 1, fp);
            fread( &(priority),sizeof(int),1,fp);
            fseek(fp, (-1)* sizeof( int), SEEK_CUR);

            priority += temp;
            fwrite(&priority, sizeof( int), 1, fp);
            return;
        }
        else{
            fread( &(node.isTrue),sizeof(int),1,fp);
            fread( &(node.priority),sizeof(int),1,fp);
        }
    }
    /* in case of no match we already at the end of the file so we just write our values */
    fwrite(&c, sizeof( char), 1, fp);
    fwrite(&order, sizeof( int), 1, fp);
    fwrite(&isTrue, sizeof( int), 1, fp);
    fwrite(&priority, sizeof( int), 1, fp);

    fclose( fp);
}
int synorant_generator( char *userfile){

    int i, j, total_a=0, total_s=0, temp;
    u_node node;
    FILE *fp;

    srand( time( NULL));

    fp = fopen( userfile,"rb");

    while( !feof( fp)){/* we are scanning the user file because we have information of the syn or ant representator in the file */

        fread( &(node.c),sizeof(int),1,fp);
        fread( &(node.order),sizeof(int),1,fp);
        fread( &(node.isTrue),sizeof(int),1,fp);
        fread( &(node.priority),sizeof(int),1,fp);

        if( node.c == 'a' ){/* we count the antonyms words */
            total_a++;
        }
        else if( node.c == 's'){/* we count the synonym words */
            total_s++;
        }
    }

    fclose( fp);

    temp = total_a + total_s;/* we add up total possibilities */
    if( total_a == 0 || total_s == 0){/* in case of there is no data we just deciding randomly */

        temp = (rand()%2)+1;
        return temp;
    }
    else temp = rand()%temp;/* we taking random number of total possibilities */
    
    /* if our random number is bigger than antonyms we return synonym because it means synonym has more possiblities*/
    if( temp > total_a) return 2;/* returning synonym*/
    else return 1;/* returning antonym*/
}
int nth_generator( char *userfile, int synorant, int count){

    int i, j, total=0, k, nth;
    u_node *arr;
    u_node node;
    FILE *fp;
    char c;

    arr = (u_node *) calloc( count, sizeof( u_node));/* just making an user node array */
    if( synorant == 1) c = 's';
    else if( synorant == 2) c = 'a';

    srand(time(NULL));

    fp = fopen( userfile,"rb");
    
    for( i=0; i<count; ++i){/* starting our priorities with two because we will work with 2 multiples */
        arr[i].priority = 2;
    }
    while( !feof( fp)){
        /* reading the file and we will check whether it's true or not */
        fread( &(node.c),sizeof(int),1,fp);
        fread( &(node.order),sizeof(int),1,fp);
        fread( &(node.isTrue),sizeof(int),1,fp);
        fread( &(node.priority),sizeof(int),1,fp);

        if( node.c != c ) continue;

        /*since the word asked we increase the proportion of the rest*/
        for( i=0; i< count; ++i){

            if( i== node.priority) continue;/* we skip the current one*/
            arr[i].priority *= (2*node.priority);
        }
        if( node.isTrue == 0){/* correct answered */

            node.priority /= 2; /* in case of correct we divide the priority by 2 */
        }
        else if( node.isTrue == -1){/* wrong answered */

            node.priority *=2;/* we increase the priority of the node */
        }

        arr[ (node.order)] = node; /* and we assing the new node according to it's unique node.order */
    }
    for( i=0; i<count; ++i){
        total +=arr[i].priority;/* we're adding up all the priorities */
    }

    nth = rand() % total;/* random number 0to total */
    k=0;
    for( i=0; i< count; ++i){
        
        for( j=0; j< arr[i].priority; j++, k++){
            
            if( k == nth){/* there is a k value and we increase it every priorities and total priority if equals the k we return current i*/

                fclose( fp);
                free( arr);
                return i;
            }
        }
    }

    fclose( fp);
}
int ask_question( char *userfile){

        char word[__WORDSIZE];
        a_node *head_a = NULL, *lasta;
        s_node *head_s = NULL, *lasts;
        int countsyn, countant;

        int control = 0;
        int selection, synorant, i, nth;

        srand( time( NULL));
        /* firstly decide to synon or anton first */
        /* we will consider 1 for ant 2 for syn*/
        synorant = synorant_generator( userfile);

        if( synorant == 1){/*antonyms*/

            countant= pull_antonyms( &head_a);
            lasta = head_a;

            nth = nth_generator( userfile, 1, countant);

            for( i = 0; i<=nth && (lasta->next->word != NULL) ; ++i) lasta = lasta->next;/* we're finding the wanted node of the linked list*/

            printf("What is antonym of the %s: ", lasta->word);
            scanf("%s", word);
            
            for( i=0; i<(lasta->num); i++){/* controlling whether the word is one of the antonyms or not */

                
                if( strcmp(lasta->antonyms[i], word) == 0){/*in case of match*/

                    control = 1;
                    break;
                }
                control = -1;/* otherwise assignin -1*/
            }
        }
        else if( synorant == 2){/*synonyms*/

            countsyn= pull_synonyms( &head_s);
            lasts = head_s;

            nth = nth_generator( userfile, 2, countsyn);

            for( i = 0; i<=nth && (lasts->next->word != NULL); ++i) lasts = lasts->next;/* we're finding the wanted node of the linked list*/

            printf("What is synonym of the %s: ", lasts->word);
            scanf("%s", word);

            for( i=0; i<(lasts->num); i++){/* controlling whether the word is one of the synonyms or not */

                if( strcmp(lasts->synonyms[i], word) == 0){/*in case of match*/

                    control = 1;
                    break;
                }
                control = -1;/* otherwise assignin -1*/
            }
        }

        if( control == -1){/* false answer */
            /* if the answer not matched we ask to user that do you want to add this word for a right answer  */
            printf("Do you want to add this answer as a correct answer ? (1:yes, 2:no) ");
            scanf("%d", &selection);
            if( selection == 1){
                
                /* according to selection we add this as a new answer */
                if( synorant == 1){/* antonyms */

                    lasta->antonyms = (char **) realloc(lasta->antonyms, sizeof( char *)* (lasta->num + 1));/* so we increase the char* with realloc*/
                    lasta->antonyms[lasta->num] = (char *) calloc( __WORDSIZE, sizeof( char));
                    strcpy(lasta->antonyms[lasta->num], word);/* then adding the word */
                    lasta->num++;/* num means amount of the antonyms so we increase it because we have new answer */
                    
                    update_antonyms( head_a);
                }
                else if( synorant == 2){/* synonyms */
                    
                    lasts->synonyms = (char **) realloc(lasts->synonyms, sizeof( char *)* (lasts->num + 1));
                    lasts->synonyms[lasts->num] = (char *) calloc( __WORDSIZE, sizeof( char));
                    strcpy(lasts->synonyms[lasts->num], word);
                    lasts->num++;

                    update_synonyms( head_s);
                }
            }
            /* at the end we updating userfile as a wrong ant/syn answer */
            if( synorant == 1){/* antonyms */
                write_status( userfile, 'a', nth+1, -1, 2);
            }
            else if( synorant == 2){/* synonyms */
                write_status( userfile, 's', nth+1, -1, 2);
            }

        }
        else if( control == 1){ /* correct answer */
            /* if the answer matched we ask to user that do you want to add additional word for this word  */
            printf("Do you want to add additional answers? (1:yes, 2:no)");
            scanf("%d", &selection);
            /* according to selection we add this as a new answer */
            if( selection == 1){
                
                printf("Enter the word : ");
                scanf("%s", word);
                if( synorant == 1){/* antonyms */

                    lasta->antonyms = (char **) realloc(lasta->antonyms, sizeof( char *)* (lasta->num + 1));/* so we increase the char* with realloc*/
                    lasta->antonyms[lasta->num] = (char *) calloc( __WORDSIZE, sizeof( char));
                    strcpy(lasta->antonyms[lasta->num], word);/* then adding the word */
                    lasta->num++;/* num means amount of the antonyms so we increase it because we have new answer */

                    write_status( userfile, 'a', lasta->num, 0, 1);
                    update_antonyms( head_a);
                }
                else if( synorant == 2){/* synonyms */
                    
                    lasts->synonyms = (char **) realloc(lasts->synonyms, sizeof( char *)* (lasts->num + 1));
                    lasts->synonyms[lasts->num] = (char *) calloc( __WORDSIZE, sizeof( char));
                    strcpy(lasts->synonyms[lasts->num], word);
                    lasts->num++;

                    write_status( userfile, 's', lasts->num, 0, 1);
                    update_synonyms( head_s);
                }
            }
            /* at the end we updating userfile as a wrong ant/syn answer */
            if( synorant == 1){/* antonyms */
                write_status( userfile, 'a', nth+1, 0, 2);
            }
            else if( synorant == 2){/* synonyms */
                write_status( userfile, 's', nth+1, 0, 2);
            }
        }

        if( head_a != NULL) free( head_a);
        if( head_s != NULL) free( head_s);

        printf("Do you want to terminate the program? (1:yes, 2:no) ");
        scanf("%d", &selection);
        if( selection == 1) return -1;
        
}
void start(){ /* play game function is execute userfile interface and asks question and according to return value of question it ends it */

    FILE *fp;
    char *filename = userfile();
    int selection, control;
    int synorant, nth;

    fp = fopen( filename,"a+b");/* we just in the case of new user create the file */
    fclose( fp);
    while( 1>0){

        control = ask_question( filename);
        if( control == -1) break;
    }
}

int main(){

    start();
}
